#include "Entity.h"

void Entity::rotateMat(const CMatrix& mat) {
	AbstractEntity::rotateMat(mat);
	for (auto& p : primitives) {
		p->trans(mat);
	}
}

void Entity::update(float dt) {
	AbstractEntity::update(dt);
	CMatrix mat = CMatrix::transMatrix(getVel() * dt);
	for (auto& p : primitives) {
		p->trans(mat);
	}
}

void Entity::draw(bool line) {
	AbstractEntity::draw(line);
	for (auto& p : primitives) {
		p->draw(line);
	}
}

void Entity::setPos(const CVector& pos) {
	CMatrix mat = CMatrix::transMatrix(pos - getPos());
	for (auto& p : primitives) {
		p->trans(mat);
	}
	AbstractEntity::setPos(pos);
}