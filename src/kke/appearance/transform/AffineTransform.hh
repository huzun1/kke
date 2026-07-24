#pragma once

namespace kke {
/**
 * @brief Applies a two-dimensional affine transform.
 */
struct AffineTransform {
	float xx = 1.0f;
	float xy = 0.0f;
	float yx = 0.0f;
	float yy = 1.0f;
	float tx = 0.0f;
	float ty = 0.0f;
};
} // namespace kke
