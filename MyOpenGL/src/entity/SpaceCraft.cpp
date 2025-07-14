#include "SpaceCraft.h"
#include "../manager/RenderManager.h"
#include "../manager/LightManager.h"

#include "../geometry/BoxWithBorder.h"
#include "../geometry/Cone.h"
#include "../geometry/Pyramid.h"

#include "../constant/Color.h"

#include <iostream>
#include "../manager/EntityManager.h"
#include "abstract/camera/SpaceCraftCamera.h"
#include "../manager/CameraManager.h"

// pos 为机舱底部中心位置
inline static void createTail(
	std::vector<PrimitiveUPtr>& primitives,
	const CVector& pos, const CVector& u, const CVector& v, const CVector& w,
	float width, float length,
	const CVector& color) {
	float offset = width * TAIL_OFFSET_SCALE;
	primitives.emplace_back(std::make_unique<Pyramid>(
		pos - u * offset - w * offset,
		u, -v, w, width, length, width, color
	));

	primitives.emplace_back(std::make_unique<Pyramid>(
		pos + u * offset - w * offset,
		-u, -v, w, width, length, width, color
	));

	primitives.emplace_back(std::make_unique<Pyramid>(
		pos - u * offset + w * offset,
		u, -v, -w, width, length, width, color
	));

	primitives.emplace_back(std::make_unique<Pyramid>(
		pos + u * offset + w * offset,
		-u, -v, -w, width, length, width, color
	));
}

// pos 为机舱中心位置
inline static void createWing(
	std::vector<PrimitiveUPtr>& primitives,
	const CVector& pos, const CVector& u, const CVector& v, const CVector& w,
	float size, float width, float length,
	const CVector& color) {
	primitives.emplace_back(std::make_unique<Pyramid>(
		pos + u * size * 0.51f - v * length * 0.5f,
		u, v, w, width, length, WING_SIZE, color
	));

	primitives.emplace_back(std::make_unique<Pyramid>(
		pos - u * size * 0.51f - v * length * 0.5f,
		-u, v, w, width, length, WING_SIZE, color
	));
}

void SpaceCraft::createPrimitives() {
	CVector u = axis.crossMul(CVector(1, 0, 0)).normalize();
	CVector v = axis.crossMul(u).normalize();

	float length = size * LENGTH_SCALE;

	// 机舱
	primitives.emplace_back(std::make_unique<BoxWithBorder>(
		getPos(), v, size, length, size, BODY_COLOR
	));

	hitbox.set(getPos(), v, size, length, size);
	children.push_back(&hitbox);

	// 头部
	primitives.emplace_back(std::make_unique<Cone>(
		getPos() + axis * length * 0.51f, axis, 
		size * HEAD_RADIUS_SCALE, size * HEAD_HEIGHT_SCALE, 
		HEAD_COLOR
	));

	// 尾部
	createTail(
		primitives,
		getPos() - axis * length * 0.51f, u, axis, v,
		size * TAIL_WIDTH_SCALE, size * TAIL_LENGTH_SCALE, TAIL_COLOR
	);

	// 机翼
	createWing(
		primitives,
		getPos(), u, axis, v,
		size, size * WING_WIDTH_SCALE, size * WING_LENGTH_SCALE, WING_COLOR
	);
}

SpaceCraft::SpaceCraft(const CVector& pos, const CVector& vel, float size)
	: AbstractEntity(pos, vel, vel), size(size) {
	createPrimitives();

	center = pos;
	front = toward = axis;
	euler = front.toEuler();
	left = -axis.crossMul(CVector(1, 0, 0).normalize());
	up = axis.crossMul(left).normalize();
	fixedUp = true;

	pointLight = pos + axis * (size * LENGTH_SCALE * LIGHT_SCALE);
	spotLight = pos + axis * (size * (LENGTH_SCALE * 0.51f + HEAD_HEIGHT_SCALE));
	LightManager::initCabinLight(&pointLight);
	LightManager::initSpotLight(&spotLight, &axis);

	createCamera<SpaceCraftCamera>();
}

void SpaceCraft::update(float dt) {
	if (target) {
		rotate((*target)->getPos() - getPos());
	}
	Entity::update(dt);
	pointLight = getPos() + axis * (size * LENGTH_SCALE * LIGHT_SCALE);
	spotLight = getPos() + axis * (size * (LENGTH_SCALE * 0.51f + HEAD_HEIGHT_SCALE));
	center = getPos();
	for (auto& child : children) {
		child->setPos(child->getPos() + getVel() * dt);
		child->update(dt);
	}

	// 动态检测与星球的碰撞
	checkCollision();
}

void SpaceCraft::handleKeyPress(int keyCode) {
	// 任何时候都可以使飞船加减速，开关飞船灯光
	switch (keyCode) {
	case GLFW_KEY_PAGE_DOWN:
		changeVelicity(-SENSITIVITY);
		break;
	case GLFW_KEY_PAGE_UP:
		changeVelicity(SENSITIVITY);
		break;
	}

	// 只有上帝视角下可用方向键控制飞船方向
	if (!CameraManager::getCameraIDEqualTo("Default")) return;
	switch (keyCode) {
	case GLFW_KEY_UP:
		pitch(SENSITIVITY);
		break;
	case GLFW_KEY_DOWN:
		pitch(-SENSITIVITY);
		break;
	case GLFW_KEY_LEFT:
		yaw(SENSITIVITY);
		break;
	case GLFW_KEY_RIGHT:
		yaw(-SENSITIVITY);
		break;
	}
}

void SpaceCraft::handleKeyClick(int keyCode) {
	switch (keyCode) {
	case GLFW_KEY_1:
		LightManager::toggleCabinLight();
		break;
	case GLFW_KEY_2:
		LightManager::toggleSpotLight();
		break;
	}
}

void SpaceCraft::handleMouseOffset(double dx, double dy) {
	if ((dx == 0 && dy == 0) || target) return;
	if (!CameraManager::getCameraIDEqualTo("SpaceCraft")) return;
	ViewPoint::handleMouseOffset(dx, dy, this);
}

void SpaceCraft::draw(bool line) {
	if (target) {
		RenderManager::drawLine(CVector(1, 1, 1), getPos(), (*target)->getPos());
	}

	primitives[0]->drawWithTexture("stone"); // 机舱

	primitives[1]->drawWithTexture("brick"); // 头部
	
	for (size_t i = 2; i < 6; i++)
		primitives[i]->drawWithTexture("plank"); // 尾部

	for (size_t i = 6; i < 8; i++)
		primitives[i]->drawWithTexture("plank"); // 机翼
	
	for (auto& child : children)
		child->draw(line);
}

void SpaceCraft::pitch(float angle) {
	target = nullptr;
	CVector vel = getVel();
	CVector axis = vel.crossMul(CVector(0, 1, 0));
	rotate(angle, axis);
}

void SpaceCraft::yaw(float angle) {
	target = nullptr;
	CVector vel = getVel();
	CVector axis = CVector(0, 1, 0);
	rotate(angle, axis);
}

void SpaceCraft::rotateMat(const CMatrix& mat) {
	Entity::rotateMat(mat);
	rotateVP(mat);
	pointLight = mat.posMul(pointLight);
	spotLight = mat.posMul(spotLight);

	for (auto& child : children) {
		child->rotateMat(mat);
	}
}

void SpaceCraft::addChildren(AbstractEntity* child, float x, float y) {
	child->rotate(axis);
	child->setPos(getPos() - axis * (size * LENGTH_SCALE * 0.49f) + left * size * y * 0.49f + up * size * x * 0.49f);

	children.push_back(child);
}

void SpaceCraft::checkCollision() const {
	for (auto& planet : EntityManager::planets) {
		if (checkCollision(*planet)) return;
	}
}

bool SpaceCraft::checkCollision(const Planet& planet) const {
	if (hitbox.checkCollision(planet.getBall())) {
		spaceCraftCollisionInfo.reset();
		spaceCraftCollisionInfo = std::make_unique<SpaceCraftCollisionInfo>(
			planet.getName(), planet.getPos()
		);
		return true;
	}
	return false;
}