#pragma once

#include "Astronaut.h"

template<typename Name>
class AstronautBase : public Astronaut {
	static const char* ID;
	static const char* NAME;
public:
	using Astronaut::Astronaut;

	const char* getID() const override {
		return AstronautBase<Name>::ID;
	}

	const char* getName() const override {
		return AstronautBase<Name>::NAME;
	}
};