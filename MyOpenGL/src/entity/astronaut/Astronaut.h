#pragma once

#include "../Entity.h"
#include "../HitBox.h"

class Astronaut : public Entity {
public:
	Astronaut();
	~Astronaut() override = default;

	virtual void rotateMat(const CMatrix& mat) override;
	virtual void setPos(const CVector& pos) override;

	virtual void update(float dt) override;
	virtual void handleInput(int keyCode) {}
	void draw(bool line) override;

	const HitBox& getHitBox() const { return hitbox; }
	virtual const char* getID() const = 0;
	virtual const char* getName() const = 0;

protected:
	enum { HEAD, BODY, LEFT_ARM, RIGHT_ARM, LEFT_LEG, RIGHT_LEG, ALL_PART };

	CVector towardVel, leftVel;
	bool moving = false;

	void move(float dt);
	void yaw(float angle);

	virtual bool checkCollision() const;

private:
	// 控制四肢运动
	float arm_omega = PLAYER_ARM_OMEGA;
	float arm_angle = 0;

	HitBox hitbox;
	void createPrimitives() override;
};