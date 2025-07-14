#include "LightManager.h"

#include <iostream>

GLfloat LightManager::sunPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
CVector* LightManager::cabinPos = nullptr;
CVector* LightManager::spotPos = nullptr;
CVector* LightManager::spotDir = nullptr;

std::array<int, 3> LightManager::allLights = { GL_LIGHT0, GL_LIGHT1, GL_LIGHT2 };
std::array<bool, 3> LightManager::lightEnabled = { false, false, false };
std::array<bool, 3> LightManager::lightAvailable = { false, false, false };

void LightManager::init() {
	// 全局环境光
	GLfloat globalAmbient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    initSunLight();
}

void LightManager::initSunLight() {
    // 设置太阳光
    GLfloat sunDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat sunAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat sunSpecular[] = { 1.5f, 1.5f, 1.5f, 1.0f }; // 添加镜面光

    glLightfv(GL_LIGHT0, GL_DIFFUSE, sunDiffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, sunAmbient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, sunSpecular); // 设置镜面反射

    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0f);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.001f); // 减小衰减
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.001f); // 减小衰减

    lightAvailable[0] = true;
	lightEnabled[0] = true;
}

void LightManager::initCabinLight(CVector* cabin) {
    cabinPos = cabin;

    // 设置机舱灯光
	GLfloat cabinDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat cabinAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat cabinSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT1, GL_DIFFUSE, cabinDiffuse);
	glLightfv(GL_LIGHT1, GL_AMBIENT, cabinAmbient);
	glLightfv(GL_LIGHT1, GL_SPECULAR, cabinSpecular);

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 180.0f);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.07f);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.07f);

    lightAvailable[1] = true;
	lightEnabled[1] = true;
}

void LightManager::initSpotLight(CVector* spotPos, CVector* spotDir) {
	LightManager::spotPos = spotPos;
	LightManager::spotDir = spotDir;

	// 设置聚光灯
	GLfloat spotDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat spotAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat spotSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT2, GL_DIFFUSE, spotDiffuse);
	glLightfv(GL_LIGHT2, GL_AMBIENT, spotAmbient);
	glLightfv(GL_LIGHT2, GL_SPECULAR, spotSpecular);

	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45.0f);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 64);
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.05f);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.05f);

	lightAvailable[2] = true;
	lightEnabled[2] = true;
}

void LightManager::update(float dt) {
    if (lightAvailable[0] && lightEnabled[0])
        glLightfv(GL_LIGHT0, GL_POSITION, sunPos);
    if (lightAvailable[1] && lightEnabled[1]) {
        GLfloat cabinPosArray[] = { cabinPos->x, cabinPos->y, cabinPos->z, 1.0f };
        glLightfv(GL_LIGHT1, GL_POSITION, cabinPosArray);
    }
	if (lightAvailable[2] && lightEnabled[2]) {
		GLfloat spotPosArray[] = { spotPos->x, spotPos->y, spotPos->z, 1.0f };
		GLfloat spotDirArray[] = { spotDir->x, spotDir->y, spotDir->z };
		glLightfv(GL_LIGHT2, GL_POSITION, spotPosArray);
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDirArray);
	}
}

void LightManager::enableAllLights() {
    glEnable(GL_LIGHTING);
    if (lightEnabled[0]) {
		glEnable(GL_LIGHT0);
	}
    if (lightEnabled[1]) {
		glEnable(GL_LIGHT1);
	}
    if (lightEnabled[2]) {
		glEnable(GL_LIGHT2);
	}
}

void LightManager::disableAllLights() {
    glDisable(GL_LIGHTING);
    if (lightAvailable[0]) {
		glDisable(GL_LIGHT0);
	}
    if (lightAvailable[1]) {
		glDisable(GL_LIGHT1);
	}
    if (lightAvailable[2]) {
		glDisable(GL_LIGHT2);
	}
}

void LightManager::toggleCabinLight() {
	if (lightAvailable[1]) {
		lightEnabled[1] = !lightEnabled[1];
		std::cout << "飞船灯光：" << (lightEnabled[1] ? "开" : "关") << std::endl;
	}
}

void LightManager::toggleSpotLight() {
	if (lightAvailable[2]) {
		lightEnabled[2] = !lightEnabled[2];
		std::cout << "聚光灯：" << (lightEnabled[2] ? "开" : "关") << std::endl;
	}
}