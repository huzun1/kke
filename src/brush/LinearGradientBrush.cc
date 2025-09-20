#include <math.h>

#include <cstdint>
#include <cstdlib>
#include <numbers>
#include <oreik/brush/LinearGradientBrush.hpp>

#include "oreik/common/Color.hpp"
#include "oreik/common/Point.hpp"
#include "oreik/internal/Hasher.hpp"

oreik::LinearGradientBrush::LinearGradientBrush(oreik::Color4f const& startColor,
												oreik::Color4f const& endColor,
												oreik::Point2f const& startPoint,
												oreik::Point2f const& endPoint)
	: colors({startColor, endColor}),
	  startPoint(startPoint),
	  endPoint(endPoint) {
}

oreik::LinearGradientBrush::LinearGradientBrush(std::vector<oreik::Color4f> const& colors,
												oreik::Point2f const& startPoint,
												oreik::Point2f const& endPoint)
	: colors(colors),
	  startPoint(startPoint),
	  endPoint(endPoint) {
}

void oreik::LinearGradientBrush::setAngle(float angle) {
	this->angle = angle;
}

void oreik::LinearGradientBrush::create(ID2D1DeviceContext* context, ID2D1Brush** output) const {
	oreik::Point2f startPoint = this->startPoint;
	oreik::Point2f endPoint = this->endPoint;
	if (this->angle) {
		float width = endPoint.x - startPoint.x;
		float height = endPoint.y - startPoint.y;
		float dist = fmaxf(fabsf(width), fabsf(height)) * 0.5f;
		float centerX = width / 2 + startPoint.x;
		float centerY = height / 2 + startPoint.y;
		float offsetX = dist * cosf(*angle / 180.0f * static_cast<float>(std::numbers::pi));
		float offsetY = dist * sinf(*angle / 180.0f * static_cast<float>(std::numbers::pi));
		startPoint = oreik::Point2f{centerX + offsetX, centerY + offsetY};
		endPoint = oreik::Point2f{centerX - offsetX, centerY - offsetY};
	}

	ID2D1GradientStopCollection* gradientStopCollection;
	std::vector<D2D1_GRADIENT_STOP> gradientStops;
	for (int i = 0; i < colors.size(); i++) {
		const D2D1_COLOR_F color = D2D1::ColorF(colors[i].r, colors[i].g, colors[i].b, colors[i].a);
		const float position = i * 1.0f / (colors.size() - 1);
		gradientStops.emplace_back(position, color);
	}
	context->CreateGradientStopCollection(gradientStops.data(), gradientStops.size(), D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &gradientStopCollection);

	ID2D1LinearGradientBrush* brush;
	context->CreateLinearGradientBrush(
		D2D1::LinearGradientBrushProperties(D2D1::Point2F(startPoint.x, startPoint.y), D2D1::Point2F(endPoint.x, endPoint.y)),
		gradientStopCollection, &brush);
	gradientStopCollection->Release();

	*output = brush;
}

uint64_t oreik::LinearGradientBrush::hash() const {
	oreik::Hasher hash;
	hash.combine(static_cast<uint32_t>(oreik::BrushType::LINEAR_GRADIENT_BRUSH));
	for (oreik::Color4f const& color : colors) {
		hash.combine(color.r);
		hash.combine(color.g);
		hash.combine(color.b);
		hash.combine(color.a);
	}
	hash.combine(startPoint.hash());
	hash.combine(endPoint.hash());
	if (angle) {
		float normalizedAngle = *angle;
		while (normalizedAngle < 0.0f) {
			normalizedAngle += 360.0f;
		}
		while (normalizedAngle > 360.0f) {
			normalizedAngle -= 360.0f;
		}
		hash.combine(normalizedAngle);
	}

	return hash.get();
}
