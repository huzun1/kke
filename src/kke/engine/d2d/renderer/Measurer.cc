#include "Measurer.hh"

using namespace kke;

Scale Measurer::getViewportSize(D2dEngineContext const& context) {
	D2D1_SIZE_F viewportSize = context.getViewportSize();
	return {viewportSize.width, viewportSize.height};
}

Scale Measurer::measureTextSize(D2dEngineContext const& context, Text const& text) {
	return context.getResourceProviders()->getFontProvider()->measureTextSize(text);
}
