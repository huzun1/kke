#include "InvertedGeometryProvider.hh"

#include <cstdio>

#include "kke/engine/d2d/resource/geometry/factory/invert/InvertedGeometryFactory.hh"
#include "kke/engine/d2d/resource/geometry/hash/GeometryHashMode.hh"
#include "kke/engine/d2d/resource/geometry/hash/GeometryHasher.hh"
#include "kke/utils/Hasher.hh"

using namespace kke;
using namespace Microsoft::WRL;

void InvertedGeometryProvider::syncViewportSize(
	D2dContext const& context, D2D1_SIZE_F viewportSize
) {
	bool isViewportResized = this->isViewportResized(viewportSize);
	if (isViewportResized) {
		std::printf(
			"[kke][InvertedGeometryProvider] viewport resized: %.2f x %.2f\n",
			viewportSize.width,
			viewportSize.height
		);
		geometries.clear();
		this->viewportSize = viewportSize;
	}

	viewportGeometryCache.syncViewportSize(context, viewportSize);
}

ComPtr<ID2D1Geometry> InvertedGeometryProvider::get(
	D2dContext const& context, GeometryProvider& geometryProvider, Geometry const& geometry
) {
	uint64_t key = createInvertedGeometryKey(context, geometry);

	ComPtr<ID2D1Geometry> cachedGeometry = geometries.get(key);
	if (cachedGeometry) {
		return cachedGeometry;
	}

	ComPtr<ID2D1Geometry> invertedGeometry = InvertedGeometryFactory::createInvertedGeometry(
		context,
		geometryProvider,
		viewportGeometryCache.get(context),
		geometry
	);
	if (!invertedGeometry) {
		std::printf("[kke][InvertedGeometryProvider] failed to create inverted geometry\n");
		return nullptr;
	}

	geometries.put(key, invertedGeometry);
	return invertedGeometry;
}

ComPtr<ID2D1Geometry> InvertedGeometryProvider::get(
	D2dContext const& context, GeometryProvider& geometryProvider, GeometryCompose const& compose
) {
	uint64_t key = createInvertedGeometryKey(context, compose);

	ComPtr<ID2D1Geometry> cachedGeometry = geometries.get(key);
	if (cachedGeometry) {
		return cachedGeometry;
	}

	ComPtr<ID2D1Geometry> invertedGeometry = InvertedGeometryFactory::createInvertedGeometry(
		context,
		geometryProvider,
		viewportGeometryCache.get(context),
		compose
	);
	if (!invertedGeometry) {
		std::printf("[kke][InvertedGeometryProvider] failed to create inverted compose geometry\n");
		return nullptr;
	}

	geometries.put(key, invertedGeometry);
	return invertedGeometry;
}

uint64_t InvertedGeometryProvider::createInvertedGeometryKey(
	D2dContext const& context, Geometry const& geometry
) {
	Hasher hasher;
	hasher.combine(viewportSize.width);
	hasher.combine(viewportSize.height);
	hasher.combine(GeometryHasher::hash(geometry, GeometryHashMode::PositionDependent));
	return hasher.get();
}

uint64_t InvertedGeometryProvider::createInvertedGeometryKey(
	D2dContext const& context, GeometryCompose const& compose
) {
	Hasher hasher;
	hasher.combine(viewportSize.width);
	hasher.combine(viewportSize.height);
	hasher.combine(GeometryHasher::hash(compose, GeometryHashMode::PositionDependent));
	return hasher.get();
}

bool InvertedGeometryProvider::isViewportResized(D2D1_SIZE_F viewportSize) const {
	return this->viewportSize.width != viewportSize.width ||
		   this->viewportSize.height != viewportSize.height;
}
