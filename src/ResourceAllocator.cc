#include <d2d1.h>
#include <wrl/client.h>

#include <cstdint>
#include <kke/ResourceAllocator.hpp>

#include "kke/RenderSurface.hpp"
#include "kke/font/FontLoader.hpp"
#include "kke/internal/Hasher.hpp"

kke::ResourceAllocator::ResourceAllocator(ID2D1DeviceContext* context, FontLoader* fontLoader)
	: deviceContext(context), fontLoader(fontLoader), brushStorage() {
}

Microsoft::WRL::ComPtr<ID2D1Brush> kke::ResourceAllocator::aquireOrCreateBrush(kke::Brush const& brush) {
	uint64_t key = brush.hash();
	Microsoft::WRL::ComPtr<ID2D1Brush> cachedBrush = brushStorage.get(key);
	if (cachedBrush) {
		return cachedBrush;
	}
	Microsoft::WRL::ComPtr<ID2D1Brush> brushInstance;
	brush.create(deviceContext, &brushInstance);
	brushStorage.put(brush.hash(), brushInstance);
	return brushInstance;
}

Microsoft::WRL::ComPtr<ID2D1Image> kke::ResourceAllocator::aquireOrDispatchShadow(kke::Geometry const& geometry, kke::Brush const& brush, float strength, std::function<void(ID2D1Image**)> dispatchFunc) {
	Hasher hasher;
	hasher.combine(geometry.hash(false));
	hasher.combine(brush.hash());
	hasher.combine(strength);
	Microsoft::WRL::ComPtr<ID2D1Image> cachedShadow = shadowStorage.get(hasher.get());
	if (cachedShadow) {
		return cachedShadow;
	}
	Microsoft::WRL::ComPtr<ID2D1Image> shadowOutput;
	dispatchFunc(&shadowOutput);
	shadowStorage.put(hasher.get(), shadowOutput);
	return shadowOutput;
}

kke::RenderSurface* kke::ResourceAllocator::aquireOrCreateSurface(ID2D1DeviceContext* context) {
	// TODO: Implement resource limit?
	for (auto& surface : surfaces) {
		if (!surface.isLocking()) {
			return &surface;
		}
	}
	kke::RenderSurface surface = kke::RenderSurface::createSurface(context);
	surfaces.push_back(surface);
	return &surfaces.back();
}

IDWriteTextFormat* kke::ResourceAllocator::aquireOrCreateTextFormat(std::wstring const& fontFamily, int32_t fontSize, kke::FontWeight weight) {
    Hasher hasher;
    hasher.combine(fontFamily);
    hasher.combine(fontSize);
    hasher.combine(static_cast<uint32_t>(weight));
    uint64_t key = hasher.get();
    Microsoft::WRL::ComPtr<IDWriteTextFormat> cachedFormat = textFormatStorage.get(key);
    if (cachedFormat) {
        return cachedFormat.Get();
    }
    Microsoft::WRL::ComPtr<IDWriteTextFormat> textFormat = fontLoader->createTextFormat(fontFamily, fontSize, weight);
    textFormatStorage.put(key, textFormat);
    return textFormat.Get();
}

IDWriteTextLayout* kke::ResourceAllocator::aquireOrCreateTextLayout(std::wstring const& text, IDWriteTextFormat* textFormat) {
    Hasher hasher;
    hasher.combine(text);
    hasher.combine(reinterpret_cast<uint64_t>(textFormat));
    uint64_t key = hasher.get();
    Microsoft::WRL::ComPtr<IDWriteTextLayout> cachedLayout = textLayoutStorage.get(key);
    if (cachedLayout) {
        return cachedLayout.Get();
    }
    Microsoft::WRL::ComPtr<IDWriteTextLayout> textLayout = fontLoader->createTextLayout(text, textFormat);
    textLayoutStorage.put(key, textLayout);
    return textLayout.Get();
}
