#pragma once

struct Ellipse;
struct Point2f;

Ellipse operator+(Ellipse lhs, Point2f const& rhs);

Ellipse operator+(Ellipse lhs, Ellipse const& rhs);

Ellipse operator-(Ellipse lhs, Point2f const& rhs);

Ellipse operator-(Ellipse lhs, Ellipse const& rhs);

Ellipse& operator+=(Ellipse& lhs, Point2f const& rhs);

Ellipse& operator+=(Ellipse& lhs, Ellipse const& rhs);

Ellipse& operator-=(Ellipse& lhs, Point2f const& rhs);

Ellipse& operator-=(Ellipse& lhs, Ellipse const& rhs);
