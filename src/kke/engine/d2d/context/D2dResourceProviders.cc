#include "D2dResourceProviders.hh"

using namespace kke;

D2dResourceProviders::D2dResourceProviders() {
	brushProvider = std::make_unique<BrushProvider>();
	fontProvider = std::make_unique<FontProvider>();
	geometryProvider = std::make_unique<GeometryProvider>();
	invertedGeometryProvider = std::make_unique<InvertedGeometryProvider>();
	textureProvider = std::make_unique<TextureProvider>();
}

void D2dResourceProviders::update(D2dContext const& context, D2D1_SIZE_F viewportSize) {
	invertedGeometryProvider->syncViewportSize(context, viewportSize);
}

BrushProvider* D2dResourceProviders::getBrushProvider() {
	return brushProvider.get();
}

FontProvider* D2dResourceProviders::getFontProvider() {
	return fontProvider.get();
}

GeometryProvider* D2dResourceProviders::getGeometryProvider() {
	return geometryProvider.get();
}

InvertedGeometryProvider* D2dResourceProviders::getInvertedGeometryProvider() {
	return invertedGeometryProvider.get();
}

TextureProvider* D2dResourceProviders::getTextureProvider() {
	return textureProvider.get();
}
