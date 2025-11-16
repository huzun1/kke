#include <d2d1.h>
#include <wrl/client.h>

#include <cstdint>
#include <kke/ResourceAllocator.hpp>

#include "kke/RenderSurface.hpp"
#include "kke/font/FontLoader.hpp"
#include "kke/internal/Hasher.hpp"

using namespace kke;

ResourceAllocator::ResourceAllocator(ID2D1Factory* factory, ID2D1DeviceContext* context, FontLoader* fontLoader)
	: factory(factory), deviceContext(context), fontLoader(fontLoader), brushStorage() {
}

ComPtr<ID2D1Brush> ResourceAllocator::aquireOrCreateBrush(Brush const& brush) {
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

ComPtr<ID2D1Geometry> ResourceAllocator::aquireOrCreateGeometry(Geometry const& geometry) {
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

ComPtr<ID2D1Image> ResourceAllocator::aquireOrDispatchShadow(Geometry const& geometry, Brush const& brush, float strength, std::function<void(ID2D1Image**)> dispatchFunc) {
	Hasher hasher;
	hasher.combine(geometry.hash(false));
	hasher.combine(brush.hash());
	hasher.combine(strength);
	ComPtr<ID2D1Image> cachedShadow = shadowStorage.get(hasher.get());
	if (cachedShadow) {
		return cachedShadow;
	}
	ComPtr<ID2D1Image> shadowOutput;
	dispatchFunc(&shadowOutput);
	shadowStorage.put(hasher.get(), shadowOutput);
	return shadowOutput;
}

RenderSurface* ResourceAllocator::aquireOrCreateSurface(ID2D1DeviceContext* context) {
	// TODO: Implement resource limit?
	for (auto& surface : surfaces) {
		if (!surface.isLocking()) {
			return &surface;
		}
	}
	RenderSurface surface = RenderSurface::createSurface(context);
	surfaces.push_back(surface);
	return &surfaces.back();
}

IDWriteTextFormat* ResourceAllocator::aquireOrCreateTextFormat(std::wstring const& fontFamily, int32_t fontSize, FontWeight weight) {
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

IDWriteTextLayout* ResourceAllocator::aquireOrCreateTextLayout(std::wstring const& text, IDWriteTextFormat* textFormat) {
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
