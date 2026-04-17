#pragma once

#include "kke/engine/d2d/d2d1_headers.hh"

namespace kke {
class D2dContext {
	ID2D1Factory* factory;
	ID2D1DeviceContext* deviceContext;

public:
	D2dContext(ID2D1Factory* factory, ID2D1DeviceContext* deviceContext);

	ID2D1Factory* getFactory() const;

	ID2D1DeviceContext* getDeviceContext() const;
};
};	// namespace kke
