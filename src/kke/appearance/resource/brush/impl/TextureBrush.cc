#include "TextureBrush.hh"

#include <utility>

using namespace kke;

TextureBrush::TextureBrush(
	std::shared_ptr<Texture> texture, Rect destination, TextureDrawAppearance appearance
)
	: texture(std::move(texture)), destination(destination), appearance(appearance) {
}

std::shared_ptr<Texture> const& TextureBrush::getTexture() const {
	return texture;
}

Rect const& TextureBrush::getDestination() const {
	return destination;
}

TextureDrawAppearance const& TextureBrush::getAppearance() const {
	return appearance;
}
