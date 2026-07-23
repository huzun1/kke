#include "ColorQuantizer.hh"

#include <algorithm>
#include <cmath>

using namespace kke;

Color ColorQuantizer::quantize(Color const& color) {
	return {
		toFloat(quantizeComponent(color.r)),
		toFloat(quantizeComponent(color.g)),
		toFloat(quantizeComponent(color.b)),
		toFloat(quantizeComponent(color.a))
	};
}

uint8_t ColorQuantizer::quantizeComponent(float component) {
	if (!std::isfinite(component)) {
		return 0;
	}

	float clamped = std::clamp(component, 0.0f, 1.0f);
	return static_cast<uint8_t>(std::round(clamped * 255.0f));
}

float ColorQuantizer::toFloat(uint8_t component) {
	return static_cast<float>(component) / 255.0f;
}
