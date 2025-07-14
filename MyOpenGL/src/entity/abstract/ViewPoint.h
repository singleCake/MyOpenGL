#pragma once

#include "../../geometry/Geometry.h"
#include "../../constant/CameraConstant.h"
#include <memory>

class ViewPoint : virtual public AbstractEntity {
public:
	friend class Camera;

	ViewPoint() = default;
	~ViewPoint();

	void rotateVP(const CMatrix& mat);

	void handleMouseOffset(double dx, double dy, AbstractEntity* entity);

protected:
	CVector left, front, up;
	CEuler euler;

	float offset; // 视点中心与相机位置的偏移量（用于绑定星体）

	bool fixedUp = false; // 是否固定up向量

	std::unique_ptr<AbstractEntity> camera;

	template<typename CamType>
	void createCamera() {
		camera = std::make_unique<CamType>(this);
		registerCamera();
	}

	void registerCamera();
};