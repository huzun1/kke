#pragma once

#include <cstdint>

namespace oreik {
class Geometry {
public:
	virtual ~Geometry() = default;

	uint64_t hash() const;
};
};	// namespace oreik
