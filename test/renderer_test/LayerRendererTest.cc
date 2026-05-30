#include "LayerRendererTest.hh"

#include "kke/appearance/Text.hh"
#include "kke/appearance/resource/GeometryCompose.hh"
#include "kke/appearance/resource/brush/Brush.hh"
#include "kke/appearance/resource/brush/impl/SolidColorBrush.hh"
#include "kke/appearance/resource/font/FontWeight.hh"
#include "kke/appearance/resource/texture/TextureDrawAppearance.hh"
#include "kke/appearance/resource/texture/TextureInterpolation.hh"
#include "kke/appearance/view/LayerMode.hh"
#include "kke/geometry/Geometry.hh"
#include "kke/geometry/curved/Ellipse.hh"
#include "kke/geometry/curved/RoundedRect.hh"
#include "kke/geometry/shapes/Rect.hh"

namespace application {
namespace renderer_test {
void LayerRendererTest::render() {
	engine().fill(
		kke::Rect{{0.0f, 0.0f}, {1280.0f, 720.0f}},
		kke::SolidColorBrush({0.85f, 0.89f, 0.94f, 1.0f}));

	engine().fill(
		kke::Rect{{44.0f, 74.0f}, {1236.0f, 648.0f}},
		kke::SolidColorBrush({0.13f, 0.17f, 0.24f, 0.98f}));

	renderRoundedRectClipDemo();
	renderInversedClipDemo();
	renderComposedGeometryImageDemo();
}

void LayerRendererTest::renderRoundedRectClipDemo() {
	static float marqueeOffset = 0.0f;
	marqueeOffset -= 3.0f;
	if (marqueeOffset < -420.0f) {
		marqueeOffset += 420.0f;
	}

	kke::Brush labelBrush = kke::SolidColorBrush({0.92f, 0.95f, 0.99f, 1.0f});
	kke::Brush subLabelBrush = kke::SolidColorBrush({0.70f, 0.80f, 0.94f, 1.0f});
	kke::Brush panelOutline = kke::SolidColorBrush({0.95f, 0.97f, 0.99f, 0.95f});
	kke::Brush marqueeA = kke::SolidColorBrush({0.24f, 0.84f, 0.52f, 1.0f});
	kke::Brush marqueeB = kke::SolidColorBrush({0.34f, 0.62f, 1.0f, 1.0f});
	kke::Brush marqueeC = kke::SolidColorBrush({1.0f, 0.73f, 0.32f, 1.0f});

	kke::RoundedRect clipRect{
		kke::Rect{{76.0f, 164.0f}, {372.0f, 472.0f}},
		28.0f};

	engine().fill(kke::Text{
					  L"Rounded rect clip",
					  {78.0f, 108.0f},
					  {"Space Grotesk", 24.0f, kke::FontWeight::BOLD}},
				  labelBrush);

	engine().fill(kke::Text{
					  L"flowing text inside the mask",
					  {78.0f, 138.0f},
					  {"Space Grotesk", 18.0f, kke::FontWeight::MEDIUM}},
				  subLabelBrush);

	engine().draw(kke::Geometry{clipRect}, panelOutline, {3.0f});
	engine().pushLayer(kke::Geometry{clipRect});

	for (int row = 0; row < 6; ++row) {
		float rowTop = 182.0f + row * 48.0f;
		float textX = 92.0f + marqueeOffset - (row % 2 == 0 ? 0.0f : 150.0f);
		kke::Brush rowBrush = row % 3 == 0 ? marqueeA : (row % 3 == 1 ? marqueeB : marqueeC);

		engine().fill(kke::Text{
						  L"LAYER CLIP DEMO    LAYER CLIP DEMO    LAYER CLIP DEMO",
						  {textX, rowTop},
						  {"Space Grotesk", 28.0f, kke::FontWeight::BOLD}},
					  rowBrush);
	}

	engine().popLayer();
}

void LayerRendererTest::renderInversedClipDemo() {
	kke::Brush labelBrush = kke::SolidColorBrush({0.92f, 0.95f, 0.99f, 1.0f});
	kke::Brush subLabelBrush = kke::SolidColorBrush({0.70f, 0.80f, 0.94f, 1.0f});
	kke::Brush panelOutline = kke::SolidColorBrush({0.95f, 0.97f, 0.99f, 0.95f});
	kke::Brush invertedFill = kke::SolidColorBrush({0.22f, 0.58f, 1.0f, 0.58f});
	kke::Brush invertedAccent = kke::SolidColorBrush({1.0f, 0.62f, 0.28f, 0.92f});

	kke::RoundedRect clipRect{
		kke::Rect{{492.0f, 164.0f}, {788.0f, 472.0f}},
		28.0f};

	engine().fill(kke::Text{
					  L"Inversed rounded rect",
					  {494.0f, 108.0f},
					  {"Space Grotesk", 24.0f, kke::FontWeight::BOLD}},
				  labelBrush);

	engine().fill(kke::Text{
					  L"everything except the inner window",
					  {494.0f, 138.0f},
					  {"Space Grotesk", 18.0f, kke::FontWeight::MEDIUM}},
				  subLabelBrush);

	engine().draw(kke::Geometry{clipRect}, panelOutline, {3.0f});

	engine().fill(
		kke::Ellipse{{640.0f, 318.0f}, 86.0f},
		invertedAccent);

	engine().fill(
		kke::Rect{{538.0f, 220.0f}, {742.0f, 258.0f}},
		kke::SolidColorBrush({0.94f, 0.96f, 0.99f, 0.26f}));

	engine().fill(
		kke::Rect{{538.0f, 378.0f}, {742.0f, 416.0f}},
		kke::SolidColorBrush({0.94f, 0.96f, 0.99f, 0.26f}));

	engine().pushLayer(kke::Geometry{clipRect}, kke::LayerMode::Inverted);

	engine().fill(
		kke::Rect{{456.0f, 146.0f}, {824.0f, 492.0f}},
		invertedFill);

	engine().fill(
		kke::Ellipse{{566.0f, 236.0f}, 72.0f},
		kke::SolidColorBrush({0.24f, 0.84f, 0.52f, 0.56f}));

	engine().fill(
		kke::Ellipse{{714.0f, 398.0f}, 82.0f},
		kke::SolidColorBrush({1.0f, 0.72f, 0.30f, 0.54f}));

	engine().fill(
		kke::Rect{{506.0f, 300.0f}, {776.0f, 340.0f}},
		kke::SolidColorBrush({0.96f, 0.97f, 0.99f, 0.24f}));

	engine().popLayer();
}

void LayerRendererTest::renderComposedGeometryImageDemo() {
	kke::Brush labelBrush = kke::SolidColorBrush({0.92f, 0.95f, 0.99f, 1.0f});
	kke::Brush subLabelBrush = kke::SolidColorBrush({0.70f, 0.80f, 0.94f, 1.0f});
	kke::Brush imageFrame = kke::SolidColorBrush({0.94f, 0.96f, 0.98f, 0.95f});

	kke::GeometryCompose imageMask;
	imageMask.add(kke::Ellipse{{970.0f, 252.0f}, 72.0f});
	imageMask.add(kke::RoundedRect{kke::Rect{{900.0f, 252.0f}, {1120.0f, 430.0f}}, 34.0f});
	imageMask.add(kke::Rect{{932.0f, 396.0f}, {1104.0f, 494.0f}});

	engine().fill(kke::Text{
					  L"Composed geometry image cutout",
					  {842.0f, 108.0f},
					  {"Space Grotesk", 24.0f, kke::FontWeight::BOLD}},
				  labelBrush);

	engine().fill(kke::Text{
					  L"ellipse + rounded rect + footer",
					  {842.0f, 138.0f},
					  {"Space Grotesk", 18.0f, kke::FontWeight::MEDIUM}},
				  subLabelBrush);

	engine().fill(
		kke::RoundedRect{kke::Rect{{860.0f, 164.0f}, {1180.0f, 500.0f}}, 32.0f},
		kke::SolidColorBrush({0.94f, 0.96f, 0.99f, 0.08f}));

	engine().draw(imageMask, imageFrame, {3.0f});
	engine().pushLayer(imageMask);

	if (encodedTexture()) {
		engine().draw(
			encodedTexture(),
			{{860.0f, 164.0f}, {1180.0f, 500.0f}},
			{0.98f, kke::TextureInterpolation::Linear, kke::Rect{{70.0f, 24.0f}, {360.0f, 250.0f}}});
	} else if (rawTexture()) {
		engine().draw(
			rawTexture(),
			{{860.0f, 164.0f}, {1180.0f, 500.0f}},
			{0.98f, kke::TextureInterpolation::Nearest});
	}

	engine().popLayer();
}
}  // namespace renderer_test
}  // namespace application
