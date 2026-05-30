#include "EffectSourceRenderer.hh"

#include "kke/appearance/painting/StrokeAppearance.hh"

using namespace kke;

std::shared_ptr<D2dCanvas> EffectSourceRenderer::render(
	D2dEngineContext& context,
	EffectSource const& source,
	EffectSourceAppearance const& sourceAppearance) {
	std::shared_ptr<D2dCanvas> canvas = canvasService.createCanvas(context);
	if (!canvas) {
		return nullptr;
	}

	canvasService.pushCanvas(context, canvas);
	drawSource(context, source, sourceAppearance);
	canvasService.popCanvas(context);
	return canvas;
}

void EffectSourceRenderer::drawSource(
	D2dEngineContext const& context,
	EffectSource const& source,
	EffectSourceAppearance const& sourceAppearance) {
	std::visit([&](auto const& sourceVariant) {
		drawSource(context, sourceVariant, sourceAppearance);
	}, source);
}

void EffectSourceRenderer::drawSource(
	D2dEngineContext const& context,
	Line const& source,
	EffectSourceAppearance const& sourceAppearance) {
	strokeRenderer.draw(context, StrokeSource{source}, sourceAppearance.brush, {sourceAppearance.strokeWidth});
}

void EffectSourceRenderer::drawSource(
	D2dEngineContext const& context,
	Geometry const& source,
	EffectSourceAppearance const& sourceAppearance) {
	if (sourceAppearance.drawMode == EffectSourceDrawMode::Stroke) {
		strokeRenderer.draw(context, StrokeSource{source}, sourceAppearance.brush, {sourceAppearance.strokeWidth});
		return;
	}

	faceRenderer.fill(context, FillSource{source}, sourceAppearance.brush);
}

void EffectSourceRenderer::drawSource(
	D2dEngineContext const& context,
	GeometryCompose const& source,
	EffectSourceAppearance const& sourceAppearance) {
	if (sourceAppearance.drawMode == EffectSourceDrawMode::Stroke) {
		strokeRenderer.draw(context, StrokeSource{source}, sourceAppearance.brush, {sourceAppearance.strokeWidth});
		return;
	}

	faceRenderer.fill(context, FillSource{source}, sourceAppearance.brush);
}

void EffectSourceRenderer::drawSource(
	D2dEngineContext const& context,
	Text const& source,
	EffectSourceAppearance const& sourceAppearance) {
	faceRenderer.fill(context, FillSource{source}, sourceAppearance.brush);
}

void EffectSourceRenderer::drawSource(
	D2dEngineContext const& context,
	std::shared_ptr<Canvas> source,
	EffectSourceAppearance const& sourceAppearance) {
	(void)sourceAppearance;

	std::shared_ptr<D2dCanvas> d2dCanvas = std::dynamic_pointer_cast<D2dCanvas>(source);
	if (!d2dCanvas) {
		return;
	}

	if (!d2dCanvas->close()) {
		return;
	}

	context.getD2dContext()->getDeviceContext()->DrawImage(d2dCanvas->getCommandList().Get());
}
