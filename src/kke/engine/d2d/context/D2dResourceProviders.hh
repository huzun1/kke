#pragma once

#include <memory>

#include "kke/engine/d2d/resource/brush/BrushProvider.hh"
#include "kke/engine/d2d/resource/font/FontProvider.hh"
#include "kke/engine/d2d/resource/geometry/GeometryProvider.hh"
#include "kke/engine/d2d/resource/geometry/InvertedGeometryProvider.hh"

namespace kke {
class D2dResourceProviders {
	std::unique_ptr<BrushProvider> brushProvider;
	std::unique_ptr<FontProvider> fontProvider;
	std::unique_ptr<GeometryProvider> geometryProvider;
    std::unique_ptr<InvertedGeometryProvider> invertedGeometryProvider;

public:
    D2dResourceProviders();

	void update(D2dContext const& context, D2D1_SIZE_F viewportSize);

	BrushProvider* getBrushProvider();

	FontProvider* getFontProvider();

    GeometryProvider* getGeometryProvider();

    InvertedGeometryProvider* getInvertedGeometryProvider();
};
};	// namespace kke
