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
	void pushTranslate(kke::Point2f const& offset);

	void pushScale(kke::Point2f const& center, kke::Scale2f const& scale);

	void pushRotate(kke::Point2f const& center, float angle);

	void pop();

	D2D1::Matrix3x2F build() const;
};
};	// namespace kke
