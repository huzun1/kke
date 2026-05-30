#include "StrokeRenderer.hh"

#include <variant>

#include "kke/engine/d2d/context/D2dResourceProviders.hh"

using namespace kke;
using Microsoft::WRL::ComPtr;

void StrokeRenderer::draw(
	D2dEngineContext const& context,
	StrokeSource const& source,
	Brush const& brush,
	StrokeAppearance const& appearance) {
	ComPtr<ID2D1Brush> d2dBrush = context.getResourceProviders()->getBrushProvider()->get(*context.getD2dContext(), brush);
	if (!d2dBrush) {
		return;
	}

	std::visit([&](auto const& sourceVariant) {
		draw(context, sourceVariant, d2dBrush.Get(), appearance);
	}, source);
}

void StrokeRenderer::draw(
	D2dEngineContext const& context,
	Line const& line,
	ID2D1Brush* brush,
	StrokeAppearance const& appearance) {
	context.getD2dContext()->getDeviceContext()->DrawLine(
		{line.start.x, line.start.y},
		{line.end.x, line.end.y},
		brush,
		appearance.width);
}

void StrokeRenderer::draw(
	D2dEngineContext const& context,
	Geometry const& geometry,
	ID2D1Brush* brush,
	StrokeAppearance const& appearance) {
	ComPtr<ID2D1Geometry> d2dGeometry = context.getResourceProviders()->getGeometryProvider()->get(*context.getD2dContext(), geometry);
	if (!d2dGeometry) {
		return;
	}

	context.getD2dContext()->getDeviceContext()->DrawGeometry(d2dGeometry.Get(), brush, appearance.width);
}

void StrokeRenderer::draw(
	D2dEngineContext const& context,
	GeometryCompose const& compose,
	ID2D1Brush* brush,
	StrokeAppearance const& appearance) {
	ComPtr<ID2D1Geometry> d2dGeometry = context.getResourceProviders()->getGeometryProvider()->get(*context.getD2dContext(), compose);
	if (!d2dGeometry) {
		return;
	}

	context.getD2dContext()->getDeviceContext()->DrawGeometry(d2dGeometry.Get(), brush, appearance.width);
}
