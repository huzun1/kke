#pragma once

#include "kke/appearance/resource/brush/Brush.hh"
#include "kke/engine/d2d/context/D2dContext.hh"
#include "kke/engine/d2d/d2d1_headers.hh"

namespace kke {
class BrushFactory {
  public:
	static Microsoft::WRL::ComPtr<ID2D1Brush> create(D2dContext const& context, Brush const& brush);

	static void
	updateGradientLine(ID2D1LinearGradientBrush* d2dBrush, LinearGradientBrush const& brush);

  private:
	static Microsoft::WRL::ComPtr<ID2D1Brush>
	create(D2dContext const& context, SolidColorBrush const& brush);

	static Microsoft::WRL::ComPtr<ID2D1Brush>
	create(D2dContext const& context, LinearGradientBrush const& brush);

	static kke::Point rotatePoint(Point const& point, Point const& center, float angle);
};
} // namespace kke
