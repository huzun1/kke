#pragma once

struct Scale;

Scale operator+(Scale lhs, float rhs);

Scale operator+(Scale lhs, Scale const& rhs);

Scale operator-(Scale lhs, float rhs);

Scale operator-(Scale lhs, Scale const& rhs);

Scale operator*(Scale lhs, float rhs);

Scale operator*(Scale lhs, Scale const& rhs);

Scale operator/(Scale lhs, float rhs);

Scale operator/(Scale lhs, Scale const& rhs);

Scale& operator+=(Scale& lhs, float rhs);

Scale& operator+=(Scale& lhs, Scale const& rhs);

Scale& operator-=(Scale& lhs, float rhs);

Scale& operator-=(Scale& lhs, Scale const& rhs);

Scale& operator*=(Scale& lhs, float rhs);

Scale& operator*=(Scale& lhs, Scale const& rhs);

Scale& operator/=(Scale& lhs, float rhs);

Scale& operator/=(Scale& lhs, Scale const& rhs);
