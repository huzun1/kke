#pragma once

#include <d2d1.h>
#include <d2d1_1.h>
#include <wincodec.h>
#include <wrl/client.h>

#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <stack>

#include "RenderSurface.hpp"
#include "ResourceAllocator.hpp"
#include "ShadowDispatcher.hpp"
#include "TextureRepository.hpp"
#include "brush/Brush.hpp"
#include "common/Point.hpp"
#include "common/geometry/Ellipse.hpp"
#include "common/geometry/Rect.hpp"
#include "oreik/common/Scale.hpp"
#include "oreik/common/geometry/Ellipse.hpp"
#include "oreik/common/geometry/RoundedRect.hpp"
#include "oreik/effect/Effect.hpp"
#include "oreik/effect/EffectContainer.hpp"
#include "oreik/effect/impl/BlurEffect.hpp"
#include "oreik/transform/Matrix.hpp"

namespace oreik {
enum class InterpolationMode {
	NEAREST,
	LINEAR,
	CUBIC,
	MULTI_SAMPLE_LINEAR,
	ANISOTROPIC,
	HIGH_QUALITY_CUBIC
};

class Engine {
	ID2D1DeviceContext* deviceContext;

	ID2D1Bitmap1* renderTarget;
	ID2D1Bitmap1* effectScreenBitmap;  // Temporary bitmap for holding screen copies
	oreik::Matrix matrix;
	oreik::TextureRepository textureRepository;
	oreik::ResourceAllocator resourceAllocator;
	oreik::EffectContainer effectContainer;
	oreik::ShadowDisaptcher shadowDispatcher;
	std::stack<RenderSurface*> surfaceStack;

public:
	Engine(ID2D1DeviceContext* deviceContext);

	~Engine();

	void begin(ID2D1Bitmap* screen);

	void end(ID2D1Image** output);

	void clear();

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
		oreik::RoundedRect const& rect,
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
		oreik::RoundedRect const& rect,
		oreik::Brush const& brush);

	void fillEllipse(
		oreik::Ellipse const& ellipse,
		oreik::Brush const& brush);

	void drawRectShadow(
		oreik::Rect const& rect,
		oreik::Brush const& brush,
		float deviation);

	void drawRoundedShadow(
		oreik::RoundedRect const& rect,
		oreik::Brush const& brush,
		float deviation);

	void drawEllipseShadow(
		oreik::Ellipse const& ellipse,
		oreik::Brush const& brush,
		float deviation);

	uint64_t loadTexture(const void* data, size_t size);

	void drawImage(
		ID2D1Image* image,
		oreik::InterpolationMode interpolationMode = oreik::InterpolationMode::LINEAR);

	void drawTexture(
		size_t index,
		oreik::Rect const& dimension,
		float opacity = 1.0f,
		oreik::InterpolationMode interpolationMode = oreik::InterpolationMode::LINEAR,
		std::optional<oreik::Rect> srcRect = std::nullopt);

	void blur(
		float deviation,
		oreik::BlurBorderMode borderMode = oreik::BlurBorderMode::HARD,
		oreik::BlurOptimization optimization = oreik::BlurOptimization::BALANCED);

	template <typename T>
	std::shared_ptr<T> aquireOrCreateEffect() {
		return effectContainer.acquireOrCreateEffect<T>();
	}

	void effect(ID2D1Image* image, std::shared_ptr<Effect> effect);

	void effect(std::shared_ptr<Effect> effect);

	void pushScale(oreik::Point2f const& center, oreik::Scale2f const& scale);

	void pushRotate(oreik::Point2f const& center, float angle);

	void popTransform();

	void pushSurface();

	void popSurface(ID2D1Bitmap1** output);

private:
	void drawBitmap(
		ID2D1Bitmap* bitmap,
		oreik::Rect const& dimension,
		float opacity = 1.0f,
		oreik::InterpolationMode interpolationMode = oreik::InterpolationMode::LINEAR,
		std::optional<oreik::Rect> srcRect = std::nullopt);

	static D2D1_INTERPOLATION_MODE toD2D1InterpolationMode(oreik::InterpolationMode mode);
};
};	// namespace oreik
