#include "InvertedGeometryFactory.hh"

#include "GeometryInverter.hh"
#include "kke/engine/d2d/resource/geometry/hash/GeometryHasher.hh"
#include "kke/utils/DebugLog.hh"
#include "kke/utils/Hasher.hh"

using namespace kke;
using namespace Microsoft::WRL;

ComPtr<ID2D1Geometry> InvertedGeometryFactory::createInvertedGeometry(
	D2dContext const& context,
	GeometryProvider& geometryProvider,
	Microsoft::WRL::ComPtr<ID2D1RectangleGeometry> viewportGeometry,
	Geometry const& geometry
) {
	ComPtr<ID2D1Geometry> maskGeometry = geometryProvider.get(context, geometry);
	if (!maskGeometry) {
		kke::debug::log("[kke][InvertedGeometryFactory] mask geometry is null");
		return nullptr;
	}

	return createInvertedGeometry(context, viewportGeometry, maskGeometry.Get());
}

ComPtr<ID2D1Geometry> InvertedGeometryFactory::createInvertedGeometry(
	D2dContext const& context,
	GeometryProvider& geometryProvider,
	Microsoft::WRL::ComPtr<ID2D1RectangleGeometry> viewportGeometry,
	GeometryCompose const& compose
) {
	ComPtr<ID2D1Geometry> maskGeometry = geometryProvider.get(context, compose);
	if (!maskGeometry) {
		kke::debug::log("[kke][InvertedGeometryFactory] mask compose geometry is null");
		return nullptr;
	}

	return createInvertedGeometry(context, viewportGeometry, maskGeometry.Get());
}

ComPtr<ID2D1Geometry> InvertedGeometryFactory::createInvertedGeometry(
	D2dContext const& context,
	Microsoft::WRL::ComPtr<ID2D1RectangleGeometry> viewportGeometry,
	ID2D1Geometry* maskGeometry
) {
	if (!viewportGeometry) {
		kke::debug::log("[kke][InvertedGeometryFactory] viewport geometry is null");
	}
	if (!maskGeometry) {
		kke::debug::log("[kke][InvertedGeometryFactory] mask geometry pointer is null");
	}

	return GeometryInverter::create(context, viewportGeometry.Get(), maskGeometry);
}
