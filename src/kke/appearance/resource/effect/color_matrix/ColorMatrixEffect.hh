#pragma once

#include "kke/appearance/resource/effect/color_matrix/ColorMatrix.hh"
#include "kke/appearance/resource/effect/color_matrix/ColorMatrixAlphaMode.hh"

namespace kke {
struct ColorMatrixEffect {
	ColorMatrix colorMatrix {};
	ColorMatrixAlphaMode alphaMode = ColorMatrixAlphaMode::Premultiplied;
	bool clampOutput = false;
};
}	// namespace kke
