#pragma once

#include <d2d1_1.h>
#include <wincodec.h>
#include <winnt.h>
#include <wrl/client.h>

using namespace Microsoft::WRL;

class EffectInstance {
	GUID guid;
	ComPtr<ID2D1Effect> d2d1Effect;
	bool isLocked = false;

public:
    void lock();

    void unlock();

    bool isLocking() const;

    GUID const& getGUID() const;

    ComPtr<ID2D1Effect> getD2D1Effect();

    EffectInstance(ID2D1DeviceContext* deviceContext, GUID guid);
};
