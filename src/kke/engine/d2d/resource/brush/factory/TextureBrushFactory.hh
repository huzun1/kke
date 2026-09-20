#pragma once

#include "kke/appearance/resource/brush/impl/TextureBrush.hh"
#include "kke/engine/d2d/context/D2dContext.hh"

namespace kke {
class TextureBrushFactory {
  public:
	static Microsoft::WRL::ComPtr<ID2D1Brush>
	create(D2dContext const& context, TextureBrush const& brush);

  private:
	static bool isValidRect(Rect const& rect);
};
} // namespace kke
