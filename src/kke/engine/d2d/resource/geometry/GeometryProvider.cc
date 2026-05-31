#include "GeometryProvider.hh"

#include <cstdint>

#include "kke/engine/d2d/resource/geometry/factory/GeometryFactory.hh"

#include "hash/GeometryHashMode.hh"
#include "hash/GeometryHasher.hh"

using namespace kke;
using namespace Microsoft::WRL;

GeometryProvider::GeometryProvider(uint32_t limit) : storage(limit) {
}

ComPtr<ID2D1Geometry> GeometryProvider::get(D2dContext const& context, Geometry const& geometry) {
	uint64_t key = GeometryHasher::hash(geometry, GeometryHashMode::PositionDependent);

	ComPtr<ID2D1Geometry> cachedGeometry = storage.get(key);
	if (cachedGeometry) {
		return cachedGeometry;
	}

	ComPtr<ID2D1Geometry> createdGeometry = GeometryFactory::create(context, geometry);
	if (!createdGeometry) {
		return nullptr;
	}

	storage.put(key, createdGeometry);
	return createdGeometry;
}

ComPtr<ID2D1Geometry>
GeometryProvider::get(D2dContext const& context, GeometryCompose const& compose) {
	uint64_t key = GeometryHasher::hash(compose, GeometryHashMode::PositionDependent);

	ComPtr<ID2D1Geometry> cachedGeometry = storage.get(key);
	if (cachedGeometry) {
		return cachedGeometry;
	}

	ComPtr<ID2D1Geometry> createdGeometry = GeometryFactory::create(context, compose);
	if (!createdGeometry) {
		return nullptr;
	}

	storage.put(key, createdGeometry);
	return storage.get(key);
}
