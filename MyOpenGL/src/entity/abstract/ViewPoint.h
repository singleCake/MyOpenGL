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

	float offset; // �ӵ����������λ�õ�ƫ���������ڰ����壩

	bool fixedUp = false; // �Ƿ�̶�up����

	std::unique_ptr<AbstractEntity> camera;

	template<typename CamType>
	void createCamera() {
		camera = std::make_unique<CamType>(this);
		registerCamera();
	}

	void registerCamera();
};