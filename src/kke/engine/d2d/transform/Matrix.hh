#pragma once

#include <memory>
#include <vector>

#include "Transform.hh"
#include "kke/common/Point.hh"
#include "kke/common/Scale.hh"

namespace kke {
class Matrix {
	std::vector<std::shared_ptr<Transform>> transforms;

public:
    void pushTransform(Transform const& transform);

	void pushScale();

	void pushRotate(kke::Point const& center, float angle);

	void pop();

	D2D1::Matrix3x2F build();
};
};	// namespace kke
