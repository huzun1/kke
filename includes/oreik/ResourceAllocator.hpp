#pragma once

#include <d2d1.h>
#include <d2d1_1.h>
#include <wincodec.h>
#include <wrl/client.h>

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
	CacheStorage<ID2D1Image> shadowStorage;
	std::vector<RenderSurface> surfaces;

public:
	ResourceAllocator(ID2D1DeviceContext* context);

	Microsoft::WRL::ComPtr<ID2D1Brush> aquireOrCreateBrush(oreik::Brush const& brush);

	Microsoft::WRL::ComPtr<ID2D1Image> aquireOrDispatchShadow(oreik::Geometry const& geometry, oreik::Brush const& brush, float strength, std::function<void(ID2D1Image**)> dispatchFunc);

	oreik::RenderSurface* aquireOrCreateSurface(ID2D1DeviceContext* context);
};
};	// namespace oreik
