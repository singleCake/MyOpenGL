#include "BackgroundManager.h"
#include "../constant/BackgroundConstant.h"

StarsUPtr BackgroundManager::stars;

void BackgroundManager::init() {
	initStar();
}

void BackgroundManager::initStar() {
	stars = std::make_unique<Star[]>(STAR_NUM);
	for (int i = 0; i < STAR_NUM; i++) {
		CVector position = CVector(
			(float)rand() / RAND_MAX * STAR_RANGE * 2 - STAR_RANGE,
			(float)rand() / RAND_MAX * STAR_RANGE * 2 - STAR_RANGE,
			(float)rand() / RAND_MAX * STAR_RANGE * 2 - STAR_RANGE);
		float phi = (float)rand() / RAND_MAX * 2 * PI;
		float size = (float)rand() / RAND_MAX * 64;
		float vel = (float)rand() / RAND_MAX / 2;
		bool clockwise = rand() % 2 == 0;
		stars.get()[i].set(phi, position, size, vel, clockwise);
	}
}

void BackgroundManager::update(float dt) {
	for (int i = 0; i < STAR_NUM; i++) {
		stars.get()[i].update(dt);
	}
}

void BackgroundManager::draw() {
	for (int i = 0; i < STAR_NUM; i++) {
		stars[i].draw();
	}
}

void BackgroundManager::close() {
}