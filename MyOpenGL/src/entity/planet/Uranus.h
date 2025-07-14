#pragma once

#include "PlanetBase.h"

class Uranus : public PlanetBase<Uranus> {
public:
	Uranus(const CVector& pos, const CVector& vel, float r)
		: AbstractEntity(pos, vel), PlanetBase<Uranus>(r) {}
};