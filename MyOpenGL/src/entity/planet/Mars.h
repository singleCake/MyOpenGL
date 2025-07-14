#pragma once

#include "PlanetBase.h"

class Mars : public PlanetBase<Mars> {
public:
	Mars(const CVector& pos, const CVector& vel, float r) 
		: AbstractEntity(pos, vel), PlanetBase<Mars>(r) {}
};