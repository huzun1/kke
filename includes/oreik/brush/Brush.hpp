#pragma once

#include <d2d1.h>
#include <d2d1_1.h>

#include <cstdint>

namespace oreik {
enum class BrushType : uint32_t {
	SOLID_BRUSH,
	LINEAR_GRADIENT_BRUSH
};

class Brush {
public:
	virtual ~Brush() = default;

	virtual void create(ID2D1DeviceContext* context, ID2D1Brush** output) const = 0;

	virtual uint64_t hash() const = 0;
};
};	// namespace oreik
