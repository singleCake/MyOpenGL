#pragma once

#include "CameraBase.h"

class DefaultCamera : public CameraBase<DefaultCamera> {
	using CameraBase<DefaultCamera>::CameraBase;
	
public:
	void updateCamera() override;
};