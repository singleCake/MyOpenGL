#pragma once

#include "PlanetBase.h"

class Earth : public PlanetBase<Earth> {
public:
	Earth(const CVector& pos, const CVector& vel, float r, CVector& center = DEFAULT_ANCHOR, bool src = false)
		: AbstractEntity(pos, vel), PlanetBase<Earth>(center, r, WHITE, src) {}
};