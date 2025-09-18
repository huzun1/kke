#include <oreik/transform/Matrix.hpp>

#include "oreik/transform/impl/RotateTransform.hpp"
#include "oreik/transform/impl/ScaleTransform.hpp"

void oreik::Matrix::pushScale(oreik::Point2f const& center, oreik::Scale2f const& scale) {
	transforms.push_back(std::make_shared<oreik::ScaleTransform>(center, scale));
}

void oreik::Matrix::pushRotate(oreik::Point2f const& center, float angle) {
	transforms.push_back(std::make_shared<oreik::RotateTransform>(center, angle));
}

void oreik::Matrix::pop() {
	transforms.pop_back();
}

D2D1::Matrix3x2F oreik::Matrix::build() {
	D2D1::Matrix3x2F multiplied = D2D1::Matrix3x2F::Identity();
	for (const auto& transform : transforms) {
		transform->transform(multiplied);
	}
	return multiplied;
}
