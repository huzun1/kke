#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1helper.h>
#include <dcommon.h>
#include <wrl/client.h>

#include <memory>
#include <oreik/Engine.hpp>

#include "oreik/ResourceAllocator.hpp"
#include "oreik/TextureRepository.hpp"
#include "oreik/effect/EffectContainer.hpp"
#include "oreik/effect/impl/BlurEffect.hpp"
#include "oreik/transform/Matrix.hpp"

oreik::Engine::Engine(ID2D1DeviceContext* deviceContext)
	: deviceContext(deviceContext), textureRepository(deviceContext), resourceAllocator(deviceContext), effectContainer(deviceContext) {
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

void oreik::Engine::drawLine(oreik::Point2f start, oreik::Point2f end, oreik::Brush const& brush, float strokeWidth) {
	deviceContext->DrawLine(start.point2f(), end.point2f(), resourceAllocator.aquireOrCreate(brush), strokeWidth);
}

void oreik::Engine::drawRect(oreik::Rect const& rect, oreik::Brush const& brush, float strokeWidth) {
	deviceContext->DrawRectangle(rect.rectF(), resourceAllocator.aquireOrCreate(brush), strokeWidth);
}

void oreik::Engine::drawRounded(oreik::Rect const& rect, float radius, oreik::Brush const& brush, float strokeWidth) {
	D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(rect.rectF(), radius, radius);
	deviceContext->DrawRoundedRectangle(roundedRect, resourceAllocator.aquireOrCreate(brush), strokeWidth);
}

void oreik::Engine::drawEllipse(oreik::Ellipse const& ellipse, oreik::Brush const& brush, float strokeWidth) {
	deviceContext->DrawEllipse(ellipse.ellipse(), resourceAllocator.aquireOrCreate(brush), strokeWidth);
}

void oreik::Engine::fillRect(oreik::Rect const& rect, oreik::Brush const& brush) {
	deviceContext->FillRectangle(rect.rectF(), resourceAllocator.aquireOrCreate(brush));
}

void oreik::Engine::fillRounded(oreik::Rect const& rect, float radius, oreik::Brush const& brush) {
	D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(rect.rectF(), radius, radius);
	deviceContext->FillRoundedRectangle(roundedRect, resourceAllocator.aquireOrCreate(brush));
}

void oreik::Engine::fillEllipse(oreik::Ellipse const& ellipse, oreik::Brush const& brush) {
	deviceContext->FillEllipse(ellipse.ellipse(), resourceAllocator.aquireOrCreate(brush));
}

uint64_t oreik::Engine::loadTexture(const void* data, size_t size) {
	return textureRepository.load(data, size);
}

void oreik::Engine::drawTexture(size_t index, oreik::Rect const& dimension, float opacity, oreik::InterpolationMode interpolationMode, std::optional<oreik::Rect> srcRect) {
	Microsoft::WRL::ComPtr<ID2D1Bitmap> texture = textureRepository.fetchTexture(index);
	if (!texture) {
		return;
	}

	D2D1_INTERPOLATION_MODE mode;
	switch (interpolationMode) {
		case oreik::InterpolationMode::NEAREST:
			mode = D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
			break;
		case oreik::InterpolationMode::LINEAR:
			mode = D2D1_INTERPOLATION_MODE_LINEAR;
			break;
		case oreik::InterpolationMode::CUBIC:
			mode = D2D1_INTERPOLATION_MODE_CUBIC;
			break;
		case oreik::InterpolationMode::MULTI_SAMPLE_LINEAR:
			mode = D2D1_INTERPOLATION_MODE_MULTI_SAMPLE_LINEAR;
			break;
		case oreik::InterpolationMode::ANISOTROPIC:
			mode = D2D1_INTERPOLATION_MODE_ANISOTROPIC;
			break;
		case oreik::InterpolationMode::HIGH_QUALITY_CUBIC:
			mode = D2D1_INTERPOLATION_MODE_HIGH_QUALITY_CUBIC;
			break;
		default:
			mode = D2D1_INTERPOLATION_MODE_FORCE_DWORD;
			break;
	}

	D2D1_RECT_F rect = dimension.rectF();

	D2D1_RECT_F srcRectangle;
	D2D1_RECT_F* srcRectanglePtr = nullptr;
	if (srcRect) {
		srcRectangle = srcRect->rectF();
		srcRectanglePtr = &srcRectangle;
	}

	deviceContext->DrawBitmap(texture.Get(), &rect, opacity, mode, srcRectanglePtr);
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

void oreik::Engine::effect(std::shared_ptr<Effect> effect) {
	// Copy the render target to the temp buffer
	D2D1_SIZE_U pixelSize = renderTarget->GetPixelSize();
	D2D1_POINT_2U destPoint(0, 0);
	D2D1_RECT_U srcRectangle(0, 0, pixelSize.width, pixelSize.height);
	deviceContext->Flush();	 // To apply current commands to the render target
	effectScreenBitmap->CopyFromBitmap(&destPoint, renderTarget, &srcRectangle);

	// Render the effect
	Microsoft::WRL::ComPtr<ID2D1Effect> d2d1Effect = effect->getEffect();
	d2d1Effect->SetInput(0, effectScreenBitmap);
	effect->setProperties(d2d1Effect.Get());
	deviceContext->DrawImage(d2d1Effect.Get());
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
