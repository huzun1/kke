#pragma once

#include <string_view>

#include "kke/geometry/primitives/Point.hh"
#include "kke/resources/brush/Brush.hh"
#include "kke/resources/font/style/FontStyle.hh"

namespace kke {
class TextRenderer {
public:
	void fillText(
		std::string_view text,
		kke::Point2f const& position,
		kke::Brush const& brush,
		kke::FontStyle const& style);

	void fillText(
		std::wstring_view text,
		kke::Point2f const& position,
		kke::Brush const& brush,
		kke::FontStyle const& style);		
};
};
