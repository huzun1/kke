#include <kke/transform/impl/ScaleTransform.hpp>

kke::ScaleTransform::ScaleTransform(kke::Point2f center, kke::Scale2f scale)
	: center(center),
	  scale(scale) {
}

void kke::ScaleTransform::transform(D2D1::Matrix3x2F& matrix) {
	matrix = matrix * D2D1::Matrix3x2F::Scale({scale.x, scale.y}, {center.x, center.y});
}
