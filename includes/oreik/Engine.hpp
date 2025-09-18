#pragma once

#include <d2d1.h>
#include <d2d1_1.h>

#include <memory>

#include "ResourceAllocator.hpp"
#include "brush/Brush.hpp"
#include "common/Ellipse.hpp"
#include "common/Point.hpp"
#include "common/Rect.hpp"
#include "oreik/effect/Effect.hpp"
#include "oreik/effect/EffectContainer.hpp"
#include "oreik/effect/impl/BlurEffect.hpp"

namespace oreik {
class Engine {
	ID2D1DeviceContext* deviceContext;

	ID2D1Bitmap1* renderTarget;
	ID2D1Bitmap1* effectScreenBitmap;  // Temporary bitmap for holding screen copies
	oreik::ResourceAllocator resourceAllocator;
	oreik::EffectContainer effectContainer;

public:
	Engine(ID2D1DeviceContext* deviceContext);

	void begin(ID2D1Bitmap* screen);

	void end(ID2D1Image** output);

	void drawLine(
		oreik::Point2f start,
		oreik::Point2f end,
		oreik::Brush const& brush,
		float strokeWidth);

	void drawRect(
		oreik::Rect const& rect,
		oreik::Brush const& brush,
		float strokeWidth);

	void drawRounded(
		oreik::Rect const& rect,
		float radius,
		oreik::Brush const& brush,
		float strokeWidth);

	void drawEllipse(
		oreik::Ellipse const& ellipse,
		oreik::Brush const& brush,
		float strokeWidth);

	void fillRect(
		oreik::Rect const& rect,
		oreik::Brush const& brush);

	void fillRounded(
		oreik::Rect const& rect,
		float radius,
		oreik::Brush const& brush);

	void fillEllipse(
		oreik::Ellipse const& ellipse,
		oreik::Brush const& brush);

	template <typename T>
	std::shared_ptr<T> aquireOrCreateEffect() {
		return effectContainer.acquireOrCreateEffect<T>();
	}

	void blur(
		float deviation,
		oreik::BlurBorderMode borderMode = oreik::BlurBorderMode::HARD,
		oreik::BlurOptimization optimization = oreik::BlurOptimization::BALANCED);

	void effect(std::shared_ptr<Effect> effect);
};
};	// namespace oreik
