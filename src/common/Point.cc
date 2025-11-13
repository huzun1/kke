#include <kke/common/Point.hpp>

#include "kke/internal/Hasher.hpp"

D2D1_POINT_2F kke::Point2f::point2f() const {
	return {x, y};
}

uint64_t kke::Point2f::hash() const {
	kke::Hasher hash;
	hash.combine(x);
	hash.combine(y);
	return hash.get();
}
