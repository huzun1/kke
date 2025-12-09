#include "kke/Engine.hh"

#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1_1helper.h>
#include <d2d1helper.h>
#include <dcommon.h>
#include <wincodec.h>
#include <wrl/client.h>

#include <cstdint>
#include <memory>

#include "kke/RenderSurface.hh"
#include "kke/ResourceAllocator.hh"
#include "kke/TextureRepository.hh"
#include "kke/common/Geometry.hh"
#include "kke/common/Point.hh"
#include "kke/common/Scale.hh"
#include "kke/common/geometry/Rect.hh"
#include "kke/effect/EffectContainer.hh"
#include "kke/effect/EffectInstance.hh"
#include "kke/effect/impl/BlurEffect.hh"
#include "kke/font/FontData.hh"
#include "kke/internal/Hasher.hh"
#include "kke/transform/Matrix.hh"

using namespace kke;

Engine::Engine(ID2D1Factory* factory, ID2D1DeviceContext* deviceContext)
	: factory(factory), deviceContext(deviceContext), textureRepository(deviceContext), resourceAllocator(factory, deviceContext, &fontLoader), effectContainer(deviceContext), shadowDispatcher(deviceContext, &resourceAllocator, &effectContainer) {
}

void Engine::init(std::vector<FontData> loadFonts) {
	fontLoader.preInit();
	for (const auto& font : loadFonts) {
		fontLoader.loadFont(font.data, font.size);
	}
	fontLoader.init();
}

void Engine::begin(ID2D1Bitmap* screen) {
	// Create a bitmap of the same size as the current screen
	// and use it as the target
	D2D1_SIZE_U screenSize = screen->GetPixelSize();
	float dpiX, dpiY;
	screen->GetDpi(&dpiX, &dpiY);

	D2D1_BITMAP_PROPERTIES1 properties = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET, screen->GetPixelFormat(), dpiX, dpiY);
	deviceContext->CreateBitmap(
		screenSize,
		nullptr,
		0,
		properties,
		&renderTarget);

	deviceContext->BeginDraw();
	deviceContext->SetTarget(renderTarget);
	deviceContext->Clear();

	D2D1_RECT_F destRectangle(0, 0, (float)screenSize.width, (float)screenSize.height);
	deviceContext->DrawBitmap(screen, destRectangle);

	// Create a temp bitmap
	deviceContext->CreateBitmap(
		screenSize,
		nullptr,
		0,
		properties,
		&effectScreenBitmap);

	// Reset the matrix
	matrix = Matrix();
	deviceContext->SetTransform(matrix.build());

	resourceAllocator.nextFrame();

	deviceContext->Flush();
}

void Engine::end(ID2D1Image** output) {
	deviceContext->EndDraw();
	*output = renderTarget;
	effectScreenBitmap->Release();
}

void Engine::flush() {
	deviceContext->Flush();
	copyScreenToEffectBitmap();
}

void Engine::clear() {
	deviceContext->Clear();
}

kke::Scale2f Engine::getViewportSize() const {
    D2D1_SIZE_F size = deviceContext->GetSize();
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
	Microsoft::WRL::ComPtr<IDWriteTextFormat> format = resourceAllocator.acquireOrCreateTextFormat(fontFamily, fontSize, weight);
	Microsoft::WRL::ComPtr<IDWriteTextLayout> layout = resourceAllocator.acquireOrCreateTextLayout(text, format.Get());
	DWRITE_TEXT_METRICS metrics;
	layout->GetMetrics(&metrics);
	return Scale2f(metrics.width, metrics.height);
}

void Engine::drawLine(Point2f start, Point2f end, Brush const& brush, float strokeWidth) {
	deviceContext->DrawLine(start.point2f(), end.point2f(), resourceAllocator.acquireOrCreateBrush(brush).Get(), strokeWidth);
}

void Engine::drawTriangle(
	Triangle const& triangle,
	Brush const& brush,
	float strokeWidth) {
	ComPtr<ID2D1Geometry> geometry = resourceAllocator.acquireOrCreateGeometry(triangle);
	deviceContext->DrawGeometry(geometry.Get(), resourceAllocator.acquireOrCreateBrush(brush).Get(), strokeWidth);
}

void Engine::drawRect(Rect const& rect, Brush const& brush, float strokeWidth) {
	deviceContext->DrawRectangle(rect.rectF(), resourceAllocator.acquireOrCreateBrush(brush).Get(), strokeWidth);
}

void Engine::drawRounded(RoundedRect const& rect, Brush const& brush, float strokeWidth) {
	deviceContext->DrawRoundedRectangle(rect.roundedRect(), resourceAllocator.acquireOrCreateBrush(brush).Get(), strokeWidth);
}

void Engine::drawEllipse(Ellipse const& ellipse, Brush const& brush, float strokeWidth) {
	deviceContext->DrawEllipse(ellipse.ellipse(), resourceAllocator.acquireOrCreateBrush(brush).Get(), strokeWidth);
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
	Microsoft::WRL::ComPtr<IDWriteTextFormat> format = resourceAllocator.acquireOrCreateTextFormat(fontFamily, fontSize, weight);
	Microsoft::WRL::ComPtr<IDWriteTextLayout> layout = resourceAllocator.acquireOrCreateTextLayout(text, format.Get());
	D2D1_POINT_2F origin = position.point2f();
	deviceContext->DrawTextLayout(
		origin,
		layout.Get(),
		resourceAllocator.acquireOrCreateBrush(brush).Get(),
		D2D1_DRAW_TEXT_OPTIONS_NONE);
}

void Engine::fillTriangle(
	Triangle const& triangle,
	Brush const& brush) {
	ComPtr<ID2D1Geometry> geometry = resourceAllocator.acquireOrCreateGeometry(triangle);
	deviceContext->FillGeometry(geometry.Get(), resourceAllocator.acquireOrCreateBrush(brush).Get());
}

void Engine::fillRect(Rect const& rect, Brush const& brush) {
	deviceContext->FillRectangle(rect.rectF(), resourceAllocator.acquireOrCreateBrush(brush).Get());
}

void Engine::fillRounded(RoundedRect const& rect, Brush const& brush) {
	deviceContext->FillRoundedRectangle(rect.roundedRect(), resourceAllocator.acquireOrCreateBrush(brush).Get());
}

void Engine::fillEllipse(Ellipse const& ellipse, Brush const& brush) {
	deviceContext->FillEllipse(ellipse.ellipse(), resourceAllocator.acquireOrCreateBrush(brush).Get());
}

void Engine::drawRectShadow(
	Rect const& rect,
	Brush const& brush,
	float deviation,
	bool clipOriginalGeometry) {
	Point2f offset = shadowDispatcher.computeOffset(rect);
	Microsoft::WRL::ComPtr<ID2D1Image> shadowOutput = resourceAllocator.acquireOrDispatchShadow(rect, brush, deviation, [&](ID2D1Image** output) {
		shadowDispatcher.dispatch(rect, deviation, [&]() {
			fillRect(rect, brush);
		}, output);
	});
	deviceContext->SetTransform(matrix.build());

	if (clipOriginalGeometry) {
		ComPtr<ID2D1Geometry> clipGeometry = resourceAllocator.acquireOrCreateInvertedGeometry(rect);
		deviceContext->PushLayer(
			D2D1::LayerParameters(D2D1::InfiniteRect(), clipGeometry.Get()),
			nullptr);
	}
	deviceContext->DrawImage(shadowOutput.Get(), offset.point2f());
	if (clipOriginalGeometry) {
		deviceContext->PopLayer();
	}
}

void Engine::drawRoundedShadow(
	RoundedRect const& rect,
	Brush const& brush,
	float deviation,
	bool clipOriginalGeometry) {
	Point2f offset = shadowDispatcher.computeOffset(rect);
	Microsoft::WRL::ComPtr<ID2D1Image> shadowOutput = resourceAllocator.acquireOrDispatchShadow(rect, brush, deviation, [&](ID2D1Image** output) {
		shadowDispatcher.dispatch(rect, deviation, [&]() {
			fillRounded(rect, brush);
		}, output);
	});
	deviceContext->SetTransform(matrix.build());

	if (clipOriginalGeometry) {
		ComPtr<ID2D1Geometry> clipGeometry = resourceAllocator.acquireOrCreateInvertedGeometry(rect);
		deviceContext->PushLayer(
			D2D1::LayerParameters(D2D1::InfiniteRect(), clipGeometry.Get()),
			nullptr);
	}
	deviceContext->DrawImage(shadowOutput.Get(), offset.point2f());
	if (clipOriginalGeometry) {
		deviceContext->PopLayer();
	}
}

void Engine::drawEllipseShadow(
	Ellipse const& ellipse,
	Brush const& brush,
	float deviation,
	bool clipOriginalGeometry) {
	Rect dimension = {ellipse.point.x - ellipse.radius,
					  ellipse.point.y - ellipse.radius,
					  ellipse.point.x + ellipse.radius,
					  ellipse.point.y + ellipse.radius};
	Point2f offset = shadowDispatcher.computeOffset(dimension);
	Microsoft::WRL::ComPtr<ID2D1Image> shadowOutput = resourceAllocator.acquireOrDispatchShadow(ellipse, brush, deviation, [&](ID2D1Image** output) {
		shadowDispatcher.dispatch(dimension, deviation, [&]() {
			fillEllipse(ellipse, brush);
		}, output);
	});
	deviceContext->SetTransform(matrix.build());

	if (clipOriginalGeometry) {
		ComPtr<ID2D1Geometry> clipGeometry = resourceAllocator.acquireOrCreateInvertedGeometry(ellipse);
		deviceContext->PushLayer(
			D2D1::LayerParameters(D2D1::InfiniteRect(), clipGeometry.Get()),
			nullptr);
	}
	deviceContext->DrawImage(shadowOutput.Get(), offset.point2f());
	if (clipOriginalGeometry) {
		deviceContext->PopLayer();
	}
}

void Engine::drawLineShadow(
	kke::Point2f start,
	kke::Point2f end,
	kke::Brush const& brush,
	float strokeWidth,
	float deviation) {
	Rect dimension = {
		start.x - 1.0f,
		start.y - 1.0f,
		end.x + 1.0f,
		end.y + 1.0f,
	};

	Hasher hasher;
	hasher.combine(0xFFFFFF);  // Unique identifier for line shadows
	hasher.combine(start.x);
	hasher.combine(start.y);
	hasher.combine(end.x);
	hasher.combine(end.y);
	hasher.combine(brush.hash());
	hasher.combine(strokeWidth);
	hasher.combine(deviation);

	Point2f offset = shadowDispatcher.computeOffset(dimension);
	Microsoft::WRL::ComPtr<ID2D1Image> shadowOutput = resourceAllocator.acquireOrDispatchShadow(dimension, hasher.get(), brush, deviation, [&](ID2D1Image** output) {
		shadowDispatcher.dispatch(dimension, deviation, [&]() {
			drawLine(start, end, brush, strokeWidth);
		}, output);
	});
	deviceContext->SetTransform(matrix.build());
	deviceContext->DrawImage(shadowOutput.Get(), offset.point2f());
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
	Microsoft::WRL::ComPtr<IDWriteTextFormat> format = resourceAllocator.acquireOrCreateTextFormat(fontFamily, fontSize, weight);
	Microsoft::WRL::ComPtr<IDWriteTextLayout> layout = resourceAllocator.acquireOrCreateTextLayout(text, format.Get());
	DWRITE_TEXT_METRICS metrics;
	layout->GetMetrics(&metrics);
	Rect dimension = {
		position.x,
		position.y,
		position.x + metrics.width,
		position.y + metrics.height};
	Point2f offset = shadowDispatcher.computeOffset(dimension);
	Microsoft::WRL::ComPtr<ID2D1Image> shadowOutput = resourceAllocator.acquireOrDispatchShadow(dimension, brush, deviation, [&](ID2D1Image** output) {
		shadowDispatcher.dispatch(dimension, deviation, [&]() {
			drawText(position, text, weight, fontFamily, fontSize, brush);
		}, output);
	});
	deviceContext->SetTransform(matrix.build());
	deviceContext->DrawImage(shadowOutput.Get(), offset.point2f());
}

uint64_t Engine::loadTexture(const void* data, size_t size) {
	return textureRepository.load(data, size);
}

void Engine::releaseTexture(uint64_t index) {
	textureRepository.release(index);
}

void Engine::drawImage(ID2D1Image* image, InterpolationMode interpolationMode) {
	deviceContext->DrawImage(
		image,
		nullptr,
		nullptr,
		toD2D1InterpolationMode(interpolationMode),
		D2D1_COMPOSITE_MODE_SOURCE_OVER);
}

void Engine::drawTexture(size_t index, Rect const& dimension, float opacity, InterpolationMode interpolationMode, std::optional<Rect> srcRect) {
	ComPtr<ID2D1Bitmap> texture = textureRepository.getTexture(index);
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
	if (fillGeometry) {
		pushLayer(*fillGeometry);
	}

	std::shared_ptr<BlurEffect> blurEffect = effectContainer.acquireOrCreateEffect<BlurEffect>();
	blurEffect->setDeviation(deviation);
	blurEffect->setBorderMode(borderMode);
	blurEffect->setOptimization(optimization);
	effect(blurEffect);

	if (fillGeometry) {
		popLayer();
	}
}

void Engine::blur(
	ID2D1Image* input,
	float deviation,
	kke::Geometry* fillGeometry,
	BlurBorderMode borderMode,
	BlurOptimization optimization) {
	if (fillGeometry) {
		pushLayer(*fillGeometry);
	}

	std::shared_ptr<BlurEffect> blurEffect = effectContainer.acquireOrCreateEffect<BlurEffect>();
	blurEffect->setDeviation(deviation);
	blurEffect->setBorderMode(borderMode);
	blurEffect->setOptimization(optimization);
	effect(input, blurEffect);

	if (fillGeometry) {
		popLayer();
	}
}

void Engine::pushTranslate(Point2f const& offset) {
	matrix.pushTranslate(offset);
	deviceContext->SetTransform(matrix.build());
}

void Engine::pushScale(Point2f const& center, Scale2f const& scale) {
	matrix.pushScale(center, scale);
	deviceContext->SetTransform(matrix.build());
}

void Engine::pushRotate(Point2f const& center, float angle) {
	matrix.pushRotate(center, angle);
	deviceContext->SetTransform(matrix.build());
}

void Engine::popTransform() {
	matrix.pop();
	deviceContext->SetTransform(matrix.build());
}

void Engine::pushSurface() {
	RenderSurface* surface = resourceAllocator.acquireOrCreateSurface();
	surface->setLocking(true);
	deviceContext->SetTarget(surface->getRenderTarget());
	deviceContext->Clear();
	surfaceStack.push(surface);
}

void Engine::pushLayer(Geometry const& geometry) {
	deviceContext->PushLayer(
		D2D1::LayerParameters(
			D2D1::InfiniteRect(),
			resourceAllocator.acquireOrCreateGeometry(geometry).Get()),
		nullptr);
}

void Engine::popLayer() {
	deviceContext->PopLayer();
}

void Engine::copyScreenToEffectBitmap() {
	// Copy the render target to the temp buffer
	D2D1_SIZE_U pixelSize = renderTarget->GetPixelSize();
	D2D1_POINT_2U destPoint(0, 0);
	D2D1_RECT_U srcRectangle(0, 0, pixelSize.width, pixelSize.height);
	effectScreenBitmap->CopyFromBitmap(&destPoint, renderTarget, &srcRectangle);
}

void Engine::popSurface(ID2D1Bitmap1** output) {
	deviceContext->Flush();	 // To apply current commands to the render target
	RenderSurface* currentSurface = surfaceStack.top();
	surfaceStack.pop();
	// Restore the render target
	if (surfaceStack.empty()) {
		deviceContext->SetTarget(renderTarget);
	} else {
		deviceContext->SetTarget(surfaceStack.top()->getRenderTarget());
	}
	*output = currentSurface->getRenderTarget();
}

void Engine::effect(ID2D1Image* image, std::shared_ptr<Effect> effect) {
	EffectInstance* instance = resourceAllocator.acquireOrCreateEffect(effect);
	instance->lock();

	ComPtr<ID2D1Effect> d2d1Effect = instance->getD2D1Effect();
	d2d1Effect->SetInput(0, image);
	effect->setProperties(d2d1Effect);

	deviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	deviceContext->DrawImage(d2d1Effect.Get());
	deviceContext->SetTransform(matrix.build());
}

void Engine::effect(std::shared_ptr<Effect> effect) {
	this->effect(effectScreenBitmap, effect);
}

void Engine::drawBitmap(
	ID2D1Bitmap* bitmap,
	Rect const& dimension,
	float opacity,
	InterpolationMode interpolationMode,
	std::optional<Rect> srcRect) {
	D2D1_RECT_F rect = dimension.rectF();
	D2D1_RECT_F srcRectangle;
	D2D1_RECT_F* srcRectanglePtr = nullptr;
	if (srcRect) {
		srcRectangle = srcRect->rectF();
		srcRectanglePtr = &srcRectangle;
	}
	deviceContext->DrawBitmap(bitmap, &rect, opacity, toD2D1InterpolationMode(interpolationMode), srcRectanglePtr);
}

std::wstring Engine::toWString(std::string const& str) {
	uint32_t size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	std::wstring result(size - 1, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &result[0], size);
	return result;
}

D2D1_INTERPOLATION_MODE Engine::toD2D1InterpolationMode(InterpolationMode mode) {
	switch (mode) {
		case InterpolationMode::NEAREST:
			return D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
		case InterpolationMode::LINEAR:
			return D2D1_INTERPOLATION_MODE_LINEAR;
		case InterpolationMode::CUBIC:
			return D2D1_INTERPOLATION_MODE_CUBIC;
		case InterpolationMode::MULTI_SAMPLE_LINEAR:
			return D2D1_INTERPOLATION_MODE_MULTI_SAMPLE_LINEAR;
		case InterpolationMode::ANISOTROPIC:
			return D2D1_INTERPOLATION_MODE_ANISOTROPIC;
		case InterpolationMode::HIGH_QUALITY_CUBIC:
			return D2D1_INTERPOLATION_MODE_HIGH_QUALITY_CUBIC;
		default:
			return D2D1_INTERPOLATION_MODE_FORCE_DWORD;
	}
}
