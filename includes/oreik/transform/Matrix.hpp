#pragma once

#include <memory>
#include <vector>

#include "Transform.hpp"
#include "oreik/common/Point.hpp"
#include "oreik/common/Scale.hpp"

namespace oreik {
class Matrix {
	std::vector<std::shared_ptr<Transform>> transforms;

public:
	void pushScale(oreik::Point2f const& center, oreik::Scale2f const& scale);

	void pushRotate(oreik::Point2f const& center, float angle);

	void pop();

	D2D1::Matrix3x2F build();
};
};	// namespace oreik
