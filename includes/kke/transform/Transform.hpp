#pragma once

#include <d2d1.h>

namespace kke {
class Transform {
public:
	virtual ~Transform() = default;

	virtual void transform(D2D1::Matrix3x2F& matrix) = 0;
};
};	// namespace kke
