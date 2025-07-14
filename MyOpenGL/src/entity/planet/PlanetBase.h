#pragma once

#include "Planet.h"

template<typename Name>
class PlanetBase : public Planet {
	using Planet::Planet;

	static const char* ID;
	static const char* NAME;

public:
	const char* getID() const override {
		return PlanetBase<Name>::ID;
	}

	const char* getName() const override {
		return PlanetBase<Name>::NAME;
	}
};