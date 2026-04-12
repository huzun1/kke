#pragma once

#include "kke/appearance/effect/Effect.hh"
#include "kke/appearance/text/Text.hh"

#include "kke/appearance/painting/StrokeAppearance.hh"
#include "kke/engine/EngineInterface.hh"
#include "kke/engine/d2d1/renderer/painting/FaceRenderer.hh"
#include "kke/engine/d2d1/renderer/painting/StrokeRenderer.hh"

namespace kke {
class D2d1Engine : public EngineInterface {
	FaceRenderer faceRenderer;
	StrokeRenderer strokeRenderer;

public:
	void beginDraw() override;

	void endDraw() override;

	void pushTranslate(Translate const& translate) override;

	void pushScale(Scale const& scale) override;

	void pushRotate(Rotate const& rotate) override;

	void popTranslate() override;

	void popScale() override;

	void popRotate() override;

	std::shared_ptr<Canvas> createCanvas(std::optional<Scale2f> scale = std::nullopt) override;

	void pushCanvas(std::shared_ptr<Canvas> canvas) override;

	void popCanvas() override;

	void draw(std::shared_ptr<Canvas> canvas) override;

	kke::Scale2f getViewportSize() override;

	kke::Scale2f measureTextSize(
		std::string_view text,
		kke::FontAppearance const& appearance) override;

	kke::Scale2f measureTextSize(
		std::wstring_view text,
		kke::FontAppearance const& appearance) override;

	void draw(
		Line const& line,
		Brush const& brush,
		StrokeAppearance const& appearance) override;

	void draw(
		Triangle const& triangle,
		Brush const& brush,
		StrokeAppearance const& appearance) override;

	void draw(
		Rect const& rect,
		Brush const& brush,
		StrokeAppearance const& appearance) override;

	void draw(
		RoundedRect const& roundedRect,
		Brush const& brush,
		StrokeAppearance const& appearance) override;

	void fill(
		Triangle const& triangle,
		Brush const& brush) override;

	void fill(
		Rect const& rect,
		Brush const& brush) override;

	void fill(
		RoundedRect const& roundedRect,
		Brush const& brush) override;

	void fill(
		Text const& text,
		Brush const& brush) override;

	void renderEffect(
		Line const& line,
		Effect const& effect) override;

	void renderEffect(
		Triangle const& triangle,
		Effect const& effect) override;

	void renderEffect(
		Rect const& rect,
		Effect const& effect) override;

	void renderEffect(
		RoundedRect const& roundedRect,
		Effect const& effect) override;

	void renderEffect(
		Text const& text,
		Effect const& effect) override;

	void renderEffect(
		std::shared_ptr<Canvas> canvas,
		Effect const& effect) override;
};
};	// namespace kke
