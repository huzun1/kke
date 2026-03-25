#include "kke/Engine.hh"

#include <Windows.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <wrl/client.h>

#include <memory>
#include <optional>
#include <stack>
#include <stdexcept>
#include <string>
#include <utility>

#include "internal/HResult.hh"
#include "kke/RenderSurface.hh"
#include "kke/ResourceAllocator.hh"
#include "kke/ShadowDispatcher.hh"
#include "kke/TextureRepository.hh"
#include "kke/effect/EffectInstance.hh"
#include "kke/font/FontLoader.hh"
#include "kke/internal/Hasher.hh"
#include "kke/transform/Matrix.hh"

using Microsoft::WRL::ComPtr;
using kke::internal::throwIfFailed;

namespace {
std::wstring toWString(const std::string& str) {
	const int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	if (size <= 0) {
		throw std::runtime_error("Failed to convert UTF-8 text to UTF-16.");
	}

	std::wstring result(static_cast<size_t>(size - 1), L'\0');
	const int converted = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, result.data(), size);
	if (converted == 0) {
		throw std::runtime_error("Failed to convert UTF-8 text to UTF-16.");
	}

	return result;
}

D2D1_INTERPOLATION_MODE toD2D1InterpolationMode(kke::InterpolationMode mode) {
	switch (mode) {
		case kke::InterpolationMode::NEAREST:
			return D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
		case kke::InterpolationMode::LINEAR:
			return D2D1_INTERPOLATION_MODE_LINEAR;
		case kke::InterpolationMode::CUBIC:
			return D2D1_INTERPOLATION_MODE_CUBIC;
		case kke::InterpolationMode::MULTI_SAMPLE_LINEAR:
			return D2D1_INTERPOLATION_MODE_MULTI_SAMPLE_LINEAR;
		case kke::InterpolationMode::ANISOTROPIC:
			return D2D1_INTERPOLATION_MODE_ANISOTROPIC;
		case kke::InterpolationMode::HIGH_QUALITY_CUBIC:
			return D2D1_INTERPOLATION_MODE_HIGH_QUALITY_CUBIC;
		default:
			throw std::runtime_error("Unsupported interpolation mode.");
	}
}

void copyScreenToEffectBitmap(ID2D1Bitmap1* renderTarget, ID2D1Bitmap1* effectScreenBitmap) {
	const D2D1_SIZE_U pixelSize = renderTarget->GetPixelSize();
	const D2D1_POINT_2U destPoint(0, 0);
	const D2D1_RECT_U srcRectangle(0, 0, pixelSize.width, pixelSize.height);
	throwIfFailed(
		effectScreenBitmap->CopyFromBitmap(&destPoint, renderTarget, &srcRectangle),
		"Failed to copy render target into effect bitmap");
}

void applyEffect(
	ID2D1DeviceContext* deviceContext,
	kke::ResourceAllocator* resourceAllocator,
	kke::Matrix const& matrix,
	ID2D1Image* image,
	std::shared_ptr<kke::Effect> const& effect) {
	EffectInstance* instance = resourceAllocator->acquireOrCreateEffect(effect);
	instance->lock();

	ComPtr<ID2D1Effect> d2d1Effect = instance->getD2D1Effect();
	d2d1Effect->SetInput(0, image);
	effect->setProperties(d2d1Effect);

	deviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	deviceContext->DrawImage(d2d1Effect.Get());
	deviceContext->SetTransform(matrix.build());
}
}  // namespace

namespace kke {
struct Engine::Impl {
	ID2D1Factory* factory;
	ID2D1DeviceContext* deviceContext;
	ComPtr<ID2D1Bitmap1> renderTarget;
	ComPtr<ID2D1Bitmap1> effectScreenBitmap;
	kke::FontLoader fontLoader;
	kke::Matrix matrix;
	kke::ResourceAllocator resourceAllocator;
	kke::TextureRepository textureRepository;
	kke::ShadowDisaptcher shadowDispatcher;
	std::stack<RenderSurface*> surfaceStack;

	Impl(ID2D1Factory* factory, ID2D1DeviceContext* deviceContext)
		: factory(factory),
		  deviceContext(deviceContext),
		  resourceAllocator(factory, deviceContext, &fontLoader),
		  textureRepository(deviceContext),
		  shadowDispatcher(deviceContext, &resourceAllocator) {
	}
};

Engine::Engine(ID2D1Factory* factory, ID2D1DeviceContext* deviceContext)
	: impl(std::make_unique<Impl>(factory, deviceContext)) {
}

Engine::~Engine() = default;

Engine::Engine(Engine&&) noexcept = default;

Engine& Engine::operator=(Engine&&) noexcept = default;

void Engine::init(std::vector<FontData> loadFonts) {
	impl->fontLoader.preInit();
	for (const auto& font : loadFonts) {
		impl->fontLoader.loadFont(font.data, font.size);
	}
	impl->fontLoader.init();
}

void Engine::begin(ID2D1Bitmap* screen) {
	const D2D1_SIZE_U screenSize = screen->GetPixelSize();
	float dpiX = 0.0f;
	float dpiY = 0.0f;
	screen->GetDpi(&dpiX, &dpiY);

	const D2D1_BITMAP_PROPERTIES1 properties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET,
		screen->GetPixelFormat(),
		dpiX,
		dpiY);
	throwIfFailed(
		impl->deviceContext->CreateBitmap(
			screenSize,
			nullptr,
			0,
			properties,
			impl->renderTarget.ReleaseAndGetAddressOf()),
		"Failed to create engine render target");

	impl->deviceContext->BeginDraw();
	impl->deviceContext->SetTarget(impl->renderTarget.Get());
	impl->deviceContext->Clear();

	const D2D1_RECT_F destRectangle(0.0f, 0.0f, static_cast<float>(screenSize.width), static_cast<float>(screenSize.height));
	impl->deviceContext->DrawBitmap(screen, destRectangle);

	throwIfFailed(
		impl->deviceContext->CreateBitmap(
			screenSize,
			nullptr,
			0,
			properties,
			impl->effectScreenBitmap.ReleaseAndGetAddressOf()),
		"Failed to create effect staging bitmap");

	impl->matrix = Matrix();
	impl->deviceContext->SetTransform(impl->matrix.build());
	impl->resourceAllocator.nextFrame();

	throwIfFailed(impl->deviceContext->Flush(), "Failed to flush engine begin state");
}

void Engine::end(ID2D1Image** output) {
	if (output == nullptr) {
		throw std::invalid_argument("Engine::end requires a non-null output pointer.");
	}

	throwIfFailed(impl->deviceContext->EndDraw(), "Failed to end engine draw pass");
	impl->renderTarget.CopyTo(output);
	impl->renderTarget.Reset();
	impl->effectScreenBitmap.Reset();
}

void Engine::flush() {
	throwIfFailed(impl->deviceContext->Flush(), "Failed to flush draw commands");
	copyScreenToEffectBitmap(impl->renderTarget.Get(), impl->effectScreenBitmap.Get());
}

void Engine::clear() {
	impl->deviceContext->Clear();
}

kke::Scale2f Engine::getViewportSize() const {
	const D2D1_SIZE_F size = impl->deviceContext->GetSize();
	return Scale2f(size.width, size.height);
}

kke::Scale2f Engine::getTextSize(
	std::string const& text,
	FontWeight weight,
	std::string const& fontFamily,
	int32_t fontSize) {
	return getTextSize(toWString(text), weight, toWString(fontFamily), fontSize);
}

kke::Scale2f Engine::getTextSize(
	std::wstring const& text,
	FontWeight weight,
	std::wstring const& fontFamily,
	int32_t fontSize) {
	IDWriteTextFormat* format = impl->resourceAllocator.acquireOrCreateTextFormat(fontFamily, fontSize, weight);
	IDWriteTextLayout* layout = impl->resourceAllocator.acquireOrCreateTextLayout(text, format);
	DWRITE_TEXT_METRICS metrics{};
	throwIfFailed(layout->GetMetrics(&metrics), "Failed to query text metrics");
	return Scale2f(metrics.width, metrics.height);
}

void Engine::drawLine(Point2f start, Point2f end, Brush const& brush, float strokeWidth) {
	impl->deviceContext->DrawLine(start.point2f(), end.point2f(), impl->resourceAllocator.acquireOrCreateBrush(brush).Get(), strokeWidth);
}

void Engine::drawTriangle(
	Triangle const& triangle,
	Brush const& brush,
	float strokeWidth) {
	ComPtr<ID2D1Geometry> geometry = impl->resourceAllocator.acquireOrCreateGeometry(triangle);
	impl->deviceContext->DrawGeometry(geometry.Get(), impl->resourceAllocator.acquireOrCreateBrush(brush).Get(), strokeWidth);
}

void Engine::drawRect(Rect const& rect, Brush const& brush, float strokeWidth) {
	impl->deviceContext->DrawRectangle(rect.rectF(), impl->resourceAllocator.acquireOrCreateBrush(brush).Get(), strokeWidth);
}

void Engine::drawRounded(RoundedRect const& rect, Brush const& brush, float strokeWidth) {
	impl->deviceContext->DrawRoundedRectangle(rect.roundedRect(), impl->resourceAllocator.acquireOrCreateBrush(brush).Get(), strokeWidth);
}

void Engine::drawEllipse(Ellipse const& ellipse, Brush const& brush, float strokeWidth) {
	impl->deviceContext->DrawEllipse(ellipse.ellipse(), impl->resourceAllocator.acquireOrCreateBrush(brush).Get(), strokeWidth);
}

void Engine::drawText(
	Point2f const& position,
	std::string const& text,
	FontWeight weight,
	std::string const& fontFamily,
	int32_t fontSize,
	Brush const& brush) {
	drawText(position, toWString(text), weight, toWString(fontFamily), fontSize, brush);
}

void Engine::drawText(
	Point2f const& position,
	std::wstring const& text,
	FontWeight weight,
	std::wstring const& fontFamily,
	int32_t fontSize,
	Brush const& brush) {
	IDWriteTextFormat* format = impl->resourceAllocator.acquireOrCreateTextFormat(fontFamily, fontSize, weight);
	IDWriteTextLayout* layout = impl->resourceAllocator.acquireOrCreateTextLayout(text, format);
	impl->deviceContext->DrawTextLayout(
		position.point2f(),
		layout,
		impl->resourceAllocator.acquireOrCreateBrush(brush).Get(),
		D2D1_DRAW_TEXT_OPTIONS_NONE);
}

void Engine::fillTriangle(
	Triangle const& triangle,
	Brush const& brush) {
	ComPtr<ID2D1Geometry> geometry = impl->resourceAllocator.acquireOrCreateGeometry(triangle);
	impl->deviceContext->FillGeometry(geometry.Get(), impl->resourceAllocator.acquireOrCreateBrush(brush).Get());
}

void Engine::fillRect(Rect const& rect, Brush const& brush) {
	impl->deviceContext->FillRectangle(rect.rectF(), impl->resourceAllocator.acquireOrCreateBrush(brush).Get());
}

void Engine::fillRounded(RoundedRect const& rect, Brush const& brush) {
	impl->deviceContext->FillRoundedRectangle(rect.roundedRect(), impl->resourceAllocator.acquireOrCreateBrush(brush).Get());
}

void Engine::fillEllipse(Ellipse const& ellipse, Brush const& brush) {
	impl->deviceContext->FillEllipse(ellipse.ellipse(), impl->resourceAllocator.acquireOrCreateBrush(brush).Get());
}

void Engine::drawRectShadow(
	Rect const& rect,
	Brush const& brush,
	float deviation,
	bool clipOriginalGeometry) {
	const Point2f offset = impl->shadowDispatcher.computeOffset(rect);
	const ComPtr<ID2D1Image> shadowOutput = impl->resourceAllocator.acquireOrDispatchShadow(rect, brush, deviation, [&](ID2D1Image** output) {
		impl->shadowDispatcher.dispatch(rect, deviation, [&]() {
			fillRect(rect, brush);
		}, output);
	});
	impl->deviceContext->SetTransform(impl->matrix.build());

	if (clipOriginalGeometry) {
		ComPtr<ID2D1Geometry> clipGeometry = impl->resourceAllocator.acquireOrCreateInvertedGeometry(rect);
		impl->deviceContext->PushLayer(
			D2D1::LayerParameters(D2D1::InfiniteRect(), clipGeometry.Get()),
			nullptr);
	}
	impl->deviceContext->DrawImage(shadowOutput.Get(), offset.point2f());
	if (clipOriginalGeometry) {
		impl->deviceContext->PopLayer();
	}
}

void Engine::drawRoundedShadow(
	RoundedRect const& rect,
	Brush const& brush,
	float deviation,
	bool clipOriginalGeometry) {
	const Point2f offset = impl->shadowDispatcher.computeOffset(rect);
	const ComPtr<ID2D1Image> shadowOutput = impl->resourceAllocator.acquireOrDispatchShadow(rect, brush, deviation, [&](ID2D1Image** output) {
		impl->shadowDispatcher.dispatch(rect, deviation, [&]() {
			fillRounded(rect, brush);
		}, output);
	});
	impl->deviceContext->SetTransform(impl->matrix.build());

	if (clipOriginalGeometry) {
		ComPtr<ID2D1Geometry> clipGeometry = impl->resourceAllocator.acquireOrCreateInvertedGeometry(rect);
		impl->deviceContext->PushLayer(
			D2D1::LayerParameters(D2D1::InfiniteRect(), clipGeometry.Get()),
			nullptr);
	}
	impl->deviceContext->DrawImage(shadowOutput.Get(), offset.point2f());
	if (clipOriginalGeometry) {
		impl->deviceContext->PopLayer();
	}
}

void Engine::drawEllipseShadow(
	Ellipse const& ellipse,
	Brush const& brush,
	float deviation,
	bool clipOriginalGeometry) {
	const Rect dimension = {
		ellipse.point.x - ellipse.radius,
		ellipse.point.y - ellipse.radius,
		ellipse.point.x + ellipse.radius,
		ellipse.point.y + ellipse.radius,
	};
	const Point2f offset = impl->shadowDispatcher.computeOffset(dimension);
	const ComPtr<ID2D1Image> shadowOutput = impl->resourceAllocator.acquireOrDispatchShadow(ellipse, brush, deviation, [&](ID2D1Image** output) {
		impl->shadowDispatcher.dispatch(dimension, deviation, [&]() {
			fillEllipse(ellipse, brush);
		}, output);
	});
	impl->deviceContext->SetTransform(impl->matrix.build());

	if (clipOriginalGeometry) {
		ComPtr<ID2D1Geometry> clipGeometry = impl->resourceAllocator.acquireOrCreateInvertedGeometry(ellipse);
		impl->deviceContext->PushLayer(
			D2D1::LayerParameters(D2D1::InfiniteRect(), clipGeometry.Get()),
			nullptr);
	}
	impl->deviceContext->DrawImage(shadowOutput.Get(), offset.point2f());
	if (clipOriginalGeometry) {
		impl->deviceContext->PopLayer();
	}
}

void Engine::drawLineShadow(
	kke::Point2f start,
	kke::Point2f end,
	kke::Brush const& brush,
	float strokeWidth,
	float deviation) {
	const Rect dimension = {
		start.x - 1.0f,
		start.y - 1.0f,
		end.x + 1.0f,
		end.y + 1.0f,
	};

	Hasher hasher;
	hasher.combine(0xFFFFFF);
	hasher.combine(start.x);
	hasher.combine(start.y);
	hasher.combine(end.x);
	hasher.combine(end.y);
	hasher.combine(brush.hash());
	hasher.combine(strokeWidth);
	hasher.combine(deviation);

	const Point2f offset = impl->shadowDispatcher.computeOffset(dimension);
	const ComPtr<ID2D1Image> shadowOutput = impl->resourceAllocator.acquireOrDispatchShadow(dimension, hasher.get(), brush, deviation, [&](ID2D1Image** output) {
		impl->shadowDispatcher.dispatch(dimension, deviation, [&]() {
			drawLine(start, end, brush, strokeWidth);
		}, output);
	});
	impl->deviceContext->SetTransform(impl->matrix.build());
	impl->deviceContext->DrawImage(shadowOutput.Get(), offset.point2f());
}

void Engine::drawTextShadow(
	Point2f const& position,
	std::string const& text,
	FontWeight weight,
	std::string const& fontFamily,
	int32_t fontSize,
	Brush const& brush,
	float deviation) {
	drawTextShadow(position, toWString(text), weight, toWString(fontFamily), fontSize, brush, deviation);
}

void Engine::drawTextShadow(
	Point2f const& position,
	std::wstring const& text,
	FontWeight weight,
	std::wstring const& fontFamily,
	int32_t fontSize,
	Brush const& brush,
	float deviation) {
	IDWriteTextFormat* format = impl->resourceAllocator.acquireOrCreateTextFormat(fontFamily, fontSize, weight);
	IDWriteTextLayout* layout = impl->resourceAllocator.acquireOrCreateTextLayout(text, format);
	DWRITE_TEXT_METRICS metrics{};
	throwIfFailed(layout->GetMetrics(&metrics), "Failed to query text shadow metrics");
	const Rect dimension = {
		position.x,
		position.y,
		position.x + metrics.width,
		position.y + metrics.height,
	};
	const Point2f offset = impl->shadowDispatcher.computeOffset(dimension);
	const ComPtr<ID2D1Image> shadowOutput = impl->resourceAllocator.acquireOrDispatchShadow(dimension, brush, deviation, [&](ID2D1Image** output) {
		impl->shadowDispatcher.dispatch(dimension, deviation, [&]() {
			drawText(position, text, weight, fontFamily, fontSize, brush);
		}, output);
	});
	impl->deviceContext->SetTransform(impl->matrix.build());
	impl->deviceContext->DrawImage(shadowOutput.Get(), offset.point2f());
}

uint64_t Engine::loadTexture(const void* data, size_t size) {
	return impl->textureRepository.load(data, size);
}

void Engine::releaseTexture(uint64_t index) {
	impl->textureRepository.release(index);
}

void Engine::drawImage(ID2D1Image* image, InterpolationMode interpolationMode) {
	impl->deviceContext->DrawImage(
		image,
		nullptr,
		nullptr,
		toD2D1InterpolationMode(interpolationMode),
		D2D1_COMPOSITE_MODE_SOURCE_OVER);
}

void Engine::drawTexture(size_t index, Rect const& dimension, float opacity, InterpolationMode interpolationMode, std::optional<Rect> srcRect) {
	const ComPtr<ID2D1Bitmap> texture = impl->textureRepository.getTexture(index);
	if (!texture) {
		return;
	}
	drawBitmap(texture.Get(), dimension, opacity, interpolationMode, srcRect);
}

void Engine::blur(
	float deviation,
	kke::Geometry* fillGeometry,
	BlurBorderMode borderMode,
	BlurOptimization optimization) {
	if (fillGeometry != nullptr) {
		pushLayer(*fillGeometry);
	}

	std::shared_ptr<BlurEffect> blurEffect = acquireOrCreateEffect<BlurEffect>();
	blurEffect->setDeviation(deviation);
	blurEffect->setBorderMode(borderMode);
	blurEffect->setOptimization(optimization);
	applyEffect(impl->deviceContext, &impl->resourceAllocator, impl->matrix, impl->effectScreenBitmap.Get(), blurEffect);

	if (fillGeometry != nullptr) {
		popLayer();
	}
}

void Engine::blur(
	ID2D1Image* input,
	float deviation,
	kke::Geometry* fillGeometry,
	BlurBorderMode borderMode,
	BlurOptimization optimization) {
	if (fillGeometry != nullptr) {
		pushLayer(*fillGeometry);
	}

	std::shared_ptr<BlurEffect> blurEffect = acquireOrCreateEffect<BlurEffect>();
	blurEffect->setDeviation(deviation);
	blurEffect->setBorderMode(borderMode);
	blurEffect->setOptimization(optimization);
	applyEffect(impl->deviceContext, &impl->resourceAllocator, impl->matrix, input, blurEffect);

	if (fillGeometry != nullptr) {
		popLayer();
	}
}

void Engine::pushTranslate(Point2f const& offset) {
	impl->matrix.pushTranslate(offset);
	impl->deviceContext->SetTransform(impl->matrix.build());
}

void Engine::pushScale(Point2f const& center, Scale2f const& scale) {
	impl->matrix.pushScale(center, scale);
	impl->deviceContext->SetTransform(impl->matrix.build());
}

void Engine::pushRotate(Point2f const& center, float angle) {
	impl->matrix.pushRotate(center, angle);
	impl->deviceContext->SetTransform(impl->matrix.build());
}

void Engine::popTransform() {
	impl->matrix.pop();
	impl->deviceContext->SetTransform(impl->matrix.build());
}

void Engine::pushSurface() {
	RenderSurface* surface = impl->resourceAllocator.acquireOrCreateSurface();
	surface->setLocking(true);
	impl->deviceContext->SetTarget(surface->getRenderTarget());
	impl->deviceContext->Clear();
	impl->surfaceStack.push(surface);
}

void Engine::pushLayer(Geometry const& geometry) {
	impl->deviceContext->PushLayer(
		D2D1::LayerParameters(
			D2D1::InfiniteRect(),
			impl->resourceAllocator.acquireOrCreateGeometry(geometry).Get()),
		nullptr);
}

void Engine::popLayer() {
	impl->deviceContext->PopLayer();
}

void Engine::popSurface(ID2D1Bitmap1** output) {
	if (output == nullptr) {
		throw std::invalid_argument("Engine::popSurface requires a non-null output pointer.");
	}
	if (impl->surfaceStack.empty()) {
		throw std::runtime_error("Engine::popSurface called with an empty surface stack.");
	}

	throwIfFailed(impl->deviceContext->Flush(), "Failed to flush off-screen surface");
	RenderSurface* currentSurface = impl->surfaceStack.top();
	impl->surfaceStack.pop();

	if (impl->surfaceStack.empty()) {
		impl->deviceContext->SetTarget(impl->renderTarget.Get());
	} else {
		impl->deviceContext->SetTarget(impl->surfaceStack.top()->getRenderTarget());
	}

	ID2D1Bitmap1* surfaceTarget = currentSurface->getRenderTarget();
	surfaceTarget->AddRef();
	*output = surfaceTarget;
}

void Engine::drawBitmap(
	ID2D1Bitmap* bitmap,
	Rect const& dimension,
	float opacity,
	InterpolationMode interpolationMode,
	std::optional<Rect> srcRect) {
	const D2D1_RECT_F rect = dimension.rectF();
	D2D1_RECT_F srcRectangle{};
	D2D1_RECT_F* srcRectanglePtr = nullptr;
	if (srcRect) {
		srcRectangle = srcRect->rectF();
		srcRectanglePtr = &srcRectangle;
	}
	impl->deviceContext->DrawBitmap(bitmap, &rect, opacity, toD2D1InterpolationMode(interpolationMode), srcRectanglePtr);
}
}  // namespace kke
