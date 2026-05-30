#include "RotateManipulator.hh"

kke::RotateManipulator::RotateManipulator(Rotation const& rotation)
	: rotation(rotation) {
}

void kke::RotateManipulator::transform(D2D1::Matrix3x2F& matrix) {
	matrix = matrix * D2D1::Matrix3x2F::Rotation(rotation.angle, {rotation.center.x, rotation.center.y});
}
