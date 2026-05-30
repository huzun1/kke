#pragma once

#include "kke/engine/d2d/d2d1_headers.hh"

namespace kke {
class Manipulator {
public:
	virtual ~Manipulator() = default;

	virtual void transform(D2D1::Matrix3x2F& matrix) = 0;
};
};	// namespace kke
