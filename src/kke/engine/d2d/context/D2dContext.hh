#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d2d1.h>
#include <d2d1_1.h>
#include <unknwnbase.h>
#include <wrl/client.h>

namespace kke {
class D2dContext {
	ID2D1Factory* factory;
	ID2D1DeviceContext* deviceContext;

	Microsoft::WRL::ComPtr<ID2D1CommandList> targetCommandList = nullptr;

  public:
	D2dContext(ID2D1Factory* factory, ID2D1DeviceContext* deviceContext);

	ID2D1Factory* getFactory() const;

	ID2D1DeviceContext* getDeviceContext() const;

	Microsoft::WRL::ComPtr<ID2D1CommandList> getTargetCommandList() const;

	void setTargetCommandList(Microsoft::WRL::ComPtr<ID2D1CommandList> commandList);
};
}; // namespace kke
