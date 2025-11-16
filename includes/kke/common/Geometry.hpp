#pragma once

#include <d2d1.h>
#include <d2d1_1.h>
#include <cstdint>

namespace kke {
enum class GeometryType : uint32_t {
	ELLIPSE = 0,
	RECT = 1,
	ROUNDED_RECT = 2
};

class Geometry {
public:
	virtual ~Geometry() = default;

	virtual void create(ID2D1Factory* factory, ID2D1Geometry** output) const = 0;

	virtual uint64_t hash(bool positionDependent) const = 0;
};
};	// namespace kke
