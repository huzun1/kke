#pragma once

namespace kke {
/**
 * Immutable image of the active render target at a specific draw boundary.
 *
 * A snapshot can be used as an explicit effect input after the active target
 * has changed. Implementations own the backend resource needed to keep that
 * image alive.
 */
class TargetSnapshot {
  public:
	virtual ~TargetSnapshot() = default;
};
} // namespace kke
