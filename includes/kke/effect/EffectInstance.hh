#pragma once

#include <d2d1_1.h>
#include <wincodec.h>
#include <winnt.h>
#include <wrl/client.h>

class EffectInstance {
	GUID guid;
	Microsoft::WRL::ComPtr<ID2D1Effect> d2d1Effect;
	bool isLocked = false;

public:
	void lock();

	void unlock();

	bool isLocking() const;

	GUID const& getGUID() const;

	Microsoft::WRL::ComPtr<ID2D1Effect> getD2D1Effect();

	EffectInstance(ID2D1DeviceContext* deviceContext, GUID guid);
};
