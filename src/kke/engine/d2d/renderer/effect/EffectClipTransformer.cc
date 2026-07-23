#include "EffectClipTransformer.hh"

#include <algorithm>
#include <cmath>
#include <type_traits>
#include <vector>

using namespace kke;

EffectClipSource
EffectClipTransformer::transform(EffectClipSource const& clip, D2D1_MATRIX_3X2_F const& matrix) {
	return std::visit(
		[&](auto const& clipVariant) -> EffectClipSource {
			using ClipType = std::decay_t<decltype(clipVariant)>;
			if constexpr (std::is_same_v<ClipType, Geometry>) {
				return transformGeometry(clipVariant, matrix);
			} else {
				GeometryCompose transformedCompose;
				for (Geometry const& geometry : clipVariant.getGeometries()) {
					transformedCompose.add(transformGeometry(geometry, matrix));
				}
				return transformedCompose;
			}
		},
		clip
	);
}

Geometry EffectClipTransformer::transformGeometry(
	Geometry const& geometry, D2D1_MATRIX_3X2_F const& matrix
) {
	return std::visit(
		[&](auto const& geometryVariant) -> Geometry {
			using GeometryType = std::decay_t<decltype(geometryVariant)>;
			if constexpr (std::is_same_v<GeometryType, Rect>) {
				return transformRect(geometryVariant, matrix);
			} else if constexpr (std::is_same_v<GeometryType, RoundedRect>) {
				return transformRoundedRect(geometryVariant, matrix);
			} else if constexpr (std::is_same_v<GeometryType, Triangle>) {
				return Triangle(
					transformPoint(geometryVariant.a, matrix),
					transformPoint(geometryVariant.b, matrix),
					transformPoint(geometryVariant.c, matrix)
				);
			} else if constexpr (std::is_same_v<GeometryType, Ellipse>) {
				return transformEllipse(geometryVariant, matrix);
			} else {
				std::vector<Point> points;
				points.reserve(geometryVariant.getPoints().size());
				for (Point const& point : geometryVariant.getPoints()) {
					points.push_back(transformPoint(point, matrix));
				}
				return Polygon(std::move(points));
			}
		},
		geometry
	);
}

Geometry EffectClipTransformer::transformRect(Rect const& rect, D2D1_MATRIX_3X2_F const& matrix) {
	std::array<Point, 4> corners = rectCorners(rect);
	for (Point& point : corners) {
		point = transformPoint(point, matrix);
	}
	if (isAxisAligned(matrix)) {
		return boundingRect(corners);
	}
	return Polygon(std::vector<Point>(corners.begin(), corners.end()));
}

Geometry EffectClipTransformer::transformRoundedRect(
	RoundedRect const& roundedRect, D2D1_MATRIX_3X2_F const& matrix
) {
	std::array<Point, 4> corners = rectCorners(roundedRect);
	for (Point& point : corners) {
		point = transformPoint(point, matrix);
	}
	if (!isAxisAligned(matrix)) {
		return Polygon(std::vector<Point>(corners.begin(), corners.end()));
	}

	float scale = std::max(std::abs(matrix._11), std::abs(matrix._22));
	return RoundedRect(boundingRect(corners), roundedRect.rounding * scale);
}

Geometry
EffectClipTransformer::transformEllipse(Ellipse const& ellipse, D2D1_MATRIX_3X2_F const& matrix) {
	std::vector<Point> points;
	points.reserve(ellipseSegmentCount);
	for (int index = 0; index < ellipseSegmentCount; ++index) {
		float radians = static_cast<float>(index) / static_cast<float>(ellipseSegmentCount) * 2.0f *
						3.14159265358979323846f;
		Point point{
			ellipse.point.x + std::cos(radians) * ellipse.radius,
			ellipse.point.y + std::sin(radians) * ellipse.radius,
		};
		points.push_back(transformPoint(point, matrix));
	}
	return Polygon(std::move(points));
}

Point EffectClipTransformer::transformPoint(Point const& point, D2D1_MATRIX_3X2_F const& matrix) {
	return {
		point.x * matrix._11 + point.y * matrix._21 + matrix._31,
		point.x * matrix._12 + point.y * matrix._22 + matrix._32,
	};
}

bool EffectClipTransformer::isAxisAligned(D2D1_MATRIX_3X2_F const& matrix) {
	return std::abs(matrix._12) <= axisAlignedEpsilon && std::abs(matrix._21) <= axisAlignedEpsilon;
}

Rect EffectClipTransformer::boundingRect(std::span<Point const> points) {
	float left = points.front().x;
	float top = points.front().y;
	float right = points.front().x;
	float bottom = points.front().y;
	for (Point const& point : points.subspan(1)) {
		left = std::min(left, point.x);
		top = std::min(top, point.y);
		right = std::max(right, point.x);
		bottom = std::max(bottom, point.y);
	}
	return {{left, top}, {right, bottom}};
}

std::array<Point, 4> EffectClipTransformer::rectCorners(Rect const& rect) {
	return {rect.min, Point{rect.max.x, rect.min.y}, rect.max, Point{rect.min.x, rect.max.y}};
}
