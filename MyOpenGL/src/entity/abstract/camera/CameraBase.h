#pragma once

#include "Camera.h"

template<typename T>
class CameraBase : public Camera {
	static const char* ID;

public:
	using Camera::Camera;

	const char* getID() const override { 
		return CameraBase<T>::ID; 
	}
};