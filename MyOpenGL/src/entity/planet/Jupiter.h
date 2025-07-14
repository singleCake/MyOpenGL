#pragma once

#include "PlanetBase.h"

class Jupiter : public PlanetBase<Jupiter> {
public:
	Jupiter(const CVector& pos, const CVector& vel, float r)
		: AbstractEntity(pos, vel), PlanetBase<Jupiter>(r) {}
};