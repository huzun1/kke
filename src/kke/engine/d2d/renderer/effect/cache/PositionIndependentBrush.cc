#include "PositionIndependentBrush.hh"

#include "kke/appearance/resource/brush/impl/LinearGradientBrush.hh"
#include "kke/appearance/resource/brush/impl/SolidColorBrush.hh"
#include "kke/engine/d2d/resource/brush/hash/BrushHasher.hh"

using namespace kke;

Brush PositionIndependentBrush::normalize(Brush const& brush, Point const& origin) {
	return std::visit(
		[&](auto const& brushVariant) -> Brush {
			using BrushType = std::decay_t<decltype(brushVariant)>;
			if constexpr (std::is_same_v<BrushType, SolidColorBrush>) {
				return brushVariant;
			} else if constexpr (std::is_same_v<BrushType, LinearGradientBrush>) {
				return LinearGradientBrush{
					brushVariant.getColors(),
					brushVariant.getStartPoint() - origin,
					brushVariant.getEndPoint() - origin,
					brushVariant.getAngle()
				};
			} else {
				Rect destination = brushVariant.getDestination();
				destination.min = destination.min - origin;
				destination.max = destination.max - origin;
				return RasterSurfaceBrush{
					brushVariant.getSurface(),
					destination,
					brushVariant.getOpacity()
				};
			}
		},
		brush
	);
}

uint64_t PositionIndependentBrush::hash(Brush const& brush, Point const& origin) {
	return BrushHasher::hash(normalize(brush, origin));
}
