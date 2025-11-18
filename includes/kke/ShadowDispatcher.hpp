#pragma once

#include <d2d1_1.h>
#include <wincodec.h>

#include <functional>

#include "common/Point.hpp"
#include "common/geometry/Rect.hpp"
#include "kke/ResourceAllocator.hpp"
#include "kke/effect/EffectContainer.hpp"

namespace kke {
class ShadowDisaptcher {
	const float bufferPad = 40.0f;

	ID2D1DeviceContext* deviceContext;
	kke::ResourceAllocator* resourceAllocator;
	EffectContainer* effectContainer;

public:
	ShadowDisaptcher(ID2D1DeviceContext* context, kke::ResourceAllocator* resourceAllocator, kke::EffectContainer* container);

	void dispatch(kke::Rect const& dimension, float deviation, std::function<void(kke::Point2f const& start)> drawFunc, ID2D1Image** output);

	kke::Point2f computeOffset(kke::Rect const& dimension) const;
};
};	// namespace kke
