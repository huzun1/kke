#include "InvertedGeometryProvider.hh"

#include "kke/engine/d2d/resource/geometry/factory/invert/InvertedGeometryFactory.hh"
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

ComPtr<ID2D1Geometry> InvertedGeometryProvider::get(
	D2dContext const& context,
	GeometryProvider& geometryProvider,
	Geometry const& geometry) {
	uint64_t key = createInvertedGeometryKey(context, geometry);

	ComPtr<ID2D1Geometry> cachedGeometry = geometries.get(key);
	if (cachedGeometry) {
		return cachedGeometry;
	}

	ComPtr<ID2D1Geometry> invertedGeometry = InvertedGeometryFactory::createInvertedGeometry(context, geometryProvider,
																							 viewportGeometryCache.get(context), geometry);
	if (!invertedGeometry) {
		return nullptr;
	}

	geometries.put(key, invertedGeometry);
	return invertedGeometry;
}

ComPtr<ID2D1Geometry> InvertedGeometryProvider::get(
	D2dContext const& context,
	GeometryProvider& geometryProvider,
	GeometryCompose const& compose) {
	uint64_t key = createInvertedGeometryKey(context, compose);

	ComPtr<ID2D1Geometry> cachedGeometry = geometries.get(key);
	if (cachedGeometry) {
		return cachedGeometry;
	}

	ComPtr<ID2D1Geometry> invertedGeometry = InvertedGeometryFactory::createInvertedGeometry(context, geometryProvider,
																							 viewportGeometryCache.get(context), compose);
	if (!invertedGeometry) {
		return nullptr;
	}

	geometries.put(key, invertedGeometry);
	return invertedGeometry;
}

uint64_t InvertedGeometryProvider::createInvertedGeometryKey(D2dContext const& context, Geometry const& geometry) {
	D2D1_SIZE_F viewportSize = context.getDeviceContext()->GetSize();

	Hasher hasher;
	hasher.combine(viewportSize.width);
	hasher.combine(viewportSize.height);
	hasher.combine(GeometryHasher::hash(geometry, GeometryHashMode::PositionDependent));
	return hasher.get();
}

uint64_t InvertedGeometryProvider::createInvertedGeometryKey(D2dContext const& context, GeometryCompose const& compose) {
	D2D1_SIZE_F viewportSize = context.getDeviceContext()->GetSize();

	Hasher hasher;
	hasher.combine(viewportSize.width);
	hasher.combine(viewportSize.height);
	hasher.combine(GeometryHasher::hash(compose, GeometryHashMode::PositionDependent));
	return hasher.get();
}

bool InvertedGeometryProvider::isViewportResized(D2dContext const& context) const {
	D2D1_SIZE_F currentViewportSize = context.getDeviceContext()->GetSize();
	return viewportSize.width != currentViewportSize.width || viewportSize.height != currentViewportSize.height;
}
