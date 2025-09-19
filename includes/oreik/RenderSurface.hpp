#pragma once

#include <d2d1_1.h>
#include <wrl/client.h>

namespace oreik {
class RenderSurface {
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> renderTarget;
	bool locking;

public:
	RenderSurface(Microsoft::WRL::ComPtr<ID2D1Bitmap1> renderTarget);

	void setLocking(bool locking);

	bool isLocking() const;

	ID2D1Bitmap1* getRenderTarget();

	static RenderSurface createSurface(ID2D1DeviceContext* context);
};
};	// namespace oreik
