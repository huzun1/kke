#include <kke/transform/Matrix.hh>

#include "kke/transform/impl/RotateTransform.hh"
#include "kke/transform/impl/ScaleTransform.hh"
#include "kke/transform/impl/TranslateTransfrom.hh"

void kke::Matrix::pushTranslate(kke::Point const& offset) {
	transforms.push_back(std::make_shared<kke::TranslateTransform>(offset));
}

void kke::Matrix::pushScale(kke::Point const& center, kke::Scale const& scale) {
	transforms.push_back(std::make_shared<kke::ScaleTransform>(center, scale));
}

void kke::Matrix::pushRotate(kke::Point const& center, float angle) {
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
