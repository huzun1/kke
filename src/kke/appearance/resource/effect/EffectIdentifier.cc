#include "EffectIdentifier.hh"

using namespace kke;

namespace {
constexpr size_t shadowEffectIndex = 0;
constexpr size_t blurEffectIndex = 1;
constexpr size_t directionalBlurEffectIndex = 2;
constexpr size_t colorMatrixEffectIndex = 3;
} // namespace

EffectKind EffectIdentifier::identify(Effect const& effect) {
	switch (effect.index()) {
	case shadowEffectIndex:
		return EffectKind::Shadow;
	case blurEffectIndex:
		return EffectKind::Blur;
	case directionalBlurEffectIndex:
		return EffectKind::DirectionalBlur;
	case colorMatrixEffectIndex:
		return EffectKind::ColorMatrix;
	default:
		return EffectKind::Shadow;
	}
}
