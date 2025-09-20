#pragma once

#include <cstdint>

namespace oreik {
class Geometry {
public:
	virtual ~Geometry() = default;

	virtual uint64_t hash() const = 0;
};
};	// namespace oreik
