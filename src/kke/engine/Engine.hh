#pragma once

#include <d2d1.h>
#include <d2d1_1.h>
#include <wincodec.h>
#include <wrl/client.h>

#include "kke/engine/EngineInterface.hh"

#include "kke/geometry/Point.hh"
#include "kke/renderer/filler/FaceRenderer.hh"
#include "kke/renderer/shadow/ShadowRenderer.hh"
#include "kke/renderer/stroke/StrokeRenderer.hh"

namespace kke {
class Engine : public EngineInterface {
	kke::FaceRenderer faceRenderer;
	kke::ShadowRenderer shadowRenderer;
	kke::StrokeRenderer strokeRenderer;

public:
	void beginDraw() override;

	void endDraw() override;

	kke::Scale2f getViewportSize() const override;

	kke::Scale2f measureTextSize(
		std::string_view text,
		kke::FontAppearance const& appearance) const override;

	
};
};	// namespace kke
