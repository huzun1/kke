#pragma once

#include <span>

#include "kke/engine/d2d/context/D2dContext.hh"
#include "kke/engine/d2d/d2d1_headers.hh"
#include "kke/geometry/Geometry.hh"

namespace kke {
class GeometryFactory {
public:
	static Microsoft::WRL::ComPtr<ID2D1Geometry> create(D2dContext const& context, Geometry const& geometry);

private:
	static Microsoft::WRL::ComPtr<ID2D1Geometry> create(D2dContext const& context, Triangle const& triangle);

	static Microsoft::WRL::ComPtr<ID2D1Geometry> create(D2dContext const& context, Rect const& rect);

	static Microsoft::WRL::ComPtr<ID2D1Geometry> create(D2dContext const& context, RoundedRect const& roundedRect);

	static Microsoft::WRL::ComPtr<ID2D1Geometry> create(D2dContext const& context, Ellipse const& ellipse);

	static Microsoft::WRL::ComPtr<ID2D1Geometry> create(D2dContext const& context, Polygon const& polygon);

	static D2D1_POINT_2F pointToD2d(Point const& point);

	static Microsoft::WRL::ComPtr<ID2D1Geometry> createPathGeometry(D2dContext const& context, std::span<Point const> points);
};
}	// namespace kke
