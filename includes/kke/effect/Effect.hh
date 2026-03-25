#pragma once

#include <d2d1_1.h>
#include <wincodec.h>
#include <winnt.h>
#include <wrl/client.h>

namespace kke {
class Effect {
public:
	virtual ~Effect() = default;

	virtual GUID effectGuid() const = 0;

	virtual void setProperties(Microsoft::WRL::ComPtr<ID2D1Effect> effectInstance) const = 0;
};
};	// namespace kke
