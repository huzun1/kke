#pragma once

#include <array>

namespace kke {
/**
 * @brief 4x5 color transform matrix stored in row-major order.
 */
struct ColorMatrix {
	/**
	 * @brief Matrix coefficients laid out as 20 consecutive floats.
	 */
	std::array<float, 20> values {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	};
};
}	// namespace kke
