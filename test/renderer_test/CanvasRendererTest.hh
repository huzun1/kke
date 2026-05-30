#pragma once

#include "RendererTest.hh"

namespace application {
namespace renderer_test {
class CanvasRendererTest : public RendererTest {
public:
	using RendererTest::RendererTest;

	void render() override;
};
}
}
