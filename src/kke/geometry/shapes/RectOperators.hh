#pragma once

struct Rect;
struct Point;

Rect operator+(Rect lhs, Point const& rhs);

Rect operator+(Rect lhs, Rect const& rhs);

Rect operator-(Rect lhs, Point const& rhs);

Rect operator-(Rect lhs, Rect const& rhs);

Rect& operator+=(Rect& lhs, Point const& rhs);

Rect& operator+=(Rect& lhs, Rect const& rhs);

Rect& operator-=(Rect& lhs, Point const& rhs);

Rect& operator-=(Rect& lhs, Rect const& rhs);
