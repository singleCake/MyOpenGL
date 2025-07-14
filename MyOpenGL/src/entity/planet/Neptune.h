#pragma once

#include "PlanetBase.h"

class Neptune : public PlanetBase<Neptune> {
public:
	Neptune(const CVector& pos, const CVector& vel, float r)
		: AbstractEntity(pos, vel), PlanetBase<Neptune>(r) {}
};