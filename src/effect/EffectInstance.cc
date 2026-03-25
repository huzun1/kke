#include "kke/effect/EffectInstance.hh"

#include "../internal/HResult.hh"

using kke::internal::throwIfFailed;

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

Microsoft::WRL::ComPtr<ID2D1Effect> EffectInstance::getD2D1Effect() {
	return d2d1Effect;
}

EffectInstance::EffectInstance(ID2D1DeviceContext* deviceContext, GUID guid)
	: guid(guid) {
	throwIfFailed(
		deviceContext->CreateEffect(guid, d2d1Effect.GetAddressOf()),
		"Failed to create D2D1 effect");
}
