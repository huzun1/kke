#include "D2d1Context.hh"

using namespace kke;

D2d1Context::D2d1Context(ID2D1Factory* factory, ID2D1DeviceContext* deviceContext)
	: factory(factory), deviceContext(deviceContext) {
}

ID2D1Factory* D2d1Context::getFactory() const {
	return factory;
}

ID2D1DeviceContext* D2d1Context::getDeviceContext() const {
    return deviceContext;
}
