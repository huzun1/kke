#pragma once

#include <d2d1_1.h>
#include <wrl/client.h>

#include "kke/appearance/resource/Canvas.hh"

using namespace Microsoft::WRL;

namespace kke {
class D2dCanvas : public Canvas {
	ComPtr<ID2D1CommandList> commandList;
	bool isClosed = false;

  public:
	D2dCanvas(ComPtr<ID2D1CommandList> commandList);

	ComPtr<ID2D1CommandList> getCommandList() const;

	bool closed() const;

	bool close();
};
}; // namespace kke
