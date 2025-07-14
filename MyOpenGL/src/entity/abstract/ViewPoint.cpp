#include "../../manager/CameraManager.h"

ViewPoint::~ViewPoint() {
	camera.reset();
	CameraManager::removeEmptyCameras();
}

void ViewPoint::rotateVP(const CMatrix& rotation) {
	front = rotation.vecMul(front);
	left = rotation.vecMul(left);
	up = rotation.vecMul(up);
}

void ViewPoint::handleMouseOffset(double dx, double dy, AbstractEntity* entity) {
	if (dx != 0 || dy != 0) {
		// 计算相机的旋转
		float dyaw = dx * DEFAULT_CAMERA_SENSITIVITY;
		float dpitch = -dy * DEFAULT_CAMERA_SENSITIVITY;

		CEuler e(dyaw / PI * 180, dpitch / PI * 180, 0);

		if (Camera::VIEWPOINT_MODE == Camera::ViewpointMode::Child) {
			CMatrix mat = CMatrix::rotateMatrix(dyaw, up, getPos());
			entity->rotateMat(mat);

			mat = CMatrix::rotateMatrix(dpitch, left, getPos());
			CVector tmp = mat.vecMul(front);
			if (tmp.crossMul(up).len() < 0.1f) {
				return;
			}
			if (!fixedUp) {
				front = mat.vecMul(front);
			}
			else {
				CVector l_up = up;
				entity->rotateMat(mat);
				up = l_up;
			}

			euler = euler + e;
			euler.normalize();
		}
		else {
			CEuler newEuler = euler + e;
			newEuler.normalize();
			if (newEuler.p > 80) newEuler.p = 80;
			if (newEuler.p < -80) newEuler.p = -80;
			e = newEuler - euler;
			if (!fixedUp) {
				CEuler onlyYaw(e.h, 0, 0), onlyPitch(0, e.p, 0);
				entity->rotateMat(onlyYaw.toMatrix(up, left, getPos()));
				front = onlyPitch.toMatrix(up, left, getPos()).vecMul(front);
			}
			else {
				entity->rotateMat(e.toMatrix(up, left, getPos()));
			}
			euler = newEuler;
		}
	}
}

void ViewPoint::registerCamera() {
	if (camera) {
		CameraManager::addCamera(reinterpret_cast<CameraUPtr*>(&camera));
	}
}