#include "D2dEffectPool.hh"

using namespace kke;
using Microsoft::WRL::ComPtr;

void D2dEffectPool::beginFrame(ID2D1DeviceContext* deviceContext) {
	if (activeDeviceContext != deviceContext) {
		effectTypes.clear();
		activeDeviceContext = deviceContext;
	}
	for (EffectTypeEntry& effectType : effectTypes) {
		for (ComPtr<ID2D1Effect> const& effect : effectType.effects) {
			UINT32 inputCount = effect->GetInputCount();
			for (UINT32 inputIndex = 0; inputIndex < inputCount; ++inputIndex) {
				effect->SetInput(inputIndex, nullptr);
			}
		}
		effectType.nextEffectIndex = 0;
	}
}

ComPtr<ID2D1Effect>
D2dEffectPool::acquire(ID2D1DeviceContext* deviceContext, CLSID const& effectId) {
	if (deviceContext == nullptr) {
		return nullptr;
	}
	if (activeDeviceContext != deviceContext) {
		beginFrame(deviceContext);
	}

	EffectTypeEntry& effectType = resolveEffectType(effectId);
	if (effectType.nextEffectIndex < effectType.effects.size()) {
		return effectType.effects[effectType.nextEffectIndex++];
	}

	ComPtr<ID2D1Effect> effect;
	HRESULT result = deviceContext->CreateEffect(effectId, &effect);
	if (FAILED(result) || effect == nullptr) {
		return nullptr;
	}
	effectType.effects.push_back(effect);
	++effectType.nextEffectIndex;
	return effect;
}

D2dEffectPool::EffectTypeEntry& D2dEffectPool::resolveEffectType(CLSID const& effectId) {
	for (EffectTypeEntry& effectType : effectTypes) {
		if (InlineIsEqualGUID(effectType.effectId, effectId)) {
			return effectType;
		}
	}
	effectTypes.push_back({.effectId = effectId});
	return effectTypes.back();
}
