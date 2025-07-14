#include "Planet.h"
#include "../../manager/InputManager.h"
#include "../../manager/RenderManager.h"
#include "../../geometry/Ball.h"

#include <iostream>

void Planet::createPrimitives() {
	primitives.clear();
	primitives.reserve(3);

	// default ball
	primitives.emplace_back(std::make_unique<Ball>(getPos(), radius, color, src));

	// hover ball
	primitives.emplace_back(std::make_unique<Ball>(
		getPos(), radius * LINE_BALL_SCALE, CVector(HOVER_COLOR), src
	));

	// select ball
	primitives.emplace_back(std::make_unique<Ball>(
		getPos(), radius * LINE_BALL_SCALE, CVector(SELECT_COLOR), src
	));
}

Planet::Planet(float r, const CVector& color, bool src)
	: Planet(DEFAULT_ANCHOR, r, DEFAULT_AUTO_ROTATION, color, src) {}

Planet::Planet(float r, float autoRotation, const CVector& color, bool src)
	: Planet(DEFAULT_ANCHOR, r, autoRotation, color, src) {}

Planet::Planet(CVector& anchor, float r, const CVector& color, bool src)
	: Planet(anchor, r, DEFAULT_AUTO_ROTATION, color, src) {}

Planet::Planet(CVector& anchor, float r, float autoRotation, const CVector& color, bool src)
	: anchor(anchor), autoRotation(autoRotation), radius(r), color(color), src(src) {

	up = axis;
	offset = radius * 1.5f;
	createPrimitives();
	init();
}

void Planet::init() {
	orbitalColor = DEFAULT_ORBIT_COLOR 
		+ CVector((float)(rand() % 10 ) / 100, (float)(rand() % 10) / 100, (float)(rand() % 10) / 100);

	CVector r = this->getPos() - anchor;
	orbitalRadius = r.len();
	rotationAxis = (r.crossMul(this->getVel())).normalize();
	omega = this->getVel().len() / orbitalRadius;
	lastAnchor = anchor;
}

void Planet::update(float dt) {
	// 公转
	if (omega > 0) {
		setPos(getPos() + anchor - lastAnchor);
		rotate(omega * dt, rotationAxis, anchor);
		lastAnchor = anchor;
	}

	// 自转
	rotate(autoRotation * dt, CVector(0, 1, 0));
}

void Planet::draw(bool line) {
	RenderManager::drawCircle(orbitalColor, anchor, rotationAxis, orbitalRadius);
	primitives[DEFAULT]->drawWithTexture(getID());
	if (hovered && !selected)
		primitives[HOVER]->draw(true);
	if (selected)
		primitives[SELECT]->draw(true);
}

bool Planet::raySphereIntersect(const CVector& rayOrigin, const CVector& rayDir, float& t) const {
	return dynamic_cast<Ball*>(primitives[DEFAULT].get())->raySphereIntersect(rayOrigin, rayDir, t);
}