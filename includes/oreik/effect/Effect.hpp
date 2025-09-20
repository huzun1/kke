#pragma once

#include <d2d1_1.h>
#include <wincodec.h>
#include <winnt.h>
#include <wrl/client.h>

namespace oreik {
class Effect {
protected:
	Microsoft::WRL::ComPtr<ID2D1Effect> effectInstance = nullptr;

public:
	virtual ~Effect() = default;

	void setInput(ID2D1Image* input);

	virtual GUID effectGuid() const = 0;

	virtual void setProperties() const = 0;

	void init(ID2D1DeviceContext* context);

	void createOutput(ID2D1Image** output);
};
};	// namespace oreik
