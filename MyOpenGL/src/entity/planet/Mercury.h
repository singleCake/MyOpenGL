#pragma once

#include "PlanetBase.h"

class Mercury : public PlanetBase<Mercury> {
public:
	Mercury(const CVector& pos, const CVector& vel, float r)
		: AbstractEntity(pos, vel), PlanetBase<Mercury>(r) {}
};