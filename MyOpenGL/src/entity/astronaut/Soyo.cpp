#include "Soyo.h"
#include "../../manager/CameraManager.h"

void Soyo::handleInput(int keyCode) {
	// 任意视角下，F使soyo原地踏步
	if (keyCode == GLFW_KEY_F) {
		moving = true;
		return;
	}

	// 玩家视角下，方向键控制soyo移动
	if (CameraManager::getCameraIDEqualTo("Player")) {
		CVector left = axis.crossMul(toward).normalize();
		switch (keyCode) {
		case GLFW_KEY_UP:
			if (checkCollision()) return;
			moving = true;
			towardVel = toward * PLAYER_DEFAULT_SPEED;
			break;
		case GLFW_KEY_DOWN:
			if (checkCollision()) return;
			moving = true;
			towardVel = -toward * PLAYER_DEFAULT_SPEED;
			break;
		case GLFW_KEY_LEFT:
			yaw(SENSITIVITY);
			break;
		case GLFW_KEY_RIGHT:
			yaw(-SENSITIVITY);
			break;
		}
		return;
	}
}