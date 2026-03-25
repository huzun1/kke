#pragma once

#include <d2d1_1.h>
#include <wrl/client.h>
#include <memory>

namespace kke {
class RenderSurface {
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> renderTarget;
	bool locking = false;

public:
	RenderSurface(Microsoft::WRL::ComPtr<ID2D1Bitmap1> renderTarget);

	void setLocking(bool locking);

	bool isLocking() const;

	ID2D1Bitmap1* getRenderTarget() const;

	static std::shared_ptr<RenderSurface> createSurface(ID2D1DeviceContext* context);
};
};	// namespace kke
