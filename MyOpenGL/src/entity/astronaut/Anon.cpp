#include "Anon.h"
#include "../../manager/CameraManager.h"

void Anon::handleInput(int keyCode) {
	// 任意视角下，F使anon原地踏步
	if (keyCode == GLFW_KEY_F) {
		moving = true;
		return;
	}
	// 飞船视角下，方向键控制anon移动
	if (CameraManager::getCameraIDEqualTo("SpaceCraft")) {
		if (checkCollision()) return;
		switch (keyCode) {
		case GLFW_KEY_UP:
			moving = true;
			towardVel = toward * PLAYER_DEFAULT_SPEED;
			break;
		case GLFW_KEY_DOWN:
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

	// 玩家视角下，WASD控制移动
	if (CameraManager::getCameraIDEqualTo("Player")) {
		CVector left = axis.crossMul(toward).normalize();
		if (checkCollision()) return;
		switch (keyCode) {
		case GLFW_KEY_W:
			moving = true;
			towardVel = toward * PLAYER_DEFAULT_SPEED;
			break;
		case GLFW_KEY_S:
			moving = true;
			towardVel = -toward * PLAYER_DEFAULT_SPEED;
			break;
		case GLFW_KEY_A:
			moving = true;
			leftVel = left * PLAYER_DEFAULT_SPEED;
			break;
		case GLFW_KEY_D:
			moving = true;
			leftVel = -left * PLAYER_DEFAULT_SPEED;
			break;
		}
	}
}