#pragma once

#include <string_view>

#include "kke/appearance/brush/Brush.hh"
#include "kke/appearance/effect/shadow/ShadowAppearance.hh"
#include "kke/appearance/font/FontStyle.hh"
#include "kke/geometry/curved/RoundedRect.hh"
#include "kke/geometry/primitives/Line.hh"
#include "kke/geometry/primitives/Point.hh"
#include "kke/geometry/shapes/Rect.hh"
#include "kke/geometry/shapes/Triangle.hh"

namespace kke {
class ShadowRenderer {
public:
	void renderShadow(
		Line const& line,
		Brush const& brush,
		float thick,
		ShadowAppearance const& appearance);

	void renderShadow(
		Triangle const& triangle,
		Brush const& brush,
		ShadowAppearance const& appearance);

	void renderShadow(
		Rect const& rect,
		Brush const& brush,
		ShadowAppearance const& appearance);

	void renderShadow(
		RoundedRect const& rounded,
		Brush const& brush,
		ShadowAppearance const& appearance);

	void renderTextShadow(
		std::string_view text,
		Point2f const& position,
		Brush const& brush,
		FontStyle const& fontStyle,
		ShadowAppearance const& shadowAppearance);

	void renderTextShadow(
		std::wstring_view text,
		Point2f const& position,
		Brush const& brush,
		FontStyle const& fontStyle,
		ShadowAppearance const& shadowAppearance);
};
};	// namespace kke
