#include "InputManager.h"
#include "CameraManager.h"
#include "RenderManager.h"
#include "EntityManager.h"

int InputManager::mouseLeftButtonState = GLFW_RELEASE;
std::map<int, int> InputManager::keyPrevStates;
double InputManager::scrollOffset = 0.0f;

int InputManager::ALL_KEY_TO_PRESS[] = {
	GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_F,
    GLFW_KEY_R, GLFW_KEY_J, GLFW_KEY_K, GLFW_KEY_LEFT_CONTROL,
    GLFW_KEY_PAGE_UP , GLFW_KEY_PAGE_DOWN, GLFW_KEY_ENTER,
    GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT,
	-1
};

int InputManager::ALL_KEY_TO_CLICK[] = {
    GLFW_KEY_ENTER, GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_SPACE, -1
};

void InputManager::init() {
    glfwSetCursorPos(Application::window, INIT_MOUSE_X, INIT_MOUSE_Y);
    glfwSetInputMode(Application::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void InputManager::handleKeyPress(int keyCode, std::function<void()> event) {
    int currentState = glfwGetKey(Application::window, keyCode);
    if (currentState == GLFW_PRESS) {
        event();
    }
    keyPrevStates[keyCode] = currentState;
}

void InputManager::handleKeyClick(int keyCode, std::function<void()> event) {
    int currentState = glfwGetKey(Application::window, keyCode);
    if (currentState == GLFW_PRESS && keyPrevStates[keyCode] == GLFW_RELEASE) {
        event();
    }
    keyPrevStates[keyCode] = currentState;
}

void InputManager::handleAllKeyPress(std::function<void(int)> event) {
    for (int i = 0; ALL_KEY_TO_PRESS[i] != -1; ++i) {
        handleKeyPress(ALL_KEY_TO_PRESS[i], [&]() {
            event(ALL_KEY_TO_PRESS[i]);
            });
	}
}

void InputManager::handleAllKeyClick(std::function<void(int)> event) {
    for (int i = 0; ALL_KEY_TO_CLICK[i] != -1; ++i) {
        handleKeyClick(ALL_KEY_TO_CLICK[i], [&]() {
			event(ALL_KEY_TO_CLICK[i]);
			});
    }
}

void InputManager::getPickingRay(double mouseX, double mouseY,
	CVector& out_origin, CVector& out_direction) {
    // ת��Ϊ�ӿ�����
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float localX = mouseX - viewport[0];
    float localY = viewport[3] - (mouseY - viewport[1]); // Y�ᷭת
    // ת��ΪNDC���꣨-1��1��
    float x = (2.0f * localX) / viewport[2] - 1.0f;
    float y = (2.0f * localY) / viewport[3] - 1.0f;
    CVector rayClip(x, y, -1.0f);

    // ת�����ۿռ�
    CVector rayEye = CameraManager::getProjection().getInverse().posMul(rayClip);
    rayEye.set(rayEye.x, rayEye.y, -1.0f); // ��ǰ��

    // ת��������ռ�
    CVector rayWorld = CameraManager::getView().getInverse().vecMul(rayEye);
    out_direction = rayWorld.normalize();

    // �������Ϊ���λ�ã���ͼ�����������ƽ�Ʒ�����
    CMatrix iv = CameraManager::getView().getInverse();
    out_origin = {iv.m03, iv.m13, iv.m23};
}

void InputManager::handleMouseMove(std::function<void(double, double)> event) {
	double x, y;
	glfwGetCursorPos(Application::window, &x, &y);
	event(x, y);
}

static bool isFirstMouse = true; // �Ƿ��һ���ƶ����
static double lastX = InputManager::INIT_MOUSE_X, lastY = InputManager::INIT_MOUSE_Y; // �ϴ����λ��

void InputManager::handleMouseOffset(std::function<void(double, double)> event) {
    if (isFirstMouse) {
		lastX = INIT_MOUSE_X;
		lastY = INIT_MOUSE_Y;
		isFirstMouse = false;
    }
	double x, y;
	glfwGetCursorPos(Application::window, &x, &y);
	event(lastX - x, lastY - y);
    lastX = x;
    lastY = y;
}

void InputManager::resetMousePosition() {
	glfwSetCursorPos(Application::window, INIT_MOUSE_X, INIT_MOUSE_Y);
	isFirstMouse = true;
}

void InputManager::handleMouseClick(std::function<void(double, double)> event) {
	int button = GLFW_MOUSE_BUTTON_LEFT;
	int currentState = glfwGetMouseButton(Application::window, button);
    if (currentState == GLFW_PRESS && mouseLeftButtonState == GLFW_RELEASE) {
		double x, y;
		glfwGetCursorPos(Application::window, &x, &y);
		event(x, y);
	}
	mouseLeftButtonState = currentState;
}

void InputManager::handleMouseScroll(std::function<void(double)> event) {
    glfwSetScrollCallback(Application::window, [](GLFWwindow* window, double x, double y) {
        scrollOffset += y;
        if (scrollOffset <= -4.0f)
            scrollOffset = -4.0f;
        if (scrollOffset >= 4.0f)
            scrollOffset = 4.0f;
    });
    event(scrollOffset);
}

void InputManager::handleGlobalEvent() {
    // �˳�
    handleKeyClick(GLFW_KEY_ESCAPE, []() {
		glfwSetWindowShouldClose(Application::window, GLFW_TRUE);
	});
	// �л��߿����ģʽ
	handleKeyClick(GLFW_KEY_F1, []() {
		RenderManager::LINE_MODE = !RenderManager::LINE_MODE;
	});
    // �л��ӵ����
    handleKeyClick(GLFW_KEY_F2, []() {
        CameraManager::changeViewpointMode();
        });
    // ��ʾ��ײ��
    handleKeyClick(GLFW_KEY_F3, []() {
		EntityManager::SHOW_HITBOX = !EntityManager::SHOW_HITBOX;
	});
    // ��������Ϣ
    handleKeyClick(GLFW_KEY_F4, []() {
        Camera::switchDebug();
        });
    // �л����
    handleKeyClick(GLFW_KEY_F5, []() {
        CameraManager::switchCamera();
        });
    // �л����ģʽ
    handleKeyClick(GLFW_KEY_F6, []() {
        CameraManager::switchCameraMode();
        });

    // ʱ��ֹͣ����Ӱ�����ǣ�
    handleKeyClick(GLFW_KEY_F12, []() {
		EntityManager::TIME_FREEZE = !EntityManager::TIME_FREEZE;
	});
}