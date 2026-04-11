#pragma once

struct Rect;
struct Point2f;

Rect operator+(Rect lhs, Point2f const& rhs);

Rect operator+(Rect lhs, Rect const& rhs);

Rect operator-(Rect lhs, Point2f const& rhs);

Rect operator-(Rect lhs, Rect const& rhs);

Rect& operator+=(Rect& lhs, Point2f const& rhs);

Rect& operator+=(Rect& lhs, Rect const& rhs);

Rect& operator-=(Rect& lhs, Point2f const& rhs);

Rect& operator-=(Rect& lhs, Rect const& rhs);
