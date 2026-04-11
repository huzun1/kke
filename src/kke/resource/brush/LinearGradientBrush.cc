#include <math.h>

#include <cstdint>
#include <cstdlib>
#include <kke/brush/LinearGradientBrush.hh>
#include <numbers>

#include "kke/common/Color.hh"
#include "kke/common/Point.hh"
#include "kke/internal/Hasher.hh"

kke::LinearGradientBrush::LinearGradientBrush(kke::Color4f const& startColor,
											  kke::Color4f const& endColor,
											  kke::Point2f const& startPoint,
											  kke::Point2f const& endPoint)
	: colors({startColor, endColor}),
	  startPoint(startPoint),
	  endPoint(endPoint) {
}

kke::LinearGradientBrush::LinearGradientBrush(std::vector<kke::Color4f> const& colors,
											  kke::Point2f const& startPoint,
											  kke::Point2f const& endPoint)
	: colors(colors),
	  startPoint(startPoint),
	  endPoint(endPoint) {
}

void kke::LinearGradientBrush::setAngle(float angle) {
	this->angle = angle;
}

void kke::LinearGradientBrush::create(ID2D1DeviceContext* context, ID2D1Brush** output) const {
	kke::Point2f startPoint = this->startPoint;
	kke::Point2f endPoint = this->endPoint;
	if (this->angle) {
		float width = endPoint.x - startPoint.x;
		float height = endPoint.y - startPoint.y;
		float dist = fmaxf(fabsf(width), fabsf(height)) * 0.5f;
		float centerX = width / 2 + startPoint.x;
		float centerY = height / 2 + startPoint.y;
		float offsetX = dist * cosf(*angle / 180.0f * static_cast<float>(std::numbers::pi));
		float offsetY = dist * sinf(*angle / 180.0f * static_cast<float>(std::numbers::pi));
		startPoint = kke::Point2f{centerX + offsetX, centerY + offsetY};
		endPoint = kke::Point2f{centerX - offsetX, centerY - offsetY};
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

uint64_t kke::LinearGradientBrush::hash() const {
	kke::Hasher hash;
	hash.combine(static_cast<uint32_t>(kke::BrushType::LINEAR_GRADIENT_BRUSH));
	for (kke::Color4f const& color : colors) {
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
