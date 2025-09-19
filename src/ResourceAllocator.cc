#include <d2d1.h>

#include <cstdint>
#include <cstdio>
#include <optional>
#include <oreik/ResourceAllocator.hpp>

#include "oreik/RenderSurface.hpp"

oreik::ResourceAllocator::ResourceAllocator(ID2D1DeviceContext* context)
	: deviceContext(context), brushStorage() {
}

ID2D1Brush* oreik::ResourceAllocator::aquireOrCreateBrush(oreik::Brush const& brush) {
	uint64_t key = brush.hash();
	std::optional<ID2D1Brush*> cachedBrush = brushStorage.get(key);
	if (cachedBrush) {
		return cachedBrush.value();
	}

	ID2D1Brush* brushInstance = brush.create(deviceContext);
	brushStorage.put(brush.hash(), brushInstance);
	return brushInstance;
}

ID2D1Bitmap1* oreik::ResourceAllocator::aquireOrDispatchShadow(oreik::Geometry const& geometry, float strength, std::function<ID2D1Bitmap1*()> dispatchFunc) {
}

oreik::RenderSurface* oreik::ResourceAllocator::aquireOrCreateSurface(ID2D1DeviceContext* context) {
	// TODO: Implement resource limit?
	for (auto& surface : surfaces) {
		if (!surface.isLocking()) {
			return &surface;
		}
	}

	oreik::RenderSurface surface = oreik::RenderSurface::createSurface(context);
	surfaces.push_back(surface);
	return &surfaces.back();
}
