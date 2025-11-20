#include <kke/common/Point.hh>

#include "kke/internal/Hasher.hh"

D2D1_POINT_2F kke::Point2f::point2f() const {
	return {x, y};
}

uint64_t kke::Point2f::hash() const {
	kke::Hasher hash;
	hash.combine(x);
	hash.combine(y);
	return hash.get();
}
