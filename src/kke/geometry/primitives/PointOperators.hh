#pragma once

struct Point;

Point operator+(Point lhs, float rhs);

Point operator+(Point lhs, Point const& rhs);

Point operator-(Point lhs, float rhs);

Point operator-(Point lhs, Point const& rhs);

Point operator*(Point lhs, float rhs);

Point operator*(Point lhs, Point const& rhs);

Point operator/(Point lhs, float rhs);

Point operator/(Point lhs, Point const& rhs);

Point& operator+=(Point& lhs, float rhs);

Point& operator+=(Point& lhs, Point const& rhs);

Point& operator-=(Point& lhs, float rhs);

Point& operator-=(Point& lhs, Point const& rhs);

Point& operator*=(Point& lhs, float rhs);

Point& operator*=(Point& lhs, Point const& rhs);

Point& operator/=(Point& lhs, float rhs);

Point& operator/=(Point& lhs, Point const& rhs);
