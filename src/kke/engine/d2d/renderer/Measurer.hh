#pragma once

#include <string_view>

#include "kke/geometry/primitives/Point.hh"
#include "kke/appearance/text/font/FontAppearance.hh"
#include "kke/appearance/effect/shadow/ShadowAppearance.hh"

class Measurer {
public:
    kke::Scale getViewportSize();

    kke::Scale measureTextSize(
        std::string_view text,
        kke::FontAppearance const& fontAppearance,
        kke::ShadowAppearance const& shadowAppearance);

    kke::Scale measureTextSize(
        std::wstring_view text,
        kke::FontAppearance const& fontAppearance,
        kke::ShadowAppearance const& shadowAppearance);
};
