#include "FaceRenderer.hh"

#include <variant>

using namespace kke;
using Microsoft::WRL::ComPtr;

void FaceRenderer::fill(
	D2dEngineContext const& context,
	FillSource const& source,
	Brush const& brush) {
	ComPtr<ID2D1Brush> d2dBrush = context.getResourceProviders()->getBrushProvider()->get(*context.getD2dContext(), brush);
	if (!d2dBrush) {
		return;
	}

	std::visit([&](auto const& sourceVariant) {
		fill(context, sourceVariant, d2dBrush.Get());
	}, source);
}

void FaceRenderer::fill(
	D2dEngineContext const& context,
	Geometry const& geometry,
	ID2D1Brush* brush) {
	ComPtr<ID2D1Geometry> d2dGeometry = context.getResourceProviders()->getGeometryProvider()->get(*context.getD2dContext(), geometry);
	if (!d2dGeometry) {
		return;
	}

	context.getD2dContext()->getDeviceContext()->FillGeometry(d2dGeometry.Get(), brush);
}

void FaceRenderer::fill(
	D2dEngineContext const& context,
	GeometryCompose const& compose,
	ID2D1Brush* brush) {
	ComPtr<ID2D1Geometry> d2dGeometry = context.getResourceProviders()->getGeometryProvider()->get(*context.getD2dContext(), compose);
	if (!d2dGeometry) {
		return;
	}

	context.getD2dContext()->getDeviceContext()->FillGeometry(d2dGeometry.Get(), brush);
}

void FaceRenderer::fill(
	D2dEngineContext const& context,
	Text const& text,
	ID2D1Brush* brush) {
	(void)context;
	(void)text;
	(void)brush;
}
