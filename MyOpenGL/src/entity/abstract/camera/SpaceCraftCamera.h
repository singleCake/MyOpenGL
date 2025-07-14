#pragma once

#include "CameraBase.h"

class SpaceCraftCamera : public CameraBase<SpaceCraftCamera> {
	using CameraBase<SpaceCraftCamera>::CameraBase;

	enum Mode { Default, LookInside, AllMode };

public:
	void updateCamera() override;

	void switchMode() override {
		mode = static_cast<Mode>((mode + 1) % AllMode);
	}

private:
	Mode mode = Default;
};