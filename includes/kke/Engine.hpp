#pragma once

#include <d2d1.h>
#include <d2d1_1.h>
#include <wincodec.h>
#include <wrl/client.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <stack>
#include <string>
#include <vector>

#include "RenderSurface.hpp"
#include "ResourceAllocator.hpp"
#include "ShadowDispatcher.hpp"
#include "TextureRepository.hpp"
#include "brush/Brush.hpp"
#include "common/Point.hpp"
#include "common/geometry/Ellipse.hpp"
#include "common/geometry/Rect.hpp"
#include "kke/common/Geometry.hpp"
#include "kke/common/Scale.hpp"
#include "kke/common/geometry/Ellipse.hpp"
#include "kke/common/geometry/RoundedRect.hpp"
#include "kke/effect/Effect.hpp"
#include "kke/effect/EffectContainer.hpp"
#include "kke/effect/impl/BlurEffect.hpp"
#include "kke/font/FontData.hpp"
#include "kke/font/FontLoader.hpp"
#include "kke/font/FontWeight.hpp"
#include "kke/transform/Matrix.hpp"

using namespace Microsoft::WRL;

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
	ID2D1Factory* factory;
	ID2D1DeviceContext* deviceContext;

	ID2D1Bitmap1* renderTarget;
	ID2D1Bitmap1* effectScreenBitmap;  // Temporary bitmap for holding screen copies
	kke::FontLoader fontLoader;
	kke::Matrix matrix;
	kke::ResourceAllocator resourceAllocator;
	kke::TextureRepository textureRepository;
	kke::EffectContainer effectContainer;
	kke::ShadowDisaptcher shadowDispatcher;
	std::stack<RenderSurface*> surfaceStack;

public:
	Engine(ID2D1Factory* factory, ID2D1DeviceContext* deviceContext);

	void init(std::vector<FontData> loadFonts);

	void begin(ID2D1Bitmap* screen);

	void end(ID2D1Image** output);

	void flush();

	void clear();

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
		float deviation);

	void drawRoundedShadow(
		kke::RoundedRect const& rect,
		kke::Brush const& brush,
		float deviation);

	void drawEllipseShadow(
		kke::Ellipse const& ellipse,
		kke::Brush const& brush,
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
	std::shared_ptr<T> aquireOrCreateEffect() {
		return effectContainer.acquireOrCreateEffect<T>();
	}

	void pushScale(kke::Point2f const& center, kke::Scale2f const& scale);

	void pushRotate(kke::Point2f const& center, float angle);

	void pushLayer(kke::Geometry const& geometry);

	void pushSurface();

	void popTransform();

	void popLayer();

	void popSurface(ID2D1Bitmap1** output);

private:
	void effect(ID2D1Image* image, std::shared_ptr<Effect> effect);

	void effect(std::shared_ptr<Effect> effect);

	void drawBitmap(
		ID2D1Bitmap* bitmap,
		kke::Rect const& dimension,
		float opacity = 1.0f,
		kke::InterpolationMode interpolationMode = kke::InterpolationMode::LINEAR,
		std::optional<kke::Rect> srcRect = std::nullopt);

	std::wstring toWString(std::string const& str);

	static D2D1_INTERPOLATION_MODE toD2D1InterpolationMode(kke::InterpolationMode mode);
};
};	// namespace kke
