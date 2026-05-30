#include "TranslateManipulator.hh"

using namespace kke;

TranslateManipulator::TranslateManipulator(Translation const& translation)
	: translation(translation) {
}

void TranslateManipulator::transform(D2D1::Matrix3x2F& matrix) {
	matrix = matrix * D2D1::Matrix3x2F::Translation(translation.offset.x, translation.offset.y);
}
