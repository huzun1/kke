#pragma once

#include <string_view>

#include "ShadowAppearance.hh"
#include "kke/geometry/Geometry.hh"
#include "kke/geometry/Point.hh"
#include "kke/geometry/Rect.hh"
#include "kke/geometry/RoundedRect.hh"
#include "kke/geometry/Triangle.hh"
#include "kke/resources/font/FontAppearance.hh"
#include "kke/resources/brush/Brush.hh"

namespace kke {
class ShadowRenderer {
public:
	void renderGeometry(
        kke::Geometry const& geometry,
        kke::Brush const& brush,
        kke::ShadowAppearance const& appearance
    );

    void renderLine(
        kke::Point2f const& start,
        kke::Point2f const& end,
        kke::Brush const& brush,
        kke::ShadowAppearance const& appearance
    );

    void renderTriangle(
        kke::Triangle const& triangle,
        kke::Brush const& brush,
        kke::ShadowAppearance const& appearance
    );

    void renderRect(
        kke::Rect const& rect,
        kke::Brush const& brush,
        kke::ShadowAppearance const& appearance
    );

    void renderRounded(
        kke::RoundedRect const& rounded,
        kke::Brush const& brush,
        kke::ShadowAppearance const& appearance
    );

    void renderText(
        std::string_view text,
        kke::Point2f const& position,
        kke::Brush const& brush,
        kke::FontAppearance const& fontAppearance,
        kke::ShadowAppearance const& shadowAppearance
    );
};
};	// namespace kke
