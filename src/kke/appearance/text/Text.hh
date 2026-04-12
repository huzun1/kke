#pragma once

#include <string>
#include <variant>

#include "kke/appearance/text/font/FontAppearance.hh"
#include "kke/geometry/primitives/Point.hh"

namespace kke {
struct Text {
	std::variant<std::string, std::wstring> text;
	Point2f position;
	FontAppearance fontAppearance;
};
};	// namespace kke
