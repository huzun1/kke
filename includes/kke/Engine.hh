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

#include "RenderSurface.hh"
#include "ResourceAllocator.hh"
#include "ShadowDispatcher.hh"
#include "TextureRepository.hh"
#include "brush/Brush.hh"
#include "common/Point.hh"
#include "common/geometry/Ellipse.hh"
#include "common/geometry/Rect.hh"
#include "kke/common/Geometry.hh"
#include "kke/common/Scale.hh"
#include "kke/common/geometry/Ellipse.hh"
#include "kke/common/geometry/RoundedRect.hh"
#include "kke/common/geometry/Triangle.hh"
#include "kke/effect/Effect.hh"
#include "kke/effect/EffectContainer.hh"
#include "kke/effect/impl/BlurEffect.hh"
#include "kke/font/FontData.hh"
#include "kke/font/FontLoader.hh"
#include "kke/font/FontWeight.hh"
#include "kke/transform/Matrix.hh"

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
		float deviation, bool clipOriginalGeometry = true);

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
		return effectContainer.acquireOrCreateEffect<T>();
	}

	void pushTranslate(kke::Point2f const& offset);

	void pushScale(kke::Point2f const& center, kke::Scale2f const& scale);

	void pushRotate(kke::Point2f const& center, float angle);

	void pushLayer(kke::Geometry const& geometry);

	void pushSurface();

	void popTransform();

	void popLayer();

	void popSurface(ID2D1Bitmap1** output);

private:
	void copyScreenToEffectBitmap();

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
