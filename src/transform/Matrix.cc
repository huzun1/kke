#include <kke/transform/Matrix.hh>

#include <stdexcept>

#include "kke/transform/impl/RotateTransform.hh"
#include "kke/transform/impl/ScaleTransform.hh"
#include "kke/transform/impl/TranslateTransfrom.hh"

void kke::Matrix::pushTranslate(kke::Point2f const& offset) {
	transforms.push_back(std::make_shared<kke::TranslateTransform>(offset));
}

void kke::Matrix::pushScale(kke::Point2f const& center, kke::Scale2f const& scale) {
	transforms.push_back(std::make_shared<kke::ScaleTransform>(center, scale));
}

void kke::Matrix::pushRotate(kke::Point2f const& center, float angle) {
	transforms.push_back(std::make_shared<kke::RotateTransform>(center, angle));
}

void kke::Matrix::pop() {
	if (transforms.empty()) {
		throw std::runtime_error("Cannot pop an empty transform stack.");
	}
	transforms.pop_back();
}

D2D1::Matrix3x2F kke::Matrix::build() const {
	D2D1::Matrix3x2F multiplied = D2D1::Matrix3x2F::Identity();
	for (const auto& transform : transforms) {
		transform->transform(multiplied);
	}
	return multiplied;
}
