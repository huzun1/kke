#include "D2dTargetSnapshot.hh"

#include <utility>

using namespace kke;

D2dTargetSnapshot::D2dTargetSnapshot(Microsoft::WRL::ComPtr<ID2D1Image> image)
	: image(std::move(image)) {
}

Microsoft::WRL::ComPtr<ID2D1Image> const& D2dTargetSnapshot::getImage() const {
	return image;
}
