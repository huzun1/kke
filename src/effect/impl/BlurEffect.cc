#include <d2d1.h>
#include <d2d1effects.h>

#include <kke/effect/impl/BlurEffect.hpp>

GUID kke::BlurEffect::effectGuid() const {
	return CLSID_D2D1GaussianBlur;
}

void kke::BlurEffect::setProperties(ComPtr<ID2D1Effect> effectInstance) const {
	effectInstance->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, deviation);
	switch (optimization) {
		case kke::BlurOptimization::SPEED:
			effectInstance->SetValue(D2D1_GAUSSIANBLUR_PROP_OPTIMIZATION, D2D1_GAUSSIANBLUR_OPTIMIZATION_SPEED);
			break;
		case kke::BlurOptimization::BALANCED:
			effectInstance->SetValue(D2D1_GAUSSIANBLUR_PROP_OPTIMIZATION, D2D1_GAUSSIANBLUR_OPTIMIZATION_BALANCED);
			break;
		case kke::BlurOptimization::QUALITY:
			effectInstance->SetValue(D2D1_GAUSSIANBLUR_PROP_OPTIMIZATION, D2D1_GAUSSIANBLUR_OPTIMIZATION_QUALITY);
			break;
	}
	switch (borderMode) {
		case kke::BlurBorderMode::HARD:
			effectInstance->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_HARD);
			break;
		case kke::BlurBorderMode::SOFT:
			effectInstance->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_SOFT);
			break;
	}
}
