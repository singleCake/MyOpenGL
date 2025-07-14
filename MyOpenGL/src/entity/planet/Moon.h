#pragma once

#include "PlanetBase.h"

class Moon : public PlanetBase<Moon> {
public:
	Moon(const CVector& pos, const CVector& vel, float r, CVector& center = DEFAULT_ANCHOR, bool src = false)
		: AbstractEntity(pos, vel), PlanetBase<Moon>(center, r, WHITE, src) {}
};