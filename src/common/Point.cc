#include <oreik/common/Point.hpp>

#include "oreik/internal/Hasher.hpp"

D2D1_POINT_2F oreik::Point2f::point2f() const {
	return {x, y};
}

uint64_t oreik::Point2f::hash() const {
	oreik::Hasher hash;
	hash.combine(x);
	hash.combine(y);
	return hash.get();
}
