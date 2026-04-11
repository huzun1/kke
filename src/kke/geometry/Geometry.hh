#pragma once

#include "Boundary.hh"

struct Geometry {
public:
    virtual Boundary getBounding() const = 0;
};
