#include "InvertedGeometryProvider.hh"

#include "InvertedGeometryFactory.hh"
#include "kke/engine/d2d/resource/geometry/hash/GeometryHashMode.hh"
#include "kke/engine/d2d/resource/geometry/hash/GeometryHasher.hh"
#include "kke/utils/Hasher.hh"

using namespace kke;
using namespace Microsoft::WRL;

void InvertedGeometryProvider::syncViewportSize(D2dContext const& context) {
	bool isViewportResized = this->isViewportResized(context);
	if (isViewportResized) {
		geometries.clear();
		viewportSize = context.getDeviceContext()->GetSize();
	}

	viewportGeometryCache.syncViewportSize(context);
}

ComPtr<ID2D1Geometry> InvertedGeometryProvider::getInvertedGeometry(
	D2dContext const& context,
	GeometryProvider& geometryProvider,
	Geometry const& geometry) {
	uint64_t key = createKey(context, geometry);

	ComPtr<ID2D1Geometry> cachedGeometry = geometries.get(key);
	if (cachedGeometry) {
		return cachedGeometry;
	}

	ComPtr<ID2D1Geometry> invertedGeometry = createInvertedGeometry(context, geometryProvider, geometry);
	if (!invertedGeometry) {
		return nullptr;
	}

	geometries.put(key, invertedGeometry);
	return invertedGeometry;
}

ComPtr<ID2D1Geometry> InvertedGeometryProvider::getInvertedGeometry(
	D2dContext const& context,
	GeometryProvider& geometryProvider,
	GeometryCompose const& compose) {
	uint64_t key = createKey(context, compose);

	ComPtr<ID2D1Geometry> cachedGeometry = geometries.get(key);
	if (cachedGeometry) {
		return cachedGeometry;
	}

	ComPtr<ID2D1Geometry> invertedGeometry = createInvertedGeometry(context, geometryProvider, compose);
	if (!invertedGeometry) {
		return nullptr;
	}

	geometries.put(key, invertedGeometry);
	return invertedGeometry;
}

bool InvertedGeometryProvider::isViewportResized(D2dContext const& context) const {
	D2D1_SIZE_F currentViewportSize = context.getDeviceContext()->GetSize();
	return viewportSize.width != currentViewportSize.width
		|| viewportSize.height != currentViewportSize.height;
}

uint64_t InvertedGeometryProvider::createKey(D2dContext const& context, Geometry const& geometry) const {
	D2D1_SIZE_F viewportSize = context.getDeviceContext()->GetSize();

	Hasher hasher;
	hasher.combine(viewportSize.width);
	hasher.combine(viewportSize.height);
	hasher.combine(GeometryHasher::hash(geometry, GeometryHashMode::PositionDependent));
	return hasher.get();
}

uint64_t InvertedGeometryProvider::createKey(D2dContext const& context, GeometryCompose const& compose) const {
	D2D1_SIZE_F viewportSize = context.getDeviceContext()->GetSize();

	Hasher hasher;
	hasher.combine(viewportSize.width);
	hasher.combine(viewportSize.height);
	hasher.combine(GeometryHasher::hash(compose, GeometryHashMode::PositionDependent));
	return hasher.get();
}

ComPtr<ID2D1Geometry> InvertedGeometryProvider::createInvertedGeometry(
	D2dContext const& context,
	GeometryProvider& geometryProvider,
	Geometry const& geometry) {
	ComPtr<ID2D1Geometry> maskGeometry = geometryProvider.get(context, geometry);
	if (!maskGeometry) {
		return nullptr;
	}

	return createInvertedGeometry(context, maskGeometry.Get());
}

ComPtr<ID2D1Geometry> InvertedGeometryProvider::createInvertedGeometry(
	D2dContext const& context,
	GeometryProvider& geometryProvider,
	GeometryCompose const& compose) {
	ComPtr<ID2D1Geometry> maskGeometry = geometryProvider.get(context, compose);
	if (!maskGeometry) {
		return nullptr;
	}

	return createInvertedGeometry(context, maskGeometry.Get());
}

ComPtr<ID2D1Geometry> InvertedGeometryProvider::createInvertedGeometry(
	D2dContext const& context,
	ID2D1Geometry* maskGeometry) {
	ComPtr<ID2D1RectangleGeometry> viewportGeometry = viewportGeometryCache.get(context);
	if (!viewportGeometry) {
		return nullptr;
	}

	return InvertedGeometryFactory::create(context, viewportGeometry.Get(), maskGeometry);
}
