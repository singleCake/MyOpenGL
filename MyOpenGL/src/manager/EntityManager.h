#pragma once

#include "../entity/astronaut/Player.h"
#include "../entity/SpaceCraft.h"
#include <memory>

using SpaceCraftUPtr = std::unique_ptr<SpaceCraft>;
using PlayerUPtr = std::unique_ptr<Player>;
using AstronautUPtr = std::unique_ptr<Astronaut>;
using PlanetUPtr = std::unique_ptr<Planet>;

class EntityManager {
public:
	static const int PLANET_NUM = 10;
	static bool SHOW_HITBOX;
	static bool TIME_FREEZE;

	EntityManager() = default;
	~EntityManager() = default;

	static void init();
	static void update(float dt);
	static void handleMouseMove(double x, double y);
	static void handleMouseOffset(double dx, double dy);
	static void handleMouseClick(double x, double y);
	static void handleKeyPress(int keyCode);
	static void handleKeyClick(int keyCode);
	static void draw();

	static void close();

	static SpaceCraftUPtr spaceCraft;
	static PlayerUPtr player;
	static std::vector<PlanetUPtr> planets;
	static std::vector<AstronautUPtr> astronauts;
private:

	static PlanetUPtr* selectedPlanet;

	static PlanetUPtr* detectHoverStar(double mouseX, double mouseY);

	static void initPlanet();
	static void initSpaceCraft();
};