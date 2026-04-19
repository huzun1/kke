#include "ScaleManipulator.hh"

kke::ScaleManipulator::ScaleManipulator(Scaling const& scale)
	: center(scale.center),
	  scale(scale.scale) {
}

void kke::ScaleManipulator::transform(D2D1::Matrix3x2F& matrix) {
	matrix = matrix * D2D1::Matrix3x2F::Scale({scale.x, scale.y}, {center.x, center.y});
}
