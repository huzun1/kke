#pragma once

#include "kke/appearance/transform/Rotation.hh"
#include "kke/appearance/transform/Scaling.hh"
#include "kke/appearance/transform/Translation.hh"
#include "kke/engine/Sources.hh"
#include "kke/engine/d2d/D2dContext.hh"
#include "kke/engine/d2d/transform/Matrix.hh"

namespace kke {
class MatrixState {
	Matrix matrix;

public:
	void pushTransform(D2dContext const& context, TransformSource const& transform);

	void popTransform(D2dContext const& context);

private:
	void pushTransform(D2dContext const& context, Translation const& translation);

	void pushTransform(D2dContext const& context, Scaling const& scale);

	void pushTransform(D2dContext const& context, Rotation const& rotate);

	void applyTransform(D2dContext const& context);
};
};	// namespace kke
