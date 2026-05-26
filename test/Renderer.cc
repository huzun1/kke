#include "Renderer.hh"

#include <cstdio>

#include "kke/appearance/painting/StrokeAppearance.hh"
#include "kke/appearance/resource/brush/Brush.hh"
#include "kke/appearance/resource/brush/impl/SolidColorBrush.hh"
#include "kke/appearance/view/LayerMode.hh"
#include "kke/engine/d2d/context/D2dContext.hh"
#include "kke/geometry/Geometry.hh"
#include "kke/geometry/curved/Ellipse.hh"
#include "kke/geometry/shapes/Rect.hh"

application::Renderer::Renderer(application::D2D1& d2d1)
	: d2d1(d2d1) {
}

void application::Renderer::render() {
	ID2D1DeviceContext* deviceContext = d2d1.getDeviceContext();
	ID2D1Bitmap1* renderTarget = d2d1.getRenderTarget();
	if (!deviceContext || !renderTarget) {
		return;
	}

	kke::D2dContext context(d2d1.getFactory(), deviceContext);

	engine.beginDraw(context, renderTarget);
	engine.clear();
	renderFrame();
	engine.endDraw();
}

void application::Renderer::renderFrame() {
	kke::Brush background = kke::SolidColorBrush({0.08f, 0.10f, 0.12f, 1.0f});
	kke::Brush normalLayerFill = kke::SolidColorBrush({0.20f, 0.80f, 0.46f, 0.9f});
	kke::Brush invertedLayerFill = kke::SolidColorBrush({0.30f, 0.52f, 1.0f, 0.55f});
	kke::Brush outline = kke::SolidColorBrush({0.92f, 0.94f, 0.96f, 1.0f});

	kke::Geometry fullViewport = kke::Rect{{0.0f, 0.0f}, {1280.0f, 720.0f}};
	kke::Geometry normalMask = kke::Rect{{120.0f, 110.0f}, {520.0f, 430.0f}};
	kke::Geometry invertedMask = kke::Ellipse{{850.0f, 280.0f}, 145.0f};

	engine.fill(fullViewport, background);

	engine.pushLayer(normalMask);
	engine.fill(fullViewport, normalLayerFill);
	engine.popLayer();
	engine.draw(normalMask, outline, {4.0f});

	engine.pushLayer(invertedMask, kke::LayerMode::Inverted);
	engine.fill(fullViewport, invertedLayerFill);
	engine.popLayer();
	engine.draw(invertedMask, outline, {4.0f});

	fpsCounter.frame();
}
