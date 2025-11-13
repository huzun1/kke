#include <kke/transform/impl/RotateTransform.hpp>

kke::RotateTransform::RotateTransform(kke::Point2f const& center, float angle)
	: center(center),
	  angle(angle) {
}

void kke::RotateTransform::transform(D2D1::Matrix3x2F& matrix) {
	matrix = matrix * D2D1::Matrix3x2F::Rotation(angle, {center.x, center.y});
}
