#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1helper.h>
#include <dcommon.h>
#include <wincodec.h>
#include <wrl/client.h>

#include <cstdint>
#include <memory>
#include <oreik/Engine.hpp>

#include "oreik/RenderSurface.hpp"
#include "oreik/ResourceAllocator.hpp"
#include "oreik/TextureRepository.hpp"
#include "oreik/common/Point.hpp"
#include "oreik/common/Scale.hpp"
#include "oreik/common/geometry/Rect.hpp"
#include "oreik/effect/EffectContainer.hpp"
#include "oreik/effect/impl/BlurEffect.hpp"
#include "oreik/transform/Matrix.hpp"

oreik::Engine::Engine(ID2D1DeviceContext* deviceContext)
	: deviceContext(deviceContext), textureRepository(deviceContext), resourceAllocator(deviceContext), effectContainer(deviceContext), shadowDispatcher(deviceContext, &effectContainer) {
}

oreik::Engine::~Engine() {
	printf("releasing");
}

void oreik::Engine::begin(ID2D1Bitmap* screen) {
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
	D2D1_POINT_2U destPoint(0, 0);
	D2D1_RECT_U srcRectangle(0, 0, screenSize.width, screenSize.height);

	deviceContext->BeginDraw();
	deviceContext->SetTarget(renderTarget);
	deviceContext->Clear();

	renderTarget->CopyFromBitmap(&destPoint, screen, &srcRectangle);

	// Create a temp bitmap
	deviceContext->CreateBitmap(
		screenSize,
		nullptr,
		0,
		properties,
		&effectScreenBitmap);

	// Reset the matrix
	matrix = oreik::Matrix();
	deviceContext->SetTransform(matrix.build());
}

void oreik::Engine::end(ID2D1Image** output) {
	deviceContext->EndDraw();
	*output = renderTarget;
	effectScreenBitmap->Release();
}

void oreik::Engine::clear() {
	deviceContext->Clear();
}

void oreik::Engine::drawLine(oreik::Point2f start, oreik::Point2f end, oreik::Brush const& brush, float strokeWidth) {
	deviceContext->DrawLine(start.point2f(), end.point2f(), resourceAllocator.aquireOrCreateBrush(brush).Get(), strokeWidth);
}

void oreik::Engine::drawRect(oreik::Rect const& rect, oreik::Brush const& brush, float strokeWidth) {
	deviceContext->DrawRectangle(rect.rectF(), resourceAllocator.aquireOrCreateBrush(brush).Get(), strokeWidth);
}

void oreik::Engine::drawRounded(oreik::RoundedRect const& rect, oreik::Brush const& brush, float strokeWidth) {
	deviceContext->DrawRoundedRectangle(rect.roundedRect(), resourceAllocator.aquireOrCreateBrush(brush).Get(), strokeWidth);
}

void oreik::Engine::drawEllipse(oreik::Ellipse const& ellipse, oreik::Brush const& brush, float strokeWidth) {
	deviceContext->DrawEllipse(ellipse.ellipse(), resourceAllocator.aquireOrCreateBrush(brush).Get(), strokeWidth);
}

void oreik::Engine::fillRect(oreik::Rect const& rect, oreik::Brush const& brush) {
	deviceContext->FillRectangle(rect.rectF(), resourceAllocator.aquireOrCreateBrush(brush).Get());
}

void oreik::Engine::fillRounded(oreik::RoundedRect const& rect, oreik::Brush const& brush) {
	deviceContext->FillRoundedRectangle(rect.roundedRect(), resourceAllocator.aquireOrCreateBrush(brush).Get());
}

void oreik::Engine::fillEllipse(oreik::Ellipse const& ellipse, oreik::Brush const& brush) {
	deviceContext->FillEllipse(ellipse.ellipse(), resourceAllocator.aquireOrCreateBrush(brush).Get());
}

void oreik::Engine::drawRectShadow(
	oreik::Rect const& rect,
	oreik::Brush const& brush,
	float deviation) {
	oreik::Point2f offset = shadowDispatcher.computeOffset(rect);
	Microsoft::WRL::ComPtr<ID2D1Image> shadowOutput = resourceAllocator.aquireOrDispatchShadow(rect, brush, deviation, [&](ID2D1Image** output) {
		shadowDispatcher.dispatch(rect, deviation, [&](oreik::Point2f const& start) {
			fillRect(rect, brush);
		}, output);
	});

	deviceContext->SetTransform(matrix.build());
	deviceContext->DrawImage(shadowOutput.Get(), offset.point2f());
}

uint64_t oreik::Engine::loadTexture(const void* data, size_t size) {
	return textureRepository.load(data, size);
}

void oreik::Engine::drawImage(ID2D1Image* image, oreik::InterpolationMode interpolationMode) {
	deviceContext->DrawImage(
		image,
		nullptr,
		nullptr,
		toD2D1InterpolationMode(interpolationMode),
		D2D1_COMPOSITE_MODE_SOURCE_OVER);
}

void oreik::Engine::drawTexture(size_t index, oreik::Rect const& dimension, float opacity, oreik::InterpolationMode interpolationMode, std::optional<oreik::Rect> srcRect) {
	Microsoft::WRL::ComPtr<ID2D1Bitmap> texture = textureRepository.fetchTexture(index);
	if (!texture) {
		return;
	}
	drawBitmap(texture.Get(), dimension, opacity, interpolationMode, srcRect);
}

void oreik::Engine::blur(
	float deviation,
	oreik::BlurBorderMode borderMode,
	oreik::BlurOptimization optimization) {
	std::shared_ptr<BlurEffect> blurEffect = effectContainer.acquireOrCreateEffect<BlurEffect>();
	blurEffect->setDeviation(deviation);
	blurEffect->setBorderMode(borderMode);
	blurEffect->setOptimization(optimization);
	effect(blurEffect);
}

void oreik::Engine::effect(ID2D1Image* image, std::shared_ptr<Effect> effect) {
	effect->setInput(image);
	effect->setProperties();
	deviceContext->DrawImage(effect->output());
}

void oreik::Engine::effect(std::shared_ptr<Effect> effect) {
	// Copy the render target to the temp buffer
	D2D1_SIZE_U pixelSize = renderTarget->GetPixelSize();
	D2D1_POINT_2U destPoint(0, 0);
	D2D1_RECT_U srcRectangle(0, 0, pixelSize.width, pixelSize.height);
	deviceContext->Flush();	 // To apply current commands to the render target
	effectScreenBitmap->CopyFromBitmap(&destPoint, renderTarget, &srcRectangle);

	this->effect(effectScreenBitmap, effect);
}

void oreik::Engine::pushScale(oreik::Point2f const& center, oreik::Scale2f const& scale) {
	matrix.pushScale(center, scale);
	deviceContext->SetTransform(matrix.build());
}

void oreik::Engine::pushRotate(oreik::Point2f const& center, float angle) {
	matrix.pushRotate(center, angle);
	deviceContext->SetTransform(matrix.build());
}

void oreik::Engine::popTransform() {
	matrix.pop();
	deviceContext->SetTransform(matrix.build());
}

void oreik::Engine::pushSurface() {
	RenderSurface* surface = resourceAllocator.aquireOrCreateSurface(deviceContext);
	surface->setLocking(true);
	deviceContext->SetTarget(surface->getRenderTarget());

	surfaceStack.push(surface);
}

void oreik::Engine::popSurface(ID2D1Bitmap1** output) {
	deviceContext->Flush();	 // To apply current commands to the render target

	RenderSurface* currentSurface = surfaceStack.top();
	currentSurface->setLocking(false);
	surfaceStack.pop();

	// Restore the render target
	if (surfaceStack.empty()) {
		deviceContext->SetTarget(renderTarget);
	} else {
		deviceContext->SetTarget(surfaceStack.top()->getRenderTarget());
	}

	*output = currentSurface->getRenderTarget();
}

void oreik::Engine::drawBitmap(
	ID2D1Bitmap* bitmap,
	oreik::Rect const& dimension,
	float opacity,
	oreik::InterpolationMode interpolationMode,
	std::optional<oreik::Rect> srcRect) {
	D2D1_RECT_F rect = dimension.rectF();
	D2D1_RECT_F srcRectangle;
	D2D1_RECT_F* srcRectanglePtr = nullptr;
	if (srcRect) {
		srcRectangle = srcRect->rectF();
		srcRectanglePtr = &srcRectangle;
	}
	deviceContext->DrawBitmap(bitmap, &rect, opacity, toD2D1InterpolationMode(interpolationMode), srcRectanglePtr);
}

D2D1_INTERPOLATION_MODE oreik::Engine::toD2D1InterpolationMode(oreik::InterpolationMode mode) {
	switch (mode) {
		case oreik::InterpolationMode::NEAREST:
			return D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
		case oreik::InterpolationMode::LINEAR:
			return D2D1_INTERPOLATION_MODE_LINEAR;
		case oreik::InterpolationMode::CUBIC:
			return D2D1_INTERPOLATION_MODE_CUBIC;
		case oreik::InterpolationMode::MULTI_SAMPLE_LINEAR:
			return D2D1_INTERPOLATION_MODE_MULTI_SAMPLE_LINEAR;
		case oreik::InterpolationMode::ANISOTROPIC:
			return D2D1_INTERPOLATION_MODE_ANISOTROPIC;
		case oreik::InterpolationMode::HIGH_QUALITY_CUBIC:
			return D2D1_INTERPOLATION_MODE_HIGH_QUALITY_CUBIC;
		default:
			return D2D1_INTERPOLATION_MODE_FORCE_DWORD;
	}
}
