#include <wincodec.h>

#include <oreik/effect/Effect.hpp>

void oreik::Effect::setInput(ID2D1Image* input) {
	effectInstance->SetInput(0, input);
}

void oreik::Effect::init(ID2D1DeviceContext* context) {
	context->CreateEffect(effectGuid(), &effectInstance);
}

void oreik::Effect::createOutput(ID2D1Image** output) {
	effectInstance->GetOutput(output);
}
