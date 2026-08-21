#include "D2dRasterSurface.hh"

#include <utility>

using namespace kke;

D2dRasterSurface::D2dRasterSurface(Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap)
	: bitmap(std::move(bitmap)) {
}

Microsoft::WRL::ComPtr<ID2D1Bitmap1> const& D2dRasterSurface::getBitmap() const {
	return bitmap;
}
