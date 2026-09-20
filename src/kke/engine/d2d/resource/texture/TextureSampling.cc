#include "TextureSampling.hh"

using namespace kke;

D2D1_INTERPOLATION_MODE TextureSampling::convert(TextureInterpolation interpolation) {
	switch (interpolation) {
	case TextureInterpolation::Nearest:
		return D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
	case TextureInterpolation::Linear:
		return D2D1_INTERPOLATION_MODE_LINEAR;
	case TextureInterpolation::HighQualityCubic:
		return D2D1_INTERPOLATION_MODE_HIGH_QUALITY_CUBIC;
	}
	return D2D1_INTERPOLATION_MODE_LINEAR;
}
