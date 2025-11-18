#include "kke/effect/EffectInstance.hpp"

#include <stdexcept>

void EffectInstance::lock() {
    isLocked = true;
}

void EffectInstance::unlock() {
    isLocked = false;
}

bool EffectInstance::isLocking() const {
    return isLocked;
}

GUID const& EffectInstance::getGUID() const {
    return guid;
}

ComPtr<ID2D1Effect> EffectInstance::getD2D1Effect() {
    return d2d1Effect;
}

EffectInstance::EffectInstance(ID2D1DeviceContext* deviceContext, GUID guid)
	: guid(guid) {
	HRESULT hr = deviceContext->CreateEffect(guid, &d2d1Effect);
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to create D2D1 effect.");
	}
}
