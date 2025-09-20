#include <d2d1_1.h>
#include <wincodec.h>

#include <oreik/effect/Effect.hpp>

void oreik::Effect::setInput(ID2D1Image* input) {
	effectInstance->SetInput(0, input);
}

void oreik::Effect::init(ID2D1DeviceContext* context) {
	context->CreateEffect(effectGuid(), &effectInstance);
}

ID2D1Effect* oreik::Effect::output() {
	return effectInstance.Get();
}
