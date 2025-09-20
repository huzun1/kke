#pragma once

#include <d2d1_1.h>
#include <wincodec.h>

#include <functional>

#include "common/Point.hpp"
#include "common/geometry/Rect.hpp"
#include "oreik/effect/EffectContainer.hpp"

namespace oreik {
class ShadowDisaptcher {
	const float bufferPad = 40.0f;

	ID2D1DeviceContext* deviceContext;
	EffectContainer* effectContainer;

public:
	ShadowDisaptcher(ID2D1DeviceContext* context, oreik::EffectContainer* container);

	void dispatch(oreik::Rect const& dimension, float deviation, std::function<void(oreik::Point2f const& start)> drawFunc, ID2D1Image** output);

	oreik::Point2f computeOffset(oreik::Rect const& dimension) const;
};
};	// namespace oreik
