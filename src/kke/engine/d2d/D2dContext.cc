#include "D2dContext.hh"

using namespace kke;

D2dContext::D2dContext(ID2D1Factory* factory, ID2D1DeviceContext* deviceContext)
	: factory(factory), deviceContext(deviceContext) {
}

ID2D1Factory* D2dContext::getFactory() const {
	return factory;
}

ID2D1DeviceContext* D2dContext::getDeviceContext() const {
    return deviceContext;
}

Microsoft::WRL::ComPtr<ID2D1CommandList> D2dContext::getTargetCommandList() const {
	return targetCommandList;
}

void D2dContext::setTargetCommandList(Microsoft::WRL::ComPtr<ID2D1CommandList> commandList) {
	this->targetCommandList = commandList;
}
