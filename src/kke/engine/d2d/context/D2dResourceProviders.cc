#include "D2dResourceProviders.hh"

using namespace kke;

D2dResourceProviders::D2dResourceProviders() {
    geometryProvider = std::make_unique<GeometryProvider>();
    invertedGeometryProvider = std::make_unique<InvertedGeometryProvider>();
}

GeometryProvider* D2dResourceProviders::getGeometryProvider() {
    return geometryProvider.get();
}

InvertedGeometryProvider* D2dResourceProviders::getInvertedGeometryProvider() {
    return invertedGeometryProvider.get();
}
