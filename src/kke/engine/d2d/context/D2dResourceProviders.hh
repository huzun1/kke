#pragma once

#include <memory>

#include "kke/engine/d2d/resource/geometry/GeometryProvider.hh"
#include "kke/engine/d2d/resource/geometry/InvertedGeometryProvider.hh"

namespace kke {
class D2dResourceProviders {
	std::unique_ptr<GeometryProvider> geometryProvider;
    std::unique_ptr<InvertedGeometryProvider> invertedGeometryProvider;

public:
    D2dResourceProviders();

    GeometryProvider* getGeometryProvider();

    InvertedGeometryProvider* getInvertedGeometryProvider();
};
};	// namespace kke
