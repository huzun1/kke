#include "PositionIndependentEffectCache.hh"

#include <atomic>

#include "kke/engine/d2d/renderer/effect/cache/PositionIndependentBrush.hh"
#include "kke/engine/d2d/renderer/effect/cache/PositionIndependentEffectSource.hh"
#include "kke/utils/Hasher.hh"

using namespace kke;
using Microsoft::WRL::ComPtr;

namespace {
std::atomic<uint64_t> cacheHits = 0;
std::atomic<uint64_t> cacheMisses = 0;
}

bool PositionIndependentEffectCache::supports(EffectSource const& source) const {
	return !std::holds_alternative<std::shared_ptr<Canvas>>(source);
}

PositionIndependentEffectCache::StatsSnapshot PositionIndependentEffectCache::consumeStats() {
	return {
		cacheHits.exchange(0),
		cacheMisses.exchange(0)
	};
}

PositionIndependentEffectCache::RenderResult PositionIndependentEffectCache::render(
	D2dEngineContext& context,
	EffectSource const& source,
	EffectSourceAppearance const& sourceAppearance,
	uint64_t effectHash,
	std::optional<EffectClipSource> const& clip,
	EffectImageRenderer const& effectRenderer) {
	Point sourceOrigin = PositionIndependentEffectSource::getOrigin(source);
	uint64_t cacheKey = createCacheKey(source, sourceAppearance, effectHash);
	auto cached = cache.find(cacheKey);
	if (cached == cache.end()) {
		recordMiss();
		EffectSource normalizedSource = PositionIndependentEffectSource::normalize(source, sourceOrigin);
		EffectSourceAppearance normalizedAppearance = sourceAppearance;
		normalizedAppearance.brush = PositionIndependentBrush::normalize(sourceAppearance.brush, sourceOrigin);
		SavedDeviceContextState savedState = saveDeviceContextState(context);
		ID2D1DeviceContext* deviceContext = context.getD2dContext()->getDeviceContext();
		deviceContext->SetTransform(D2D1::Matrix3x2F::Identity());

		std::shared_ptr<D2dCanvas> normalizedSourceCanvas = sourceRenderer.render(context, normalizedSource, normalizedAppearance);
		if (!normalizedSourceCanvas) {
			restoreDeviceContextState(context, savedState);
			return {nullptr, {0.0f, 0.0f}};
		}

		CachedEffectResult cachedBitmap = renderToBitmap(context, normalizedSourceCanvas->getCommandList(), effectRenderer);
		restoreDeviceContextState(context, savedState);
		if (!cachedBitmap.bitmap) {
			return {nullptr, {0.0f, 0.0f}};
		}

		cached = cache.emplace(cacheKey, cachedBitmap).first;
	} else {
		recordHit();
	}

	Point drawOffset = sourceOrigin + cached->second.relativeDrawOffset;
	return {
		cached->second.bitmap,
		drawOffset
	};
}

PositionIndependentEffectCache::CachedEffectResult PositionIndependentEffectCache::renderToBitmap(
	D2dEngineContext& context,
	ComPtr<ID2D1Image> sourceImage,
	EffectImageRenderer const& effectRenderer) {
	ComPtr<ID2D1Image> outputImage = effectRenderer(context, sourceImage);
	if (!outputImage) {
		return {nullptr, {0.0f, 0.0f}};
	}

	ID2D1DeviceContext* deviceContext = context.getD2dContext()->getDeviceContext();
	D2D1_RECT_F imageBounds;
	HRESULT boundsResult = deviceContext->GetImageLocalBounds(outputImage.Get(), &imageBounds);
	if (FAILED(boundsResult)) {
		return {nullptr, {0.0f, 0.0f}};
	}

	ComPtr<ID2D1Bitmap1> bitmap = createEffectBitmap(context, outputImage, imageBounds);
	if (!bitmap) {
		return {nullptr, {0.0f, 0.0f}};
	}

	return {
		bitmap,
		{imageBounds.left, imageBounds.top}
	};
}

uint64_t PositionIndependentEffectCache::createCacheKey(
	EffectSource const& source,
	EffectSourceAppearance const& sourceAppearance,
	uint64_t effectHash) const {
	Point origin = PositionIndependentEffectSource::getOrigin(source);

	Hasher hasher;
	hasher.combine(effectHash);
	hasher.combine(static_cast<uint64_t>(sourceAppearance.drawMode));
	hasher.combine(sourceAppearance.strokeWidth);
	hasher.combine(PositionIndependentBrush::hash(sourceAppearance.brush, origin));
	hasher.combine(PositionIndependentEffectSource::hash(source));
	return hasher.get();
}

ComPtr<ID2D1Bitmap1> PositionIndependentEffectCache::createEffectBitmap(
	D2dEngineContext& context,
	ComPtr<ID2D1Image> outputImage,
	D2D1_RECT_F const& imageBounds) {
	ID2D1DeviceContext* deviceContext = context.getD2dContext()->getDeviceContext();

	D2D1_SIZE_U pixelSize{
		static_cast<UINT32>(std::max(1.0f, std::ceil(imageBounds.right - imageBounds.left))),
		static_cast<UINT32>(std::max(1.0f, std::ceil(imageBounds.bottom - imageBounds.top)))
	};

	D2D1_BITMAP_PROPERTIES1 properties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));

	ComPtr<ID2D1Bitmap1> bitmap;
	HRESULT createResult = deviceContext->CreateBitmap(pixelSize, nullptr, 0, &properties, &bitmap);
	if (FAILED(createResult) || !bitmap) {
		return nullptr;
	}

	SavedDeviceContextState savedState = saveDeviceContextState(context);

	deviceContext->SetTarget(bitmap.Get());
	deviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	deviceContext->Clear();
	deviceContext->DrawImage(outputImage.Get(), {0.0f - imageBounds.left, 0.0f - imageBounds.top});
	HRESULT flushResult = deviceContext->Flush();
	restoreDeviceContextState(context, savedState);
	if (FAILED(flushResult)) {
		return nullptr;
	}

	return bitmap;
}

PositionIndependentEffectCache::SavedDeviceContextState PositionIndependentEffectCache::saveDeviceContextState(
	D2dEngineContext const& context) const {
	SavedDeviceContextState state;
	ID2D1DeviceContext* deviceContext = context.getD2dContext()->getDeviceContext();
	deviceContext->GetTarget(&state.target);
	deviceContext->GetTransform(&state.transform);
	return state;
}

void PositionIndependentEffectCache::restoreDeviceContextState(
	D2dEngineContext const& context,
	SavedDeviceContextState const& state) const {
	ID2D1DeviceContext* deviceContext = context.getD2dContext()->getDeviceContext();
	deviceContext->SetTarget(state.target.Get());
	deviceContext->SetTransform(state.transform);
}

void PositionIndependentEffectCache::recordHit() {
	cacheHits.fetch_add(1);
}

void PositionIndependentEffectCache::recordMiss() {
	cacheMisses.fetch_add(1);
}
