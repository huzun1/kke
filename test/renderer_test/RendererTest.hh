#pragma once

#include <memory>

#include "kke/appearance/resource/texture/Texture.hh"
#include "kke/engine/d2d/D2dEngine.hh"

namespace application {
class Renderer;

namespace renderer_test {
class RendererTest {
  protected:
	Renderer& renderer;

	kke::D2dEngine& engine();

	std::shared_ptr<kke::Texture> const& encodedTexture() const;

	std::shared_ptr<kke::Texture> const& mountainTexture() const;

	std::shared_ptr<kke::Texture> const& rawTexture() const;

  public:
	explicit RendererTest(Renderer& renderer);

	virtual ~RendererTest();

	virtual void render() = 0;
};
} // namespace renderer_test
} // namespace application
