#include <kke/transform/impl/TranslateTransfrom.hh>

kke::TranslateTransform::TranslateTransform(kke::Point offset)
	: offset(offset) {
}

void kke::TranslateTransform::transform(D2D1::Matrix3x2F& matrix) {
	matrix = matrix * D2D1::Matrix3x2F::Translation(offset.x, offset.y);
}
