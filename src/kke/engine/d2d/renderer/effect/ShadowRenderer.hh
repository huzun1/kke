#pragma once

#include <string_view>

#include "kke/appearance/brush/Brush.hh"
#include "kke/appearance/effect/shadow/ShadowAppearance.hh"
#include "kke/appearance/text/font/FontAppearance.hh"
#include "kke/geometry/curved/RoundedRect.hh"
#include "kke/geometry/primitives/Line.hh"
#include "kke/geometry/primitives/Point.hh"
#include "kke/geometry/shapes/Rect.hh"
#include "kke/geometry/shapes/Triangle.hh"

namespace kke {
class ShadowRenderer {
public:
	void renderLineShadow(
		Line const& line,
		Brush const& brush,
		float thick,
		ShadowAppearance const& appearance);

	void renderTriangleShadow(
		Triangle const& triangle,
		Brush const& brush,
		ShadowAppearance const& appearance);

	void renderRectShadow(
		Rect const& rect,
		Brush const& brush,
		ShadowAppearance const& appearance);

	void renderRoundedShadow(
		RoundedRect const& rounded,
		Brush const& brush,
		ShadowAppearance const& appearance);

	void renderTextShadow(
		std::string_view text,
		Point2f const& position,
		Brush const& brush,
		FontAppearance const& fontAppearance,
		ShadowAppearance const& shadowAppearance);

	void renderTextShadow(
		std::wstring_view text,
		Point2f const& position,
		Brush const& brush,
		FontAppearance const& fontAppearance,
		ShadowAppearance const& shadowAppearance);
};
};	// namespace kke
