#pragma once

#include <d2d1_1.h>
#include <wincodec.h>

#include <functional>

#include "common/Point.hpp"
#include "common/geometry/Rect.hpp"
#include "oreik/effect/EffectContainer.hpp"

namespace oreik {
struct ShadowDisaptcherResult {
	Microsoft::WRL::ComPtr<ID2D1Image> output;
	oreik::Point2f fixedOffset;
};

class ShadowDisaptcher {
	ID2D1DeviceContext* deviceContext;
	EffectContainer* effectContainer;

public:
	ShadowDisaptcher(ID2D1DeviceContext* context, oreik::EffectContainer* container);

	ShadowDisaptcherResult dispatch(oreik::Rect const& dimension, float deviation, std::function<void(oreik::Point2f const& start)> drawFunc);
};
};	// namespace oreik
