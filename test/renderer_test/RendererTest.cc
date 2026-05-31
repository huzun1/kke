#include "RendererTest.hh"

#include <memory>

#include "Renderer.hh"
namespace application {
namespace renderer_test {
RendererTest::RendererTest(Renderer& renderer) : renderer(renderer) {
}

RendererTest::~RendererTest() = default;

kke::D2dEngine& RendererTest::engine() {
	return renderer.engine;
}

std::shared_ptr<kke::Texture> const& RendererTest::encodedTexture() const {
	return renderer.encodedTexture;
}

std::shared_ptr<kke::Texture> const& RendererTest::rawTexture() const {
	return renderer.rawTexture;
}
} // namespace renderer_test
} // namespace application
