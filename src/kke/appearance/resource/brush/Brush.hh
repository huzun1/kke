#pragma once

#include <variant>

#include "impl/LinearGradientBrush.hh"
#include "impl/SolidColorBrush.hh"

namespace kke {
	using Brush = std::variant<SolidColorBrush, LinearGradientBrush>;
};	// namespace kke
