#include "Matrix.hh"

#include "kke/appearance/transform/Translation.hh"
#include "kke/engine/d2d/transform/impl/RotateManipulator.hh"
#include "kke/engine/d2d/transform/impl/ScaleManipulator.hh"
#include "kke/engine/d2d/transform/impl/TranslateManipulator.hh"

using namespace kke;

void Matrix::pushTransform(Translation const& translation) {
	manipulators.push_back(std::make_shared<kke::TranslateManipulator>(translation));
}

void Matrix::pushTransform(Scaling const& scale) {
	manipulators.push_back(std::make_shared<kke::ScaleManipulator>(scale));
}

void Matrix::pushTransform(Rotation const& rotate) {
	manipulators.push_back(std::make_shared<kke::RotateManipulator>(rotate));
}

D2D1::Matrix3x2F kke::Matrix::build() {
	D2D1::Matrix3x2F multiplied = D2D1::Matrix3x2F::Identity();
	for (auto& manipulator : manipulators) {
		manipulator->transform(multiplied);
	}
	return multiplied;
}
