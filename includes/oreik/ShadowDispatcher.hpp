#pragma once

#include <d2d1_1.h>
#include <wincodec.h>

#include <functional>

#include "common/Point.hpp"
#include "common/Scale.hpp"
#include "oreik/effect/EffectContainer.hpp"

namespace oreik {
class ShadowDisaptcher {
	ID2D1DeviceContext* deviceContext;
	EffectContainer* effectContainer;

public:
	ShadowDisaptcher(ID2D1DeviceContext* context, oreik::EffectContainer* container);

	Microsoft::WRL::ComPtr<ID2D1Image> dispatch(oreik::Scale2f const& geometryScale, float deviation, std::function<void(oreik::Point2f const& start)> drawFunc);
};
};	// namespace oreik
