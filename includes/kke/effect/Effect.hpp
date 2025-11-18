#pragma once

#include <d2d1_1.h>
#include <wincodec.h>
#include <winnt.h>
#include <wrl/client.h>

using namespace Microsoft::WRL;

namespace kke {
class Effect {

public:
	virtual ~Effect() = default;

	virtual GUID effectGuid() const = 0;

	virtual void setProperties(ComPtr<ID2D1Effect> effectInstance) const = 0;
};
};	// namespace kke
