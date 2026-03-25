#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "kke/brush/Brush.hh"
#include "kke/common/Geometry.hh"
#include "kke/common/Point.hh"
#include "kke/common/Scale.hh"
#include "kke/common/geometry/Ellipse.hh"
#include "kke/common/geometry/Rect.hh"
#include "kke/common/geometry/RoundedRect.hh"
#include "kke/common/geometry/Triangle.hh"
#include "kke/effect/Effect.hh"
#include "kke/effect/impl/BlurEffect.hh"
#include "kke/font/FontData.hh"
#include "kke/font/FontWeight.hh"

struct ID2D1Factory;
struct ID2D1DeviceContext;
struct ID2D1Bitmap;
struct ID2D1Bitmap1;
struct ID2D1Image;

namespace kke {
enum class InterpolationMode {
	NEAREST,
	LINEAR,
	CUBIC,
	MULTI_SAMPLE_LINEAR,
	ANISOTROPIC,
	HIGH_QUALITY_CUBIC
};

class Engine {
	struct Impl;
	std::unique_ptr<Impl> impl;
	std::vector<std::shared_ptr<Effect>> effects;

public:
	Engine(ID2D1Factory* factory, ID2D1DeviceContext* deviceContext);
	~Engine();

	Engine(Engine&&) noexcept;
	Engine& operator=(Engine&&) noexcept;

	Engine(Engine const&) = delete;
	Engine& operator=(Engine const&) = delete;

	void init(std::vector<FontData> loadFonts);

	void begin(ID2D1Bitmap* screen);

	void end(ID2D1Image** output);

	void flush();

	void clear();

	kke::Scale2f getViewportSize() const;

	kke::Scale2f getTextSize(
		std::string const& text,
		FontWeight weight,
		std::string const& fontFamily,
		int32_t fontSize);

	kke::Scale2f getTextSize(
		std::wstring const& text,
		FontWeight weight,
		std::wstring const& fontFamily,
		int32_t fontSize);

	void drawLine(
		kke::Point2f start,
		kke::Point2f end,
		kke::Brush const& brush,
		float strokeWidth);

	void drawTriangle(
		kke::Triangle const& rect,
		kke::Brush const& brush,
		float strokeWidth);

	void drawRect(
		kke::Rect const& rect,
		kke::Brush const& brush,
		float strokeWidth);

	void drawRounded(
		kke::RoundedRect const& rect,
		kke::Brush const& brush,
		float strokeWidth);

	void drawEllipse(
		kke::Ellipse const& ellipse,
		kke::Brush const& brush,
		float strokeWidth);

	void drawText(
		kke::Point2f const& position,
		std::string const& text,
		FontWeight weight,
		std::string const& fontFamily,
		int32_t fontSize,
		kke::Brush const& brush);

	void drawText(
		kke::Point2f const& position,
		std::wstring const& text,
		FontWeight weight,
		std::wstring const& fontFamily,
		int32_t fontSize,
		kke::Brush const& brush);

	void fillTriangle(
		kke::Triangle const& triangle,
		kke::Brush const& brush);

	void fillRect(
		kke::Rect const& rect,
		kke::Brush const& brush);

	void fillRounded(
		kke::RoundedRect const& rect,
		kke::Brush const& brush);

	void fillEllipse(
		kke::Ellipse const& ellipse,
		kke::Brush const& brush);

	void drawRectShadow(
		kke::Rect const& rect,
		kke::Brush const& brush,
		float deviation,
		bool clipOriginalGeometry = true);

	void drawRoundedShadow(
		kke::RoundedRect const& rect,
		kke::Brush const& brush,
		float deviation,
		bool clipOriginalGeometry = true);

	void drawEllipseShadow(
		kke::Ellipse const& ellipse,
		kke::Brush const& brush,
		float deviation,
		bool clipOriginalGeometry = true);

	void drawLineShadow(
		kke::Point2f start,
		kke::Point2f end,
		kke::Brush const& brush,
		float strokeWidth,
		float deviation);

	void drawTextShadow(
		kke::Point2f const& position,
		std::string const& text,
		FontWeight weight,
		std::string const& fontFamily,
		int32_t fontSize,
		kke::Brush const& brush,
		float deviation);

	void drawTextShadow(
		kke::Point2f const& position,
		std::wstring const& text,
		FontWeight weight,
		std::wstring const& fontFamily,
		int32_t fontSize,
		kke::Brush const& brush,
		float deviation);

	uint64_t loadTexture(const void* data, size_t size);

	void releaseTexture(uint64_t index);

	void drawImage(
		ID2D1Image* image,
		kke::InterpolationMode interpolationMode = kke::InterpolationMode::LINEAR);

	void drawTexture(
		size_t index,
		kke::Rect const& dimension,
		float opacity = 1.0f,
		kke::InterpolationMode interpolationMode = kke::InterpolationMode::LINEAR,
		std::optional<kke::Rect> srcRect = std::nullopt);

	void blur(
		float deviation,
		kke::Geometry* fillGeometry = nullptr,
		kke::BlurBorderMode borderMode = kke::BlurBorderMode::HARD,
		kke::BlurOptimization optimization = kke::BlurOptimization::BALANCED);

	void blur(
		ID2D1Image* input,
		float deviation,
		kke::Geometry* fillGeometry = nullptr,
		kke::BlurBorderMode borderMode = kke::BlurBorderMode::HARD,
		kke::BlurOptimization optimization = kke::BlurOptimization::BALANCED);

	template <typename T>
	std::shared_ptr<T> acquireOrCreateEffect() {
		for (const auto& effect : effects) {
			if (auto casted = std::dynamic_pointer_cast<T>(effect)) {
				return casted;
			}
		}
		auto effect = std::make_shared<T>();
		effects.push_back(effect);
		return effect;
	}

	void pushTranslate(kke::Point2f const& offset);

	void pushScale(kke::Point2f const& center, kke::Scale2f const& scale);

	void pushRotate(kke::Point2f const& center, float angle);

	void pushLayer(kke::Geometry const& geometry);

	void pushSurface();

	void popTransform();

	void popLayer();

	void popSurface(ID2D1Bitmap1** output);

	// FIXME: we don't want to expose this api, but currently we need it for rendering surface output with opacity...
	// In the future, we will replace whole surface background with command list API...
	void drawBitmap(
		ID2D1Bitmap* bitmap,
		kke::Rect const& dimension,
		float opacity = 1.0f,
		kke::InterpolationMode interpolationMode = kke::InterpolationMode::LINEAR,
		std::optional<kke::Rect> srcRect = std::nullopt);
};
};	// namespace kke
