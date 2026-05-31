#include <variant>

#include "GeometryFactory.hh"

using namespace kke;
using namespace Microsoft::WRL;

ComPtr<ID2D1Geometry> GeometryFactory::create(D2dContext const& context, Geometry const& geometry) {
	return std::visit([&](auto const& geo) { return create(context, geo); }, geometry);
}

ComPtr<ID2D1Geometry>
GeometryFactory::create(D2dContext const& context, GeometryCompose const& compose) {
	std::vector<ComPtr<ID2D1Geometry>> geometries;
	for (const auto& geo : compose.getGeometries()) {
		ComPtr<ID2D1Geometry> createdGeo = create(context, geo);
		if (!createdGeo) {
			return nullptr;
		}
		geometries.push_back(createdGeo);
	}

	if (geometries.empty()) {
		return nullptr;
	}

	if (geometries.size() == 1) {
		return geometries.front();
	}

	ComPtr<ID2D1GeometryGroup> geometryGroup;
	HRESULT result = context.getFactory()->CreateGeometryGroup(
		D2D1_FILL_MODE_WINDING,
		reinterpret_cast<ID2D1Geometry**>(geometries.data()),
		static_cast<UINT32>(geometries.size()),
		&geometryGroup
	);

	if (FAILED(result)) {
		return nullptr;
	}

	return geometryGroup;
}

ComPtr<ID2D1Geometry> GeometryFactory::create(D2dContext const& context, Triangle const& triangle) {
	Point points[] = {triangle.a, triangle.b, triangle.c};
	return createPathGeometry(context, points);
}

ComPtr<ID2D1Geometry> GeometryFactory::create(D2dContext const& context, Rect const& rect) {
	ComPtr<ID2D1RectangleGeometry> rectGeometry;
	D2D1_RECT_F d2dRect = D2D1::RectF(rect.min.x, rect.min.y, rect.max.x, rect.max.y);

	HRESULT result = context.getFactory()->CreateRectangleGeometry(d2dRect, &rectGeometry);
	if (FAILED(result)) {
		return nullptr;
	}

	return rectGeometry;
}

ComPtr<ID2D1Geometry>
GeometryFactory::create(D2dContext const& context, RoundedRect const& roundedRect) {
	ComPtr<ID2D1RoundedRectangleGeometry> roundedRectGeometry;
	D2D1_ROUNDED_RECT d2dRoundedRect = D2D1::RoundedRect(
		D2D1::RectF(roundedRect.min.x, roundedRect.min.y, roundedRect.max.x, roundedRect.max.y),
		roundedRect.rounding,
		roundedRect.rounding
	);

	HRESULT result =
		context.getFactory()->CreateRoundedRectangleGeometry(d2dRoundedRect, &roundedRectGeometry);
	if (FAILED(result)) {
		return nullptr;
	}

	return roundedRectGeometry;
}

ComPtr<ID2D1Geometry> GeometryFactory::create(D2dContext const& context, Ellipse const& ellipse) {
	ComPtr<ID2D1EllipseGeometry> ellipseGeometry;
	D2D1_ELLIPSE d2dEllipse =
		D2D1::Ellipse(pointToD2d(ellipse.point), ellipse.radius, ellipse.radius);

	HRESULT result = context.getFactory()->CreateEllipseGeometry(d2dEllipse, &ellipseGeometry);
	if (FAILED(result)) {
		return nullptr;
	}

	return ellipseGeometry;
}

ComPtr<ID2D1Geometry> GeometryFactory::create(D2dContext const& context, Polygon const& polygon) {
	return createPathGeometry(context, polygon.getPoints());
}

D2D1_POINT_2F GeometryFactory::pointToD2d(Point const& point) {
	return {point.x, point.y};
}

ComPtr<ID2D1Geometry>
GeometryFactory::createPathGeometry(D2dContext const& context, std::span<Point const> points) {
	ComPtr<ID2D1PathGeometry> pathGeometry;
	HRESULT result = context.getFactory()->CreatePathGeometry(&pathGeometry);
	if (FAILED(result)) {
		return nullptr;
	}

	ComPtr<ID2D1GeometrySink> sink;
	result = pathGeometry->Open(&sink);
	if (FAILED(result)) {
		return nullptr;
	}

	if (!points.empty()) {
		sink->BeginFigure(pointToD2d(points.front()), D2D1_FIGURE_BEGIN_FILLED);
		for (auto it = points.begin() + 1; it != points.end(); ++it) {
			sink->AddLine(pointToD2d(*it));
		}
		sink->EndFigure(D2D1_FIGURE_END_CLOSED);
	}

	result = sink->Close();
	if (FAILED(result)) {
		return nullptr;
	}

	return pathGeometry;
}
