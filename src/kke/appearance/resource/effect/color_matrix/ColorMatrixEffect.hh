#pragma once

#include "kke/appearance/resource/effect/color_matrix/ColorMatrix.hh"
#include "kke/appearance/resource/effect/color_matrix/ColorMatrixAlphaMode.hh"

namespace kke {
/**
 * @brief Applies a 4x5 color matrix transform to the source.
 */
struct ColorMatrixEffect {
	/**
	 * @brief Matrix coefficients used for the color transform.
	 */
	ColorMatrix colorMatrix {};

	/**
	 * @brief Alpha interpretation used while evaluating the matrix.
	 */
	ColorMatrixAlphaMode alphaMode = ColorMatrixAlphaMode::Premultiplied;

	/**
	 * @brief Whether output values are clamped to the valid color range.
	 */
	bool clampOutput = false;
};
}	// namespace kke
