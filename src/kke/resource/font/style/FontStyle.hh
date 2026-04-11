#pragma once

#include <string>

#include "FontWeight.hh"

namespace kke {
struct FontStyle {
	std::string fontFamily;
	float fontSize;
	kke::FontWeight weight;
};
};	// namespace kke
