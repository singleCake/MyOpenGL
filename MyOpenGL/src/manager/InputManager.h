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

	// 读取键盘
	static void handleAllKeyPress(std::function<void(int)> event);
	static void handleAllKeyClick(std::function<void(int)> event);

	// 转化鼠标坐标为3D射线
	static void getPickingRay(double mouseX, double mouseY,
		CVector& out_origin, CVector& out_direction);

	// 读取鼠标移动
	static void handleMouseMove(std::function<void(double, double)>);

	// 读取鼠标位移
	static void handleMouseOffset(std::function<void(double, double)>);

	// 重置鼠标位置
	static void resetMousePosition();

	// 读取鼠标点击
	static void handleMouseClick(std::function<void(double, double)>);

	// 读取鼠标滚轮
	static void handleMouseScroll(std::function<void(double)>);

	// 全局事件
	static void handleGlobalEvent();

private:
	static int mouseLeftButtonState;
	static std::map<int, int> keyPrevStates;

	static int ALL_KEY_TO_PRESS[];
	static int ALL_KEY_TO_CLICK[];

	static double scrollOffset;

	// 读取键盘
	static void handleKeyPress(int keyCode, std::function<void()>);
	static void handleKeyClick(int keyCode, std::function<void()> event);
};