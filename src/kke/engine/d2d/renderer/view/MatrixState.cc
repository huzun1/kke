#include "MatrixState.hh"

using namespace kke;

void MatrixState::pushTransform(D2dEngineContext const& context, TransformSource const& transform) {
	std::visit([&](auto&& arg) {
		pushTransform(context, arg);
	}, transform);
}

void MatrixState::popTransform(D2dEngineContext const& context) {
	matrix.popTransform();
	applyTransform(context);
}

void MatrixState::pushTransform(D2dEngineContext const& context, Translation const& translation) {
	matrix.pushTransform(translation);
	applyTransform(context);
}

void MatrixState::pushTransform(D2dEngineContext const& context, Scaling const& scale) {
	matrix.pushTransform(scale);
	applyTransform(context);
}

void MatrixState::pushTransform(D2dEngineContext const& context, Rotation const& rotate) {
	matrix.pushTransform(rotate);
	applyTransform(context);
}

void MatrixState::applyTransform(D2dEngineContext const& context) {
	D2dContext* d2dContext = context.getD2dContext();
	d2dContext->getDeviceContext()->SetTransform(matrix.build());
}
