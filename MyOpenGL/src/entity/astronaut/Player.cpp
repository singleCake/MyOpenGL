#include "Player.h"

#include "../abstract/camera/PlayerCamera.h"

#include "../../manager/CameraManager.h"
#include "../../manager/EntityManager.h"

#include "../../math/CEuler.h"

#include "../../geometry/Stick.h"

Player::Player() : Anon(), AbstractEntity() {
	front = toward;
	euler = front.toEuler();
	left = this->axis.crossMul(toward).normalize();
	up = this->axis;
	createCamera<PlayerCamera>();
}

void Player::rotateMat(const CMatrix& rotation) {
	rotateVP(rotation);
	Astronaut::rotateMat(rotation);
}

void Player::update(float dt) {
	Astronaut::update(dt);
}

void Player::handleMouseOffset(double dx, double dy) {
	if (dx == 0 && dy == 0) return;
	if (!CameraManager::getCameraIDEqualTo("Player")) return;
	ViewPoint::handleMouseOffset(dx, dy, this);
}

void Player::setPos(const CVector& pos) {
	Astronaut::setPos(pos);
}

bool Player::checkCollision() const {
	CVector collisionPoint;
	if (getHitBox().checkCollision(EntityManager::spaceCraft->getHitBox(), collisionPoint)) {
		playerCollisionInfo.reset();
		playerCollisionInfo = std::make_unique<PlayerCollisionInfo>(
			"SpaceCraft", getPos(), EntityManager::spaceCraft->getPos(), collisionPoint
		);
		return true;
	}
	else for (auto& astronaut : EntityManager::astronauts) {
		if (getHitBox().checkCollision(astronaut->getHitBox(), collisionPoint)) {
			playerCollisionInfo.reset();
			playerCollisionInfo = std::make_unique<PlayerCollisionInfo>(
				astronaut->getName(), getPos(), astronaut->getPos(), collisionPoint
			);
			return true;
		}
	}
	return false;
}