#pragma once

#include <cstddef>
#include <vector>

#include "kke/appearance/transform/AffineTransform.hh"
#include "kke/appearance/transform/Rotation.hh"
#include "kke/appearance/transform/Scaling.hh"
#include "kke/appearance/transform/Translation.hh"
#include "kke/engine/d2d/d2d1_headers.hh"

namespace kke {
class Matrix {
	std::vector<D2D1::Matrix3x2F> transforms;

  public:
	void pushTransform(Translation const& transform);

	void pushTransform(Scaling const& scale);

	void pushTransform(Rotation const& rotate);

	void pushTransform(AffineTransform const& transform);

	void popTransform();

	size_t depth() const;

	D2D1::Matrix3x2F build(size_t baseDepth = 0) const;

  private:
	static D2D1::Matrix3x2F toMatrix(Translation const& translation);

	static D2D1::Matrix3x2F toMatrix(Scaling const& scale);

	static D2D1::Matrix3x2F toMatrix(Rotation const& rotate);

	static D2D1::Matrix3x2F toMatrix(AffineTransform const& transform);
};
}; // namespace kke
