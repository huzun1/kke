#pragma once

struct Triangle;
struct Point;

Triangle operator+(Triangle lhs, Point const& rhs);

Triangle operator+(Triangle lhs, Triangle const& rhs);

Triangle operator-(Triangle lhs, Point const& rhs);

Triangle operator-(Triangle lhs, Triangle const& rhs);

Triangle& operator+=(Triangle& lhs, Point const& rhs);

Triangle& operator+=(Triangle& lhs, Triangle const& rhs);

Triangle& operator-=(Triangle& lhs, Point const& rhs);

Triangle& operator-=(Triangle& lhs, Triangle const& rhs);
