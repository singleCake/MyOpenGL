#pragma once

#include "AstronautBase.h"

class Soyo : public AstronautBase<Soyo> {
public:
	Soyo()
		: AstronautBase<Soyo>(), AbstractEntity() {}

	void handleInput(int keyCode) override;
};