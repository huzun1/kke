#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d2d1.h>
#include <d2d1_1.h>
#include <wincodec.h>
#include <wrl/client.h>

#include "kke/engine/EngineInterface.hh"
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

	kke::Scale2f getViewportSize() override;

	kke::Scale2f measureTextSize(
		std::string_view text,
		kke::FontAppearance const& appearance) override;

	kke::Scale2f measureTextSize(
		std::wstring_view text,
		kke::FontAppearance const& appearance) override;

	void drawLine(
		Line const& line,
		Brush const& brush,
		StrokeStyle const& style) override;

	void drawTriangle(
		Triangle const& triangle,
		Brush const& brush,
		StrokeStyle const& style) override;

	void drawRect(
		Rect const& rect,
		Brush const& brush,
		StrokeStyle const& style) override;

	void drawRounded(
		RoundedRect const& roundedRect,
		Brush const& brush,
		StrokeStyle const& style) override;

	void fillTriangle(
		Triangle const& triangle,
		Brush const& brush) override;

	void fillRect(
		Rect const& rect,
		Brush const& brush) override;

	void fillRounded(
		RoundedRect const& roundedRect,
		Brush const& brush) override;

	void fillText(
		std::string_view text,
		Point2f const& position,
		Brush const& brush,
		FontAppearance const& fontAppearance) override;

	void fillText(
		std::wstring_view text,
		Point2f const& position,
		Brush const& brush,
		FontAppearance const& fontAppearance) override;

	void renderLineShadow(
		Line const& line,
		Brush const& brush,
		ShadowAppearance const& shadowAppearance) override;

	void renderTriangleShadow(
		Triangle const& triangle,
		Brush const& brush,
		ShadowAppearance const& shadowAppearance) override;

	void renderRectShadow(
		Rect const& rect,
		Brush const& brush,
		ShadowAppearance const& shadowAppearance) override;

	void renderRoundedShadow(
		RoundedRect const& roundedRect,
		Brush const& brush,
		ShadowAppearance const& shadowAppearance) override;

	void renderTextShadow(
		std::string_view text,
		Point2f const& position,
		Brush const& brush,
		FontAppearance const& fontAppearance,
		ShadowAppearance const& shadowAppearance) override;

	void renderTextShadow(
		std::wstring_view text,
		Point2f const& position,
		Brush const& brush,
		FontAppearance const& fontAppearance,
		ShadowAppearance const& shadowAppearance) override;
};
};	// namespace kke
