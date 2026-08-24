#pragma once

#include <cstddef>
#include <vector>

#include <d2d1_1.h>
#include <wrl/client.h>

namespace kke {
class D2dEffectPool {
	struct EffectTypeEntry {
		CLSID effectId;
		std::vector<Microsoft::WRL::ComPtr<ID2D1Effect>> effects;
		size_t nextEffectIndex = 0;
	};

	ID2D1DeviceContext* activeDeviceContext = nullptr;
	std::vector<EffectTypeEntry> effectTypes;

  public:
	void beginFrame(ID2D1DeviceContext* deviceContext);

	Microsoft::WRL::ComPtr<ID2D1Effect>
	acquire(ID2D1DeviceContext* deviceContext, CLSID const& effectId);

  private:
	EffectTypeEntry& resolveEffectType(CLSID const& effectId);
};
} // namespace kke
