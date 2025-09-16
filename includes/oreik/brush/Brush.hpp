#pragma once

#include <d2d1.h>
#include <d2d1_1.h>

#include <cstdint>

namespace oreik {
class Brush {
public:
	virtual ~Brush() = default;

	virtual ID2D1Brush* create(ID2D1DeviceContext* context) const = 0;

	virtual uint64_t hash() const = 0;
};
};	// namespace oreik
