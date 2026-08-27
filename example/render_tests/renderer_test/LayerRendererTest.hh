#pragma once

#include "RendererTest.hh"

namespace application {
namespace renderer_test {
class LayerRendererTest : public RendererTest {
  public:
	using RendererTest::RendererTest;

	void render() override;

  private:
	void renderRoundedRectClipDemo();

	void renderInversedClipDemo();

	void renderComposedGeometryImageDemo();
};
} // namespace renderer_test
} // namespace application
