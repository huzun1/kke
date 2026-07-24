#include "Matrix.hh"

#include "kke/appearance/transform/Translation.hh"

using namespace kke;

void Matrix::pushTransform(Translation const& translation) {
	transforms.push_back(toMatrix(translation));
}

void Matrix::pushTransform(Scaling const& scale) {
	transforms.push_back(toMatrix(scale));
}

void Matrix::pushTransform(Rotation const& rotate) {
	transforms.push_back(toMatrix(rotate));
}

void Matrix::pushTransform(AffineTransform const& transform) {
	transforms.push_back(toMatrix(transform));
}

void Matrix::popTransform() {
	if (transforms.empty()) {
		return;
	}

	transforms.pop_back();
}

size_t Matrix::depth() const {
	return transforms.size();
}

D2D1::Matrix3x2F Matrix::build(size_t baseDepth) const {
	D2D1::Matrix3x2F multiplied = D2D1::Matrix3x2F::Identity();
	for (size_t transformIndex = baseDepth; transformIndex < transforms.size(); ++transformIndex) {
		multiplied = transforms[transformIndex] * multiplied;
	}
	return multiplied;
}

D2D1::Matrix3x2F Matrix::toMatrix(Translation const& translation) {
	return D2D1::Matrix3x2F::Translation(translation.offset.x, translation.offset.y);
}

D2D1::Matrix3x2F Matrix::toMatrix(Scaling const& scale) {
	return D2D1::Matrix3x2F::Scale(
		{scale.scale.x, scale.scale.y},
		{scale.center.x, scale.center.y}
	);
}

D2D1::Matrix3x2F Matrix::toMatrix(Rotation const& rotate) {
	return D2D1::Matrix3x2F::Rotation(rotate.angle, {rotate.center.x, rotate.center.y});
}

D2D1::Matrix3x2F Matrix::toMatrix(AffineTransform const& transform) {
	return D2D1::Matrix3x2F(
		transform.xx,
		transform.yx,
		transform.xy,
		transform.yy,
		transform.tx,
		transform.ty
	);
}
