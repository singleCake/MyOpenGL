#include "Astronaut.h"

#include "../../geometry/Ball.h"
#include "../../geometry/Cuboid.h"
#include "../../geometry/Stick.h"

#include "../../manager/EntityManager.h"
#include <iostream>

Astronaut::Astronaut() {
	center = getPos() + this->axis * (PLAYER_HEIGHT / 2);
	toward = this->axis.crossMul(CVector(1, 0, 0)).normalize();
	hitbox.set(center, this->axis,
		PLAYER_WIDTH, PLAYER_WIDTH, PLAYER_HEIGHT
	);
	createPrimitives();
}

void Astronaut::createPrimitives() {
	CVector toward = this->axis.crossMul(CVector(1, 0, 0)).normalize();
	CVector left = this->axis.crossMul(toward).normalize();

	// 头
	primitives.emplace_back(std::make_unique<Ball>(
		center + axis * (PLAYER_SIZE * 1.01f),
		PLAYER_SIZE / 2
	));

	// 身体
	primitives.emplace_back(std::make_unique<Cuboid>(
		center, axis, PLAYER_BODY_LENGTH, PLAYER_BODY_WIDTH, PLAYER_BODY_HEIGHT, GREY
	));

	// 手臂
	primitives.emplace_back(std::make_unique<Stick>(
		center + left * (PLAYER_ARM_OFFSET)+axis * (PLAYER_SIZE * 0.45f),
		center + left * (PLAYER_ARM_OFFSET * 1.2f) - axis * (PLAYER_SIZE * 0.65f),
		left, PLAYER_ARM_RADIUS, GREEN
	));

	primitives.emplace_back(std::make_unique<Stick>(
		center - left * (PLAYER_ARM_OFFSET)+axis * (PLAYER_SIZE * 0.45f),
		center - left * (PLAYER_ARM_OFFSET * 1.2f) - axis * (PLAYER_SIZE * 0.65f),
		left, PLAYER_ARM_RADIUS, GREEN
	));

	// 腿
	primitives.emplace_back(std::make_unique<Stick>(
		center + left * (PLAYER_LEG_OFFSET)-axis * (PLAYER_SIZE * 0.51f),
		getPos() + left * (PLAYER_LEG_OFFSET)+axis * (PLAYER_SIZE * 0.01f),
		left, PLAYER_LEG_RADIUS, BLUE
	));

	primitives.emplace_back(std::make_unique<Stick>(
		center - left * (PLAYER_LEG_OFFSET)-axis * (PLAYER_SIZE * 0.51f),
		getPos() - left * (PLAYER_LEG_OFFSET)+axis * (PLAYER_SIZE * 0.01f),
		left, PLAYER_LEG_RADIUS, BLUE
	));
}

void Astronaut::rotateMat(const CMatrix& rotation) {
	Entity::rotateMat(rotation);
	hitbox.rotateMat(rotation);
}

void Astronaut::setPos(const CVector& pos) {
	Entity::setPos(pos);
	hitbox.setPos(center);
}

void Astronaut::update(float dt) {
	if (checkCollision()) {
		setVel(-towardVel - leftVel);
	}
	else if (!moving) {
		towardVel = leftVel = CVector(0, 0, 0);
		setVel(CVector(0, 0, 0));
	}
	else {
		setVel(towardVel + leftVel);
	}

	move(dt);

	moving = false;

	Entity::update(dt);
	hitbox.setPos(center);
}

void Astronaut::draw(bool line) {
	std::string name = getID();
	primitives[HEAD]->drawWithTexture((name + "_head").c_str());

	dynamic_cast<Cuboid*>(primitives[BODY].get())->drawWithTwoTextures((name + "_front").c_str(), (name + "_back").c_str());

	primitives[LEFT_ARM]->drawWithTexture((name + "_arm").c_str());
	primitives[RIGHT_ARM]->drawWithTexture((name + "_arm").c_str());

	primitives[LEFT_LEG]->drawWithTexture((name + "_leg").c_str());
	primitives[RIGHT_LEG]->drawWithTexture((name + "_leg").c_str());

	hitbox.draw(true);
}

void Astronaut::move(float dt) {
	if (moving || arm_angle > 1e-5 || arm_angle < 0) {
		arm_angle += arm_omega * dt;
		if (fabs(arm_angle) >= PLAYER_ARM_MAX_ANGLE) {
			arm_omega = -arm_omega;
		}
		dynamic_cast<Stick*>(primitives[RIGHT_ARM].get())->rotate(arm_omega * dt);
		dynamic_cast<Stick*>(primitives[LEFT_ARM].get())->rotate(-arm_omega * dt);
		dynamic_cast<Stick*>(primitives[RIGHT_LEG].get())->rotate(-arm_omega * dt);
		dynamic_cast<Stick*>(primitives[LEFT_LEG].get())->rotate(arm_omega * dt);
	}
}

void Astronaut::yaw(float angle) {
	rotate(angle, axis);
}

bool Astronaut::checkCollision() const {
	return hitbox.checkCollision(EntityManager::spaceCraft->getHitBox()) || 
		hitbox.checkCollision(EntityManager::player->getHitBox());
}