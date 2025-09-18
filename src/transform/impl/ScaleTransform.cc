#include <oreik/transform/impl/ScaleTransform.hpp>

oreik::ScaleTransform::ScaleTransform(oreik::Point2f center, oreik::Scale2f scale)
	: center(center),
	  scale(scale) {
}

void oreik::ScaleTransform::transform(D2D1::Matrix3x2F& matrix) {
	matrix = matrix * D2D1::Matrix3x2F::Scale({scale.x, scale.y}, {center.x, center.y});
}
