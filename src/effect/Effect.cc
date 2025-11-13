#include <d2d1_1.h>
#include <wincodec.h>

#include <kke/effect/Effect.hpp>

void kke::Effect::setInput(ID2D1Image* input) {
	effectInstance->SetInput(0, input);
}

void kke::Effect::init(ID2D1DeviceContext* context) {
	context->CreateEffect(effectGuid(), &effectInstance);
}

ID2D1Effect* kke::Effect::output() {
	return effectInstance.Get();
}
