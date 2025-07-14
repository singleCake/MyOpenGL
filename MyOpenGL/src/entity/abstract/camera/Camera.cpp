#include "Camera.h"
#include "../../../math/CQuaternion.h"

#include <iostream>

bool Camera::DEBUG_INFO = false;
int Camera::VIEWPOINT_MODE = Child;

Camera::Camera() : Camera({ INIT_X, INIT_Y, INIT_Z }, { 0, 0, 0 }, { 0, 1, 0 }) {}

Camera::Camera(const CVector& pos, const CVector& vel, const CVector& axis)
	: AbstractEntity(pos, vel, axis), up(axis) {
	viewpoint = nullptr;

    cvel = DEFAULT_CAMERA_SPEED;
	view = CMatrix::lookAtMatrix(pos, CVector(0, 0, 0), up);
	front.set(-view.m20, -view.m21, -view.m22);
	euler = front.toEuler();
}

void Camera::update(float dt) {
    if (viewpoint)
        updateCamera();
    updateView();
    glLoadMatrixf(view);
}

void Camera::updateCamera() {
    setUp(getVPUp());
    setFront(getVPFront());
}

void Camera::handleKeyInput(int keyCode) {
    if (viewpoint) return;

    switch (keyCode) {
    case GLFW_KEY_W:
        moveForward(cvel);
        break;
    case GLFW_KEY_S:
        moveForward(-cvel);
        break;
    case GLFW_KEY_A:
        moveRight(-cvel);
        break;
    case GLFW_KEY_D:
        moveRight(cvel);
        break;
    case GLFW_KEY_R:
        cvel += CAMERA_SPEED_SENSITIVITY;
        break;
    case GLFW_KEY_LEFT_CONTROL:
        cvel -= CAMERA_SPEED_SENSITIVITY;
        if (cvel < 0)
            cvel = 0;
        break;
    case GLFW_KEY_J:
        sensitivity -= DEFAULT_CAMERA_SENSITIVITY;
        if (sensitivity < DEFAULT_CAMERA_SENSITIVITY)
            sensitivity = DEFAULT_CAMERA_SENSITIVITY;
        break;
    case GLFW_KEY_K:
        sensitivity += DEFAULT_CAMERA_SENSITIVITY;
        break;
    default:
        break;
    }
}

void Camera::handleMouseOffset(double dx, double dy) {
    if (dx != 0 || dy != 0) {
        // �����������ת
        float dyaw = dx * DEFAULT_CAMERA_SENSITIVITY;
        float dpitch = dy * DEFAULT_CAMERA_SENSITIVITY;

        CEuler e(dyaw / PI * 180, dpitch / PI * 180, 0);

        if (VIEWPOINT_MODE == Child) {
            CVector u = front.crossMul(up).normalize();
            CMatrix mat = CMatrix::rotateMatrix(dyaw, up)
                * CMatrix::rotateMatrix(dpitch, u);
            CVector tmp = mat.vecMul(front);
            if (tmp.crossMul(up).len() < 0.1f) {
                return;
            }
            front = tmp;


            // ����������������ŷ���ǣ�ʹ�л�ƽ��
            euler = euler + e;
            euler.normalize();
        }
        else {
            euler = euler + e;
            euler.normalize();

            if (euler.p > 80) euler.p = 80;
            if (euler.p < -80) euler.p = -80;
            CMatrix mat = euler.toMatrix();
            front = mat.vecMul(CVector(0, 0, -1));
        }
    }
}

void Camera::updateView() {
    view = CMatrix::lookAtMatrix(getPos(), front + getPos(), up);

    if (DEBUG_INFO) {
        std::cout << "=========================================\n";
        if (viewpoint) {
            std::cout << "Camera Vel: " << cvel << std::endl;
            std::cout << "Camera Sensitivity: " << sensitivity << std::endl;
        }
        std::cout << "Camera Position: " << getPos().toString() << std::endl;
        std::cout << "Camera Euler: " << euler.toString() << std::endl;
        std::cout << "Camera Quaternion: " << euler.toQuaternion().toString() << std::endl;
        std::cout << "Camera Front: " << front.toString() << std::endl;
        std::cout << "Camera Up: " << up.toString() << std::endl;
    }
}

void Camera::changeViewpointMode() {
    VIEWPOINT_MODE = (VIEWPOINT_MODE + 1) % All;
    if (VIEWPOINT_MODE == Euler) {
        std::cout << "�л���ŷ����ģʽ" << std::endl;
    }
    else if (VIEWPOINT_MODE == Child) {
        std::cout << "�л��������귽ʽ" << std::endl;
    }
}