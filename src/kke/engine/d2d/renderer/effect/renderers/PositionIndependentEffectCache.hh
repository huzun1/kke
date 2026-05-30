#pragma once

#include <functional>
#include <optional>
#include <unordered_map>

#include "kke/appearance/resource/effect/EffectSourceAppearance.hh"
#include "kke/engine/Sources.hh"
#include "kke/engine/d2d/context/D2dEngineContext.hh"
#include "kke/engine/d2d/renderer/effect/EffectSourceRenderer.hh"

namespace kke {
class PositionIndependentEffectCache {
public:
	struct StatsSnapshot {
		uint64_t hits;
		uint64_t misses;
	};

private:
	struct CachedEffectResult {
		Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap;
		Point relativeDrawOffset;
	};

	struct SavedDeviceContextState {
		Microsoft::WRL::ComPtr<ID2D1Image> target;
		D2D1_MATRIX_3X2_F transform;
	};

	EffectSourceRenderer sourceRenderer;
	std::unordered_map<uint64_t, CachedEffectResult> cache;

public:
	struct RenderResult {
		Microsoft::WRL::ComPtr<ID2D1Image> image;
		Point targetOffset;
	};

	using EffectImageRenderer = std::function<Microsoft::WRL::ComPtr<ID2D1Image>(
		D2dEngineContext&,
		Microsoft::WRL::ComPtr<ID2D1Image>)>;

	bool supports(EffectSource const& source) const;

	static StatsSnapshot consumeStats();

	RenderResult render(
		D2dEngineContext& context,
		EffectSource const& source,
		EffectSourceAppearance const& sourceAppearance,
		uint64_t effectHash,
		std::optional<EffectClipSource> const& clip,
		EffectImageRenderer const& effectRenderer);

private:
	CachedEffectResult renderToBitmap(
		D2dEngineContext& context,
		Microsoft::WRL::ComPtr<ID2D1Image> sourceImage,
		EffectImageRenderer const& effectRenderer);

	uint64_t createCacheKey(
		EffectSource const& source,
		EffectSourceAppearance const& sourceAppearance,
		uint64_t effectHash) const;

	Microsoft::WRL::ComPtr<ID2D1Bitmap1> createEffectBitmap(
		D2dEngineContext& context,
		Microsoft::WRL::ComPtr<ID2D1Image> outputImage,
		D2D1_RECT_F const& imageBounds);

	SavedDeviceContextState saveDeviceContextState(D2dEngineContext const& context) const;

	void restoreDeviceContextState(
		D2dEngineContext const& context,
		SavedDeviceContextState const& state) const;

	static void recordHit();

	static void recordMiss();
};
}	// namespace kke
