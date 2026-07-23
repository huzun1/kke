#include "BrushFactory.hh"

#include <cmath>
#include <variant>
#include <vector>

#include "kke/appearance/Color.hh"
#include "kke/appearance/ColorQuantizer.hh"

using namespace kke;
using Microsoft::WRL::ComPtr;

ComPtr<ID2D1Brush> BrushFactory::create(D2dContext const& context, Brush const& brush) {
	return std::visit(
		[&](auto const& brushVariant) { return create(context, brushVariant); },
		brush
	);
}

ComPtr<ID2D1Brush> BrushFactory::create(D2dContext const& context, SolidColorBrush const& brush) {
	Color color = ColorQuantizer::quantize(brush.getColor());
	D2D1_COLOR_F d2dColor{color.r, color.g, color.b, color.a};

	ComPtr<ID2D1SolidColorBrush> solidBrush;
	HRESULT result = context.getDeviceContext()->CreateSolidColorBrush(d2dColor, &solidBrush);
	if (FAILED(result)) {
		return nullptr;
	}

	ComPtr<ID2D1Brush> d2dBrush;
	solidBrush.As(&d2dBrush);
	return d2dBrush;
}

ComPtr<ID2D1Brush>
BrushFactory::create(D2dContext const& context, LinearGradientBrush const& brush) {
	std::vector<Color> const& colors = brush.getColors();
	if (colors.empty()) {
		return nullptr;
	}

	std::vector<D2D1_GRADIENT_STOP> gradientStops;
	gradientStops.reserve(colors.size());
	for (size_t index = 0; index < colors.size(); ++index) {
		Color color = ColorQuantizer::quantize(colors[index]);
		float position = colors.size() == 1
							 ? 0.0f
							 : static_cast<float>(index) / static_cast<float>(colors.size() - 1);
		gradientStops.push_back({position, {color.r, color.g, color.b, color.a}});
	}

	ComPtr<ID2D1GradientStopCollection> stopCollection;
	HRESULT result = context.getDeviceContext()->CreateGradientStopCollection(
		gradientStops.data(),
		static_cast<UINT32>(gradientStops.size()),
		&stopCollection
	);
	if (FAILED(result)) {
		return nullptr;
	}

	Point center = (brush.getStartPoint() + brush.getEndPoint()) / 2.0f;
	Point startPoint = rotatePoint(brush.getStartPoint(), center, brush.getAngle());
	Point endPoint = rotatePoint(brush.getEndPoint(), center, brush.getAngle());
	D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES properties{
		{startPoint.x, startPoint.y},
		{endPoint.x, endPoint.y}
	};

	ComPtr<ID2D1LinearGradientBrush> linearBrush;
	result = context.getDeviceContext()
				 ->CreateLinearGradientBrush(properties, stopCollection.Get(), &linearBrush);
	if (FAILED(result)) {
		return nullptr;
	}

	ComPtr<ID2D1Brush> d2dBrush;
	linearBrush.As(&d2dBrush);
	return d2dBrush;
}

Point BrushFactory::rotatePoint(Point const& point, Point const& center, float angle) {
	constexpr float pi = 3.14159265358979323846f;
	float radian = angle * pi / 180.0f;
	float cosValue = std::cos(radian);
	float sinValue = std::sin(radian);
	float translatedX = point.x - center.x;
	float translatedY = point.y - center.y;

	return {
		center.x + translatedX * cosValue - translatedY * sinValue,
		center.y + translatedX * sinValue + translatedY * cosValue
	};
}
