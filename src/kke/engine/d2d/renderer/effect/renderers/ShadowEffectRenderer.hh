#pragma once

#include "kke/appearance/resource/effect/shadow/ShadowEffect.hh"
#include "kke/engine/d2d/context/D2dEngineContext.hh"

namespace kke {
class ShadowEffectRenderer {
public:
	Microsoft::WRL::ComPtr<ID2D1Image> render(
		D2dEngineContext& context,
		Microsoft::WRL::ComPtr<ID2D1Image> sourceImage,
		ShadowEffect const& effect) const;

private:
	Microsoft::WRL::ComPtr<ID2D1Effect> createShadowEffect(
		ID2D1DeviceContext* deviceContext,
		Microsoft::WRL::ComPtr<ID2D1Image> sourceImage,
		ShadowEffect const& effect) const;

	Microsoft::WRL::ComPtr<ID2D1Effect> createOffsetEffect(
		ID2D1DeviceContext* deviceContext,
		ID2D1Effect* shadowEffect,
		ShadowEffect const& effect) const;

	Microsoft::WRL::ComPtr<ID2D1Image> createOuterShadowImage(
		ID2D1DeviceContext* deviceContext,
		ID2D1Effect* offsetEffect,
		Microsoft::WRL::ComPtr<ID2D1Image> sourceImage) const;
};
}	// namespace kke
