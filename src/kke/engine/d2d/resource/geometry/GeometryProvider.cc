#include <cstdint>
#include <vector>

#include "GeometryProvider.hh"
#include "GeometryFactory.hh"
#include "hash/GeometryHashMode.hh"
#include "hash/GeometryHasher.hh"

using namespace kke;
using namespace Microsoft::WRL;

GeometryProvider::GeometryProvider(uint32_t limit)
	: storage(limit) {
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

ComPtr<ID2D1Geometry> GeometryProvider::get(D2dContext const& context, GeometryCompose const& compose) {
	uint64_t key = GeometryHasher::hash(compose, GeometryHashMode::PositionDependent);

	ComPtr<ID2D1Geometry> cachedGeometry = storage.get(key);
	if (cachedGeometry) {
		return cachedGeometry;
	}

	std::vector<ComPtr<ID2D1Geometry>> geometries;
	std::vector<ID2D1Geometry*> d2dGeometries;
	auto const& sourceGeometries = compose.getGeometries();

	geometries.reserve(sourceGeometries.size());
	d2dGeometries.reserve(sourceGeometries.size());

	for (auto const& geometry : sourceGeometries) {
		ComPtr<ID2D1Geometry> d2dGeometry = get(context, geometry);
		if (!d2dGeometry) {
			return nullptr;
		}

		d2dGeometries.push_back(d2dGeometry.Get());
		geometries.push_back(d2dGeometry);
	}

	ComPtr<ID2D1GeometryGroup> createdGeometry;
	HRESULT result = context.getFactory()->CreateGeometryGroup(
		D2D1_FILL_MODE_WINDING,
		d2dGeometries.data(),
		static_cast<UINT32>(d2dGeometries.size()),
		&createdGeometry);
	if (FAILED(result)) {
		return nullptr;
	}

	storage.put(key, createdGeometry);
	return createdGeometry;
}
