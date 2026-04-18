#pragma once

#include <string>
#include <variant>

#include "kke/appearance/resource/font/FontAppearance.hh"
#include "kke/geometry/primitives/Point.hh"

namespace kke {
struct Text {
	std::variant<std::string, std::wstring> text;
	Point position;
	FontAppearance fontAppearance;
};
};	// namespace kke
