#pragma once

#include <array>

namespace kke {
struct ColorMatrix {
	std::array<float, 20> values {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	};
};
}	// namespace kke
