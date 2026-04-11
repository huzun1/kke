#pragma once

struct Triangle;
struct Point2f;

Triangle operator+(Triangle lhs, Point2f const& rhs);

Triangle operator+(Triangle lhs, Triangle const& rhs);

Triangle operator-(Triangle lhs, Point2f const& rhs);

Triangle operator-(Triangle lhs, Triangle const& rhs);

Triangle& operator+=(Triangle& lhs, Point2f const& rhs);

Triangle& operator+=(Triangle& lhs, Triangle const& rhs);

Triangle& operator-=(Triangle& lhs, Point2f const& rhs);

Triangle& operator-=(Triangle& lhs, Triangle const& rhs);
