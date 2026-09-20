#pragma once

#include <memory>

#include "kke/appearance/resource/texture/Texture.hh"
#include "kke/appearance/resource/texture/TextureDrawAppearance.hh"
#include "kke/geometry/shapes/Rect.hh"

namespace kke {
/** @brief Maps a texture source rectangle to a destination in local drawing coordinates. */
class TextureBrush {
	std::shared_ptr<Texture> texture;
	Rect destination;
	TextureDrawAppearance appearance;

  public:
	/** @note Sampling outside the source rectangle clamps to its edges. */
	TextureBrush(
		std::shared_ptr<Texture> texture, Rect destination, TextureDrawAppearance appearance = {}
	);

	std::shared_ptr<Texture> const& getTexture() const;

	Rect const& getDestination() const;

	TextureDrawAppearance const& getAppearance() const;
};
} // namespace kke
