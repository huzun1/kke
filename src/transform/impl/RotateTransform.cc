#include <oreik/transform/impl/RotateTransform.hpp>

oreik::RotateTransform::RotateTransform(oreik::Point2f const& center, float angle)
	: center(center),
	  angle(angle) {
}

void oreik::RotateTransform::transform(D2D1::Matrix3x2F& matrix) {
	matrix = matrix * D2D1::Matrix3x2F::Rotation(angle, {center.x, center.y});
}
