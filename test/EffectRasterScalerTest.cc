#include <variant>

#include "kke/appearance/resource/effect/blur/BlurEffect.hh"
#include "kke/appearance/resource/effect/color_matrix/ColorMatrixEffect.hh"
#include "kke/appearance/resource/effect/directional_blur/DirectionalBlurEffect.hh"
#include "kke/appearance/resource/effect/shadow/ShadowEffect.hh"
#include "kke/engine/d2d/renderer/effect/EffectRasterScaler.hh"

int main() {
	kke::EffectRasterScaler scaler;

	auto blur = std::get<kke::BlurEffect>(scaler.scaleEffect(kke::BlurEffect{20.0f}, 0.5f));
	if (blur.appearance.radius != 10.0f) {
		return 1;
	}

	kke::DirectionalBlurEffect directionalBlur{
		.standardDeviation = 12.0f,
	};
	auto scaledDirectionalBlur =
		std::get<kke::DirectionalBlurEffect>(scaler.scaleEffect(directionalBlur, 0.5f));
	if (scaledDirectionalBlur.standardDeviation != 6.0f) {
		return 1;
	}

	kke::ShadowEffect shadow{
		.offset = {8.0f, -4.0f},
		.blurStandardDeviation = 10.0f,
	};
	auto scaledShadow = std::get<kke::ShadowEffect>(scaler.scaleEffect(shadow, 0.5f));
	if (scaledShadow.offset.x != 4.0f || scaledShadow.offset.y != -2.0f ||
		scaledShadow.blurStandardDeviation != 5.0f) {
		return 1;
	}

	kke::ColorMatrixEffect colorMatrix;
	if (!std::holds_alternative<kke::ColorMatrixEffect>(scaler.scaleEffect(colorMatrix, 0.5f))) {
		return 1;
	}

	return 0;
}
