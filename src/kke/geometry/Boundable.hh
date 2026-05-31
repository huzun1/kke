#pragma once

#include "Boundary.hh"

struct Boundable {
  public:
	[[nodiscard]] virtual Boundary getBounding() const = 0;
};
