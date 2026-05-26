#pragma once

#include <cstddef>
#include <memory>
#include <optional>

#include "kke/appearance/painting/StrokeAppearance.hh"
#include "kke/appearance/resource/brush/Brush.hh"
#include "kke/appearance/resource/effect/EffectCompose.hh"
#include "kke/appearance/resource/font/Font.hh"
#include "kke/appearance/resource/texture/Texture.hh"
#include "kke/appearance/view/LayerMode.hh"
#include "kke/engine/Sources.hh"

namespace kke {
/**
 * @brief Abstract interface for a rendering engine.
 */
class Engine {
public:
	virtual ~Engine() = default;

	/* =============== Render Statement ================ */

	/**
	 * @brief Clears the current rendering target.
	 */
	virtual void clear() = 0;

	/*================ Transform Control ================ */

	/**
	 * @brief Pushes a transform onto the transform stack.
	 * @param transform The transform source to apply.
	 */
	virtual void pushTransform(TransformSource const& transform) = 0;

	/**
	 * @brief Pops the most recently pushed transform from the stack.
	 */
	virtual void popTransform() = 0;

	/* ================= Layer Control ================== */

	/**
	 * @brief Pushes a new layer onto the layer stack.
	 * @param mask The mask source applied to the layer.
	 * @param mode The blending mode of the layer (default: NORMAL).
	 */
	virtual void pushLayer(MaskSource const& mask, LayerMode mode = LayerMode::Normal) = 0;

	/**
	 * @brief Pops the most recently pushed layer from the stack.
	 */
	virtual void popLayer() = 0;

	/* ================= Canvas Control ================= */

	/**
	 * @brief Creates a new canvas instance.
	 * @return A shared pointer to the created canvas.
	 */
	virtual std::shared_ptr<Canvas> createCanvas() = 0;

	/**
	 * @brief Pushes a canvas onto the canvas stack as the current render target.
	 * @param canvas The canvas to set as active.
	 */
	virtual void pushCanvas(std::shared_ptr<Canvas> canvas) = 0;

	/**
	 * @brief Pops the current canvas from the stack and restores the previous one.
	 */
	virtual void popCanvas() = 0;

	/**
	 * @brief Draws a canvas onto the current render target.
	 * @param canvas The canvas to draw.
	 * @param opacity The opacity value (default: 1.0).
	 */
	virtual void draw(
		std::shared_ptr<Canvas> canvas,
		float opacity = 1.0f) = 0;

	/* ================= Measurement =================== */

	/**
	 * @brief Gets the current viewport size.
	 * @return The viewport size as a Scale.
	 */
	virtual Scale getViewportSize() = 0;

	/**
	 * @brief Measures the size of rendered text.
	 * @param text The text to measure.
	 * @return The measured size as a Scale.
	 */
	virtual Scale measureTextSize(Text const& text) = 0;

	/* ================= Stroke Rendering ================= */

	/**
	 * @brief Draws a stroke using the specified source, brush, and appearance.
	 * @param source The stroke source geometry.
	 * @param brush The brush used for rendering.
	 * @param appearance The stroke appearance settings.
	 */
	virtual void draw(
		StrokeSource const& source,
		Brush const& brush,
		StrokeAppearance const& appearance) = 0;

	/* ================= Fill Rendering ================= */

	/**
	 * @brief Fills a region using the specified source and brush.
	 * @param source The fill source geometry.
	 * @param brush The brush used for filling.
	 */
	virtual void fill(
		FillSource const& source,
		Brush const& brush) = 0;

	/* ================= Font Resource ================= */

	/**
	 * @brief Uploads raw font data.
	 * @param data Pointer to the font data.
	 * @param size Size of the data in bytes.
	 * @return A shared pointer to the uploaded font.
	 */
	virtual std::shared_ptr<Font> uploadFont(
		void const* data,
		size_t size) = 0;

	/* ================ Texture Rendering ================ */

	/**
	 * @brief Uploads raw data as a texture.
	 * @param data Pointer to the texture data.
	 * @param size Size of the data in bytes.
	 * @return A shared pointer to the uploaded texture.
	 */
	virtual std::shared_ptr<Texture> uploadTexture(
		void const* data, size_t size) = 0;

	/**
	 * @brief Draws a texture onto the current render target.
	 * @param texture The texture to draw.
	 * @param destRect Destination rectangle.
	 * @param opacity Opacity value (default: 1.0).
	 * @param srcRect Optional source rectangle within the texture.
	 */
	virtual void draw(
		std::shared_ptr<Texture> texture,
		Rect const& destRect,
		float opacity = 1.0f,
		std::optional<Rect> srcRect = std::nullopt) = 0;

	/* ================= Effect Rendering ================= */

	/**
	 * @brief Renders an effect using a single effect definition.
	 * @param source The effect source.
	 * @param effect The effect to apply.
	 */
	virtual void renderEffect(
		Effect const& effect) = 0;

	/**
	 * @brief Renders an effect using a single effect definition.
	 * @param source The effect source.
	 * @param effect The effect to apply.
	 */
	virtual void renderEffect(
		EffectSource const& source,
		Effect const& effect) = 0;

	/**
	 * @brief Renders a composed effect using multiple effects.
	 * @param source The effect source.
	 * @param effect The composed effect definition.
	 */
	virtual void renderEffect(
		EffectCompose const& effect) = 0;

	/**
	 * @brief Renders a composed effect using multiple effects.
	 * @param source The effect source.
	 * @param effect The composed effect definition.
	 */
	virtual void renderEffect(
		EffectSource const& source,
		EffectCompose const& effect) = 0;
};
} // namespace kke
