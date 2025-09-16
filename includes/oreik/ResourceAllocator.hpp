#pragma once

#include <d2d1.h>
#include <d2d1_1.h>

#include "CacheStorage.hpp"
#include "brush/Brush.hpp"

namespace oreik {
class ResourceAllocator {
	ID2D1DeviceContext* deviceContext;

	CacheStorage<ID2D1Brush> brushStorage;

public:
	ResourceAllocator(ID2D1DeviceContext* context);

	ID2D1Brush* aquireOrCreate(oreik::Brush const& brush);
};
};	// namespace oreik
