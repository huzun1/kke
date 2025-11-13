#include <kke/transform/Matrix.hpp>

#include "kke/transform/impl/RotateTransform.hpp"
#include "kke/transform/impl/ScaleTransform.hpp"

void kke::Matrix::pushScale(kke::Point2f const& center, kke::Scale2f const& scale) {
	transforms.push_back(std::make_shared<kke::ScaleTransform>(center, scale));
}

void kke::Matrix::pushRotate(kke::Point2f const& center, float angle) {
	transforms.push_back(std::make_shared<kke::RotateTransform>(center, angle));
}

void kke::Matrix::pop() {
	transforms.pop_back();
}

D2D1::Matrix3x2F kke::Matrix::build() {
	D2D1::Matrix3x2F multiplied = D2D1::Matrix3x2F::Identity();
	for (const auto& transform : transforms) {
		transform->transform(multiplied);
	}
	return multiplied;
}
