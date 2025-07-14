#pragma once

#include "CameraBase.h"

class PlayerCamera : public CameraBase<PlayerCamera> {
	using CameraBase<PlayerCamera>::CameraBase;

	enum Mode { FirstPerson, ThridPerson, SecondPerson, AllMode };

public:
	void updateCamera() override;

	void switchMode() override {
		mode = static_cast<Mode>((mode + 1) % AllMode);
	}

private:
	Mode mode = FirstPerson;
};