#pragma once
#include <map>
#include <functional>

#include "../geometry/Geometry.h"
#include "../Application.h"

class InputManager {
public:
	static const int INIT_MOUSE_X = Application::WIDTH / 2;
	static const int INIT_MOUSE_Y = Application::HEIGHT / 2;

	InputManager() = default;
	~InputManager() = default;

	static void init();

	// ��ȡ����
	static void handleAllKeyPress(std::function<void(int)> event);
	static void handleAllKeyClick(std::function<void(int)> event);

	// ת���������Ϊ3D����
	static void getPickingRay(double mouseX, double mouseY,
		CVector& out_origin, CVector& out_direction);

	// ��ȡ����ƶ�
	static void handleMouseMove(std::function<void(double, double)>);

	// ��ȡ���λ��
	static void handleMouseOffset(std::function<void(double, double)>);

	// �������λ��
	static void resetMousePosition();

	// ��ȡ�����
	static void handleMouseClick(std::function<void(double, double)>);

	// ��ȡ������
	static void handleMouseScroll(std::function<void(double)>);

	// ȫ���¼�
	static void handleGlobalEvent();

private:
	static int mouseLeftButtonState;
	static std::map<int, int> keyPrevStates;

	static int ALL_KEY_TO_PRESS[];
	static int ALL_KEY_TO_CLICK[];

	static double scrollOffset;

	// ��ȡ����
	static void handleKeyPress(int keyCode, std::function<void()>);
	static void handleKeyClick(int keyCode, std::function<void()> event);
};