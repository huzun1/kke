#include "D2dCanvas.hh"

using namespace kke;

D2dCanvas::D2dCanvas(ComPtr<ID2D1CommandList> commandList)
	: commandList(commandList) {
}

ComPtr<ID2D1CommandList> D2dCanvas::getCommandList() const {
    return commandList;
}
