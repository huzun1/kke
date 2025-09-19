#pragma once

#include <d2d1.h>
#include <d2d1_1.h>

#include <functional>
#include <vector>

#include "CacheStorage.hpp"
#include "brush/Brush.hpp"
#include "oreik/CacheStorage.hpp"
#include "oreik/RenderSurface.hpp"
#include "oreik/common/Geometry.hpp"

namespace oreik {
class ResourceAllocator {
	ID2D1DeviceContext* deviceContext;

	CacheStorage<ID2D1Brush> brushStorage;
	CacheStorage<ID2D1Bitmap1> shadowStorage;
	std::vector<RenderSurface> surfaces;

public:
	ResourceAllocator(ID2D1DeviceContext* context);

	ID2D1Brush* aquireOrCreateBrush(oreik::Brush const& brush);

	ID2D1Bitmap1* aquireOrDispatchShadow(oreik::Geometry const& geometry, float strength, std::function<ID2D1Bitmap1*()> dispatchFunc);

	oreik::RenderSurface* aquireOrCreateSurface(ID2D1DeviceContext* context);
};
};	// namespace oreik
