#include "D3d11ContextIsolation.hh"

using Microsoft::WRL::ComPtr;

namespace kke::example::overlay {
bool D3d11ContextIsolation::initialize(ID3D11Device* device) {
	release();

	ComPtr<ID3D11Device1> device1;
	if (device == nullptr || FAILED(device->QueryInterface(IID_PPV_ARGS(&device1)))) {
		return false;
	}

	D3D_FEATURE_LEVEL featureLevel = device->GetFeatureLevel();
	return SUCCEEDED(device1->CreateDeviceContextState(
		D3D11_1_CREATE_DEVICE_CONTEXT_STATE_SINGLETHREADED,
		&featureLevel,
		1,
		D3D11_SDK_VERSION,
		__uuidof(ID3D11Device1),
		nullptr,
		isolatedState.GetAddressOf()
	));
}

void D3d11ContextIsolation::release() {
	isolatedState = nullptr;
}

ComPtr<ID3DDeviceContextState> D3d11ContextIsolation::activate(ID3D11DeviceContext* context) {
	ComPtr<ID3D11DeviceContext1> context1;
	if (!isolatedState || context == nullptr ||
		FAILED(context->QueryInterface(IID_PPV_ARGS(&context1)))) {
		return nullptr;
	}

	ComPtr<ID3DDeviceContextState> previousState;
	context1->SwapDeviceContextState(isolatedState.Get(), previousState.GetAddressOf());
	return previousState;
}

void D3d11ContextIsolation::restore(
	ID3D11DeviceContext* context, ComPtr<ID3DDeviceContextState> const& previousState
) {
	ComPtr<ID3D11DeviceContext1> context1;
	if (!previousState || context == nullptr ||
		FAILED(context->QueryInterface(IID_PPV_ARGS(&context1)))) {
		return;
	}

	context1->SwapDeviceContextState(previousState.Get(), nullptr);
}
} // namespace kke::example::overlay
