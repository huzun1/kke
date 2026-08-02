#pragma once

#include <cstddef>
#include <vector>

#include "kke/engine/Sources.hh"
#include "kke/engine/d2d/context/D2dEngineContext.hh"
#include "kke/engine/d2d/transform/Matrix.hh"

namespace kke {
class MatrixState {
	Matrix matrix;
	std::vector<size_t> canvasBaseDepths;

  public:
	void pushTransform(D2dEngineContext const& context, TransformSource const& transform);

	void popTransform(D2dEngineContext const& context);

	void beginCanvas(D2dEngineContext const& context);

	void endCanvas(D2dEngineContext const& context);

  private:
	void pushTransform(D2dEngineContext const& context, Translation const& translation);

	void pushTransform(D2dEngineContext const& context, Scaling const& scale);

	void pushTransform(D2dEngineContext const& context, Rotation const& rotate);

	void pushTransform(D2dEngineContext const& context, AffineTransform const& transform);

	void applyTransform(D2dEngineContext const& context);

	size_t activeBaseDepth() const;
};
}; // namespace kke
