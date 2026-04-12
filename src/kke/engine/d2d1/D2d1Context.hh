#pragma once

#include "kke/engine/d2d1/d2d1_headers.hh"

namespace kke {
class D2d1Context {
	ID2D1Factory* factory;
	ID2D1DeviceContext* deviceContext;

public:
	D2d1Context(ID2D1Factory* factory, ID2D1DeviceContext* deviceContext);

	ID2D1Factory* getFactory() const;

	ID2D1DeviceContext* getDeviceContext() const;
};
};	// namespace kke
