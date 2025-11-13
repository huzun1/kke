#pragma once

#include <memory>
#include <vector>

#include "Transform.hpp"
#include "kke/common/Point.hpp"
#include "kke/common/Scale.hpp"

namespace kke {
class Matrix {
	std::vector<std::shared_ptr<Transform>> transforms;

public:
	void pushScale(kke::Point2f const& center, kke::Scale2f const& scale);

	void pushRotate(kke::Point2f const& center, float angle);

	void pop();

	D2D1::Matrix3x2F build();
};
};	// namespace kke
