#include <oreik/effect/Effect.hpp>

void oreik::Effect::init(ID2D1DeviceContext* context) {
	// TODO: impl
}

Microsoft::WRL::ComPtr<ID2D1Effect> oreik::Effect::getEffect() {
	return effectInstance;
}
