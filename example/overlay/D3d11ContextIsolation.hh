#pragma once

#include <d3d11_1.h>
#include <wrl/client.h>

namespace kke::example::overlay {
class D3d11ContextIsolation {
	Microsoft::WRL::ComPtr<ID3DDeviceContextState> isolatedState;

  public:
	bool initialize(ID3D11Device* device);

	void release();

	Microsoft::WRL::ComPtr<ID3DDeviceContextState> activate(ID3D11DeviceContext* context);

	void restore(
		ID3D11DeviceContext* context,
		Microsoft::WRL::ComPtr<ID3DDeviceContextState> const& previousState
	);
};
} // namespace kke::example::overlay
