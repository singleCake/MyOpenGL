#include "AbstractEntity.h"

void AbstractEntity::changeVelicity(float dv) {
	float v = vel.len();
	if (v + dv > 0.001) {
		v += dv;
	}
	else {
		v = 0.001;
	}
	vel = vel.normalize() * v;
}

void AbstractEntity::changeVelicity(float dx, float dy, float dz) {
	float v = vel.len();
	vel.x += dx * v;
	vel.y += dy * v;
	vel.z += dz * v;
	vel = vel.normalize() * v;
}

void AbstractEntity::rotateMat(const CMatrix& mat) {
	vel = mat.vecMul(vel);
	axis = mat.vecMul(axis);
	toward = mat.vecMul(toward);
	pos = mat.posMul(pos);
	center = mat.posMul(center);
}

void AbstractEntity::rotate(float angle, const CVector& axis) {
	rotate(angle, axis, pos);
}

void AbstractEntity::rotate(float angle, const CVector& axis, const CVector& anchor) {
	rotateMat(CMatrix::rotateMatrix(angle, axis, anchor));
}

void AbstractEntity::rotate(const CVector& v) {
	rotateMat(CMatrix::rotateMatrix(axis, v, pos));
}

void AbstractEntity::update(float dt) {
	pos += vel * dt;
	center += vel * dt;
}

void AbstractEntity::setPos(const CVector& pos) {
	center = center + (pos - this->pos);
	this->pos = pos;
}