#include "GeometryCompose.hh"

using namespace kke;

void GeometryCompose::add(Geometry const& geometry) {
	geometries.push_back(geometry);
}

std::vector<Geometry> const& GeometryCompose::getGeometries() const {
	return geometries;
}
