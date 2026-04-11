#pragma once

struct Point2f;

Point2f operator+(Point2f lhs, float rhs);

Point2f operator+(Point2f lhs, Point2f const& rhs);

Point2f operator-(Point2f lhs, float rhs);

Point2f operator-(Point2f lhs, Point2f const& rhs);

Point2f operator*(Point2f lhs, float rhs);

Point2f operator*(Point2f lhs, Point2f const& rhs);

Point2f operator/(Point2f lhs, float rhs);

Point2f operator/(Point2f lhs, Point2f const& rhs);

Point2f& operator+=(Point2f& lhs, float rhs);

Point2f& operator+=(Point2f& lhs, Point2f const& rhs);

Point2f& operator-=(Point2f& lhs, float rhs);

Point2f& operator-=(Point2f& lhs, Point2f const& rhs);

Point2f& operator*=(Point2f& lhs, float rhs);

Point2f& operator*=(Point2f& lhs, Point2f const& rhs);

Point2f& operator/=(Point2f& lhs, float rhs);

Point2f& operator/=(Point2f& lhs, Point2f const& rhs);
