#include "MatrixState.hh"

using namespace kke;

void MatrixState::pushTransform(D2dContext const& context, TransformSource const& transform) {
	std::visit([&](auto&& arg) {
		pushTransform(context, arg);
	}, transform);
}

void MatrixState::popTransform(D2dContext const& context) {
	matrix.popTransform();
	applyTransform(context);
}

void MatrixState::pushTransform(D2dContext const& context, Translation const& translation) {
	matrix.pushTransform(translation);
	applyTransform(context);
}

void MatrixState::pushTransform(D2dContext const& context, Scaling const& scale) {
	matrix.pushTransform(scale);
	applyTransform(context);
}

void MatrixState::pushTransform(D2dContext const& context, Rotation const& rotate) {
	matrix.pushTransform(rotate);
	applyTransform(context);
}

void MatrixState::applyTransform(D2dContext const& context) {
	context.getDeviceContext()->SetTransform(matrix.build());
}
