#pragma once

#include <d2d1_1.h>
#include <winnt.h>
#include <wrl/client.h>

namespace oreik {
class Effect {
	Microsoft::WRL::ComPtr<ID2D1Effect> effectInstance = nullptr;

public:
	virtual ~Effect() = default;

	virtual GUID effectGuid() const = 0;

	virtual void setProperties(ID2D1Effect* effect) const = 0;

	void init(ID2D1DeviceContext* context);

	Microsoft::WRL::ComPtr<ID2D1Effect> getEffect();
};
};	// namespace oreik
