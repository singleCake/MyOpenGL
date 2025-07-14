#pragma once

#include "../../geometry/Geometry.h"
#include "../../constant/EntityConstant.h"
#include <memory>

class AbstractEntity {
public:
	AbstractEntity() : AbstractEntity(CVector(0, 0, 0), CVector(0, 0, 0)) {}
	AbstractEntity(const CVector& pos, const CVector& vel) : AbstractEntity(pos, vel, CVector(0, 1, 0)) {}
	AbstractEntity(const CVector& pos, const CVector& vel, const CVector& axis)
		: pos(pos), vel(vel), axis(axis) {
		this->axis.normalize();
		this->center = pos;
	}
	virtual ~AbstractEntity() = default;

	virtual void changeVelicity(float dv);
	virtual void changeVelicity(float x, float y, float z);
	virtual void rotateMat(const CMatrix& mat);
	virtual void rotate(float angle, const CVector& axis);
	virtual void rotate(float angle, const CVector& axis, const CVector& anchor);
	virtual void rotate(const CVector& v);

	virtual void update(float dt);
	virtual void draw(bool line) {}

	virtual CVector& getPosRef() { return pos; }
	virtual const CVector& getPos() const { return pos; }
	virtual const CVector& getVel() const { return vel; }
	virtual const CVector& getAxis() const { return axis; }
	virtual const CVector& getToward() const { return toward; }
	virtual const CVector& getCenter() const { return center; }

	virtual void setPos(const CVector& pos);
	virtual void setVel(const CVector& vel) { this->vel = vel; }

protected:
	CVector axis, toward, center;

	CVector* getPosPtr() { return &pos; }
private:
	CVector pos;
	CVector vel;
};