#pragma once

#include <cstdint>

namespace oreik {
enum class GeometryType : uint32_t {
	ELLIPSE = 0,
	RECT = 1,
	ROUNDED_RECT = 2
};

class Geometry {
public:
	virtual ~Geometry() = default;

	virtual uint64_t hash(bool positionDependent) const = 0;
};
};	// namespace oreik
