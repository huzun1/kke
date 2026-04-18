#pragma once

#include "kke/geometry/primitives/Point.hh"

namespace kke {
struct ScaleTransform {
	kke::Point center;
	kke::Scale scale;
};
};	// namespace kke
