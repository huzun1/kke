#pragma once

#include <d2d1_1.h>
#include <wrl/client.h>

#include "kke/appearance/resource/TargetSnapshot.hh"

namespace kke {
class D2dTargetSnapshot : public TargetSnapshot {
	Microsoft::WRL::ComPtr<ID2D1Image> image;

  public:
	explicit D2dTargetSnapshot(Microsoft::WRL::ComPtr<ID2D1Image> image);

	Microsoft::WRL::ComPtr<ID2D1Image> const& getImage() const;
};
} // namespace kke
