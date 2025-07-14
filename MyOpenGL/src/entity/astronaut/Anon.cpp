#include "Anon.h"
#include "../../manager/CameraManager.h"

void Anon::handleInput(int keyCode) {
	// �����ӽ��£�Fʹanonԭ��̤��
	if (keyCode == GLFW_KEY_F) {
		moving = true;
		return;
	}
	// �ɴ��ӽ��£����������anon�ƶ�
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

	// ����ӽ��£�WASD�����ƶ�
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