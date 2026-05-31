#include "D2dTexture.hh"

using namespace kke;
using Microsoft::WRL::ComPtr;

D2dTexture::D2dTexture(ComPtr<ID2D1Bitmap1> bitmap) : bitmap(bitmap) {
}

ComPtr<ID2D1Bitmap1> D2dTexture::getBitmap() const {
	return bitmap;
}
