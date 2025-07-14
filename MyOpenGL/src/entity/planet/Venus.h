#pragma once

#include "PlanetBase.h"

class Venus : public PlanetBase<Venus> {
public:
	Venus(const CVector& pos, const CVector& vel, float r)
		: AbstractEntity(pos, vel), PlanetBase<Venus>(r) {}
};