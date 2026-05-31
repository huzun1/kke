#pragma once

#include <memory>

#include "kke/appearance/resource/effect/EffectSourceAppearance.hh"
#include "kke/engine/Sources.hh"
#include "kke/engine/d2d/context/D2dEngineContext.hh"
#include "kke/engine/d2d/renderer/canvas/CanvasService.hh"
#include "kke/engine/d2d/renderer/painting/FaceRenderer.hh"
#include "kke/engine/d2d/renderer/painting/StrokeRenderer.hh"
#include "kke/engine/d2d/resource/canvas/D2dCanvas.hh"

namespace kke {
class EffectSourceRenderer {
	CanvasService canvasService;
	FaceRenderer faceRenderer;
	StrokeRenderer strokeRenderer;

  public:
	std::shared_ptr<D2dCanvas> render(
		D2dEngineContext& context,
		EffectSource const& source,
		EffectSourceAppearance const& sourceAppearance
	);

  private:
	void drawSource(
		D2dEngineContext const& context,
		EffectSource const& source,
		EffectSourceAppearance const& sourceAppearance
	);

	void drawSource(
		D2dEngineContext const& context,
		Line const& source,
		EffectSourceAppearance const& sourceAppearance
	);

	void drawSource(
		D2dEngineContext const& context,
		Geometry const& source,
		EffectSourceAppearance const& sourceAppearance
	);

	void drawSource(
		D2dEngineContext const& context,
		GeometryCompose const& source,
		EffectSourceAppearance const& sourceAppearance
	);

	void drawSource(
		D2dEngineContext const& context,
		Text const& source,
		EffectSourceAppearance const& sourceAppearance
	);

	void drawSource(
		D2dEngineContext const& context,
		std::shared_ptr<Canvas> source,
		EffectSourceAppearance const& sourceAppearance
	);
};
} // namespace kke
