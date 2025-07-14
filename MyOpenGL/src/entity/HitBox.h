#pragma once

#include "Entity.h"
#include "../geometry/Cuboid.h"

#include <chrono>

struct PlayerCollisionInfo {
	std::string collisionWith;
	CVector playerPosition; // 玩家位置
	CVector otherPosition; // 碰撞物体位置
	CVector collisionPoint; // 碰撞点

	PlayerCollisionInfo(const char* collisionWith, const CVector& playerPosition, const CVector& otherPosition, const CVector& collisionPoint)
		: collisionWith(collisionWith), playerPosition(playerPosition), otherPosition(otherPosition), collisionPoint(collisionPoint) {}
};

struct SpaceCraftCollisionInfo {
	std::string collisionWith;
	std::time_t collisionTime;
	CVector collisionPoint; // 星球坐标

	SpaceCraftCollisionInfo(const char* collisionWith, const CVector& collisionPoint)
		: collisionWith(collisionWith), collisionPoint(collisionPoint) {
		collisionTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	}
};

inline std::unique_ptr<PlayerCollisionInfo> playerCollisionInfo = nullptr;
inline std::unique_ptr<SpaceCraftCollisionInfo> spaceCraftCollisionInfo = nullptr;

class HitBox : public Entity {
public:
	HitBox() = default;
	HitBox(const Cuboid& box);
	HitBox(const CVector& pos, const CVector& axis, float l, float w, float h);
	~HitBox() override = default;

	void set(const CVector& pos, const CVector& axis, float l, float w, float h);

	bool checkCollision(const Ball& ball) const;
	bool checkCollision(const HitBox& other) const;
	bool checkCollision(const HitBox& other, CVector& collisionPoint) const;

	Cuboid* getBox() const { return dynamic_cast<Cuboid*>(primitives[0].get()); }

	void draw(bool line) override;
private:
	void createPrimitives() {}
	void createPrimitives(float l, float w, float h);
};