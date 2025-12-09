#include <d2d1.h>
#include <wincodec.h>
#include <wrl/client.h>

#include <cstdint>
#include <kke/ResourceAllocator.hh>

#include "kke/RenderSurface.hh"
#include "kke/effect/EffectInstance.hh"
#include "kke/font/FontLoader.hh"
#include "kke/internal/Hasher.hh"

using namespace kke;

ResourceAllocator::ResourceAllocator(ID2D1Factory* factory, ID2D1DeviceContext* context, FontLoader* fontLoader)
	: factory(factory), deviceContext(context), fontLoader(fontLoader), brushStorage() {
}

void ResourceAllocator::nextFrame() {
	for (auto& effectInstance : effectInstances) {
		effectInstance->unlock();
	}
}

ComPtr<ID2D1Brush> ResourceAllocator::acquireOrCreateBrush(Brush const& brush) {
	uint64_t key = brush.hash();
	ComPtr<ID2D1Brush> cachedBrush = brushStorage.get(key);
	if (cachedBrush) {
		return cachedBrush;
	}
	ComPtr<ID2D1Brush> brushInstance;
	brush.create(deviceContext, &brushInstance);
	brushStorage.put(brush.hash(), brushInstance);
	return brushInstance;
}

ComPtr<ID2D1Geometry> ResourceAllocator::acquireOrCreateGeometry(Geometry const& geometry) {
	uint64_t key = geometry.hash(true);
	ComPtr<ID2D1Geometry> cachedGeometry = geometryStorage.get(key);
	if (cachedGeometry) {
		return cachedGeometry;
	}
	ComPtr<ID2D1Geometry> geometryInstance;
	geometry.create(factory, &geometryInstance);
	geometryStorage.put(geometry.hash(true), geometryInstance);
	return geometryInstance;
}

ComPtr<ID2D1Geometry> ResourceAllocator::acquireOrCreateInvertedGeometry(Geometry const& geometry) {
	uint64_t key = geometry.hash(true);
	ComPtr<ID2D1Geometry> cachedGeometry = invertedGeometryStorage.get(key);
	if (cachedGeometry) {
		return cachedGeometry;
	}

	// viewport rectangle
	ComPtr<ID2D1RectangleGeometry> rectGeo = nullptr;
	D2D1_SIZE_U viewport = deviceContext->GetPixelSize();
	factory->CreateRectangleGeometry(
		{0.0f, 0.0f, static_cast<FLOAT>(viewport.width), static_cast<FLOAT>(viewport.height)},
		&rectGeo);

	ComPtr<ID2D1Geometry> baseGeometry;
	geometry.create(factory, &baseGeometry);

	// Combine with the original geometry using a geometry group with the "exclude" mode
	ID2D1PathGeometry* combinedGeometry;
	ID2D1GeometrySink* geometrySink;
	factory->CreatePathGeometry(&combinedGeometry);
	combinedGeometry->Open(&geometrySink);
	rectGeo->CombineWithGeometry(
		baseGeometry.Get(),
		D2D1_COMBINE_MODE_EXCLUDE,
		nullptr,
		geometrySink);
	geometrySink->Close();

	invertedGeometryStorage.put(geometry.hash(true), combinedGeometry);
	return combinedGeometry;
}

ComPtr<ID2D1Image> ResourceAllocator::acquireOrDispatchShadow(Geometry const& geometry, Brush const& brush, float strength, std::function<void(ID2D1Image**)> dispatchFunc) {
	Hasher hasher;
	hasher.combine(geometry.hash(false));
	hasher.combine(brush.hash());
	hasher.combine(strength);
	return acquireOrDispatchShadow(geometry, hasher.get(), brush, strength, dispatchFunc);
}

ComPtr<ID2D1Image> ResourceAllocator::acquireOrDispatchShadow(Geometry const& geometry, uint64_t identifierHash, Brush const& brush, float strength, std::function<void(ID2D1Image**)> dispatchFunc) {
	ComPtr<ID2D1Image> cachedShadow = shadowStorage.get(identifierHash);
	if (cachedShadow) {
		return cachedShadow;
	}
	ComPtr<ID2D1Image> shadowImage;
	dispatchFunc(&shadowImage);
	shadowStorage.put(identifierHash, shadowImage);
	return shadowImage;
}

IDWriteTextFormat* ResourceAllocator::acquireOrCreateTextFormat(std::wstring const& fontFamily, int32_t fontSize, FontWeight weight) {
	Hasher hasher;
	hasher.combine(fontFamily);
	hasher.combine(fontSize);
	hasher.combine(static_cast<uint32_t>(weight));
	uint64_t key = hasher.get();
	ComPtr<IDWriteTextFormat> cachedFormat = textFormatStorage.get(key);
	if (cachedFormat) {
		return cachedFormat.Get();
	}
	ComPtr<IDWriteTextFormat> textFormat = fontLoader->createTextFormat(fontFamily, fontSize, weight);
	textFormatStorage.put(key, textFormat);
	return textFormat.Get();
}

IDWriteTextLayout* ResourceAllocator::acquireOrCreateTextLayout(std::wstring const& text, IDWriteTextFormat* textFormat) {
	Hasher hasher;
	hasher.combine(text);
	hasher.combine(reinterpret_cast<uint64_t>(textFormat));
	uint64_t key = hasher.get();
	ComPtr<IDWriteTextLayout> cachedLayout = textLayoutStorage.get(key);
	if (cachedLayout) {
		return cachedLayout.Get();
	}
	ComPtr<IDWriteTextLayout> textLayout = fontLoader->createTextLayout(text, textFormat);
	textLayoutStorage.put(key, textLayout);
	return textLayout.Get();
}

EffectInstance* ResourceAllocator::acquireOrCreateEffect(std::shared_ptr<Effect> effect) {
	// TODO: Implement resource limit?
	for (auto& effectInstance : effectInstances) {
		bool sameEffect = effectInstance->getGUID() == effect->effectGuid();
		if (sameEffect && !effectInstance->isLocking()) {
			return effectInstance.get();
		}
	}
	auto effectInstance = std::make_shared<EffectInstance>(deviceContext, effect->effectGuid());
	effectInstances.push_back(effectInstance);
	return effectInstance.get();
}

RenderSurface* ResourceAllocator::acquireOrCreateSurface() {
	// TODO: Implement resource limit?
	for (auto& surface : surfaces) {
		if (!surface->isLocking()) {
			return surface.get();
		}
	}
	auto surface = RenderSurface::createSurface(deviceContext);
	surfaces.push_back(surface);
	return surface.get();
}
