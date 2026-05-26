#include "D2dCanvas.hh"

using namespace kke;

D2dCanvas::D2dCanvas(ComPtr<ID2D1CommandList> commandList)
	: commandList(commandList) {
}

ComPtr<ID2D1CommandList> D2dCanvas::getCommandList() const {
    return commandList;
}

bool D2dCanvas::closed() const {
	return isClosed;
}

bool D2dCanvas::close() {
	if (isClosed) {
		return true;
	}

	HRESULT result = commandList->Close();
	if (FAILED(result)) {
		return false;
	}

	isClosed = true;
	return true;
}
