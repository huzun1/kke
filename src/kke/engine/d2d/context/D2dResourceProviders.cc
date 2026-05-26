#include "D2dResourceProviders.hh"

using namespace kke;

D2dResourceProviders::D2dResourceProviders() {
    brushProvider = std::make_unique<BrushProvider>();
    geometryProvider = std::make_unique<GeometryProvider>();
    invertedGeometryProvider = std::make_unique<InvertedGeometryProvider>();
}

void D2dResourceProviders::update(D2dContext const& context, D2D1_SIZE_F viewportSize) {
	invertedGeometryProvider->syncViewportSize(context, viewportSize);
}

BrushProvider* D2dResourceProviders::getBrushProvider() {
    return brushProvider.get();
}

GeometryProvider* D2dResourceProviders::getGeometryProvider() {
    return geometryProvider.get();
}

InvertedGeometryProvider* D2dResourceProviders::getInvertedGeometryProvider() {
    return invertedGeometryProvider.get();
}
