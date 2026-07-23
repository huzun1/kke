#pragma once

#include <array>
#include <span>

#include "kke/engine/Sources.hh"
#include "kke/engine/d2d/d2d1_headers.hh"

namespace kke {
class EffectClipTransformer {
	static constexpr float axisAlignedEpsilon = 0.0001f;
	static constexpr int ellipseSegmentCount = 24;

  public:
	static EffectClipSource
	transform(EffectClipSource const& clip, D2D1_MATRIX_3X2_F const& matrix);

  private:
	static Geometry transformGeometry(Geometry const& geometry, D2D1_MATRIX_3X2_F const& matrix);

	static Geometry transformRect(Rect const& rect, D2D1_MATRIX_3X2_F const& matrix);

	static Geometry
	transformRoundedRect(RoundedRect const& roundedRect, D2D1_MATRIX_3X2_F const& matrix);

	static Geometry transformEllipse(Ellipse const& ellipse, D2D1_MATRIX_3X2_F const& matrix);

	static Point transformPoint(Point const& point, D2D1_MATRIX_3X2_F const& matrix);

	static bool isAxisAligned(D2D1_MATRIX_3X2_F const& matrix);

	static Rect boundingRect(std::span<Point const> points);

	static std::array<Point, 4> rectCorners(Rect const& rect);
};
} // namespace kke
