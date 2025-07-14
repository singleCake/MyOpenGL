#pragma once

#include "AstronautBase.h"

class Anon : public AstronautBase<Anon> {
public:
	Anon()
		: AstronautBase<Anon>(), AbstractEntity() {}

	void handleInput(int keyCode) override;
};