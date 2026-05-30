#pragma once

struct Ellipse;
struct Point;

Ellipse operator+(Ellipse lhs, Point const& rhs);

Ellipse operator+(Ellipse lhs, Ellipse const& rhs);

Ellipse operator-(Ellipse lhs, Point const& rhs);

Ellipse operator-(Ellipse lhs, Ellipse const& rhs);

Ellipse& operator+=(Ellipse& lhs, Point const& rhs);

Ellipse& operator+=(Ellipse& lhs, Ellipse const& rhs);

Ellipse& operator-=(Ellipse& lhs, Point const& rhs);

Ellipse& operator-=(Ellipse& lhs, Ellipse const& rhs);
