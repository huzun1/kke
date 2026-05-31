#include "Measurer.hh"

using namespace kke;

Scale Measurer::getViewportSize(D2dEngineContext const& context) {
	D2D1_SIZE_F viewportSize = context.getViewportSize();
	return {viewportSize.width, viewportSize.height};
}

Scale Measurer::measureTextSize(D2dEngineContext const& context, Text const& text) {
	Microsoft::WRL::ComPtr<IDWriteTextLayout> textLayout =
		context.getResourceProviders()->getFontProvider()->createTextLayout(text);
	if (!textLayout) {
		return {0.0f, 0.0f};
	}

	DWRITE_TEXT_METRICS metrics = {};
	HRESULT result = textLayout->GetMetrics(&metrics);
	if (FAILED(result)) {
		return {0.0f, 0.0f};
	}

	return {metrics.widthIncludingTrailingWhitespace, metrics.height};
}
