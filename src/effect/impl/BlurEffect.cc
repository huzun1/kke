#include <d2d1.h>
#include <d2d1effects.h>

#include <oreik/effect/impl/BlurEffect.hpp>

GUID oreik::BlurEffect::effectGuid() const {
	return CLSID_D2D1GaussianBlur;
}

void oreik::BlurEffect::setProperties() const {
	effectInstance->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, deviation);
	switch (optimization) {
		case oreik::BlurOptimization::SPEED:
			effectInstance->SetValue(D2D1_GAUSSIANBLUR_PROP_OPTIMIZATION, D2D1_GAUSSIANBLUR_OPTIMIZATION_SPEED);
			break;
		case oreik::BlurOptimization::BALANCED:
			effectInstance->SetValue(D2D1_GAUSSIANBLUR_PROP_OPTIMIZATION, D2D1_GAUSSIANBLUR_OPTIMIZATION_BALANCED);
			break;
		case oreik::BlurOptimization::QUALITY:
			effectInstance->SetValue(D2D1_GAUSSIANBLUR_PROP_OPTIMIZATION, D2D1_GAUSSIANBLUR_OPTIMIZATION_QUALITY);
			break;
	}
	switch (borderMode) {
		case oreik::BlurBorderMode::HARD:
			effectInstance->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_HARD);
			break;
		case oreik::BlurBorderMode::SOFT:
			effectInstance->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_SOFT);
			break;
	}
}
