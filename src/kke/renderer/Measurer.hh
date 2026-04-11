#pragma once

#include <string_view>

#include "kke/geometry/primitives/Point.hh"
#include "kke/resources/font/FontAppearance.hh"
#include "kke/renderer/shadow/ShadowAppearance.hh"

class Measurer {
public:
    kke::Scale2f getViewportSize();

    kke::Scale2f measureTextSize(
        std::string_view text,
        kke::FontAppearance const& fontAppearance,
        kke::ShadowAppearance const& shadowAppearance);

    kke::Scale2f measureTextSize(
        std::wstring_view text,
        kke::FontAppearance const& fontAppearance,
        kke::ShadowAppearance const& shadowAppearance);
};
