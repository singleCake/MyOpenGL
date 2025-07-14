#pragma once

#include "PlanetBase.h"

class Sun : public PlanetBase<Sun> {
public:
	Sun(const CVector& pos, const CVector& vel, float r, bool src = false)
		: AbstractEntity(pos, vel), PlanetBase<Sun>(r, RED, src) {}
};