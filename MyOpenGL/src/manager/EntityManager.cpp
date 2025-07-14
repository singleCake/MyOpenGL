#include "EntityManager.h"
#include "InputManager.h"
#include "CameraManager.h"

#include "../entity/planet/all_planet.h"
#include "../entity/astronaut/all_astronauts.h"

#include "../constant/Color.h"
#include <iostream>

bool EntityManager::SHOW_HITBOX = false;
bool EntityManager::TIME_FREEZE = false;

PlayerUPtr EntityManager::player = nullptr;
SpaceCraftUPtr EntityManager::spaceCraft = nullptr;
std::vector<AstronautUPtr> EntityManager::astronauts;
std::vector<PlanetUPtr> EntityManager::planets;
PlanetUPtr* EntityManager::selectedPlanet = nullptr;

static std::pair<CVector, CVector> generateRandomPosAndSpeed(float& R) {
	while (true) {
		CVector pos = CVector((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
		CVector vel = CVector((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
		if (pos.crossMul(vel).len() > 0.1f) {
			float v = (float)rand() / RAND_MAX / 2 + DEFAULT_PLANET_VEL;
			pos = pos.normalize() * R;
			vel = vel.normalize() * v;
			R += ORBIT_RADIUS;
			return std::make_pair(pos, vel);
		}
	}
}

template<typename Name>
static void addPlanet(std::vector<PlanetUPtr>& planets, float& radius) {
	auto [pos, vel] = generateRandomPosAndSpeed(radius);
	planets.emplace_back(std::make_unique<Name>(pos, vel, DEFAULT_PLANET_RADIUS + (float)(rand() % 10) / 100));
}

void EntityManager::init() {
	initPlanet();
}

void EntityManager::initPlanet() {
	planets.reserve(PLANET_NUM);

	// 太阳
	planets.emplace_back(
		std::make_unique<Sun>(CVector(0, 0, 0), CVector(0, 0, 0),SUN_RADIUS, true)
	);

	float R = ORBIT_RADIUS * 2;

	// 水星金星地球
	addPlanet<Mercury>(planets, R);
	addPlanet<Venus>(planets, R);
	addPlanet<Earth>(planets, R);

	Planet* earth = planets.back().get();

	// 月球
	float moonR = earth->getRadius() * MOON_ORBIT_SCALE;
	auto [pos, vel] = generateRandomPosAndSpeed(moonR);
	pos += earth->getPos();
	planets.emplace_back(
		std::make_unique<Moon>(
			pos, vel * MOON_VEL_SCALE, MOON_RADIUS, earth->getPosRef()
		)
	);

	// 火星木星土星天王星海王星
	addPlanet<Mars>(planets, R);
	addPlanet<Jupiter>(planets, R);
	addPlanet<Saturn>(planets, R);
	addPlanet<Uranus>(planets, R);
	addPlanet<Neptune>(planets, R);
}

void EntityManager::initSpaceCraft() {
	CVector vel = CVector((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	vel = vel.normalize() * DEFAULT_SPACE_CRAFT_SPEED;
	// 从地球发射一个飞船，速度方向随机
	spaceCraft = std::make_unique<SpaceCraft>(
		planets[3]->getPos(), vel, DEFAULT_SPACE_CRAFT_SIZE
	);

	player = std::make_unique<Player>();
	spaceCraft->addChildren(player.get(), 0.2f, 0.2f);

	astronauts.emplace_back(std::make_unique<Soyo>());
	
	spaceCraft->addChildren(astronauts.back().get(), -0.2f, -0.2f);

	std::cout << "SpaceCraft Launched!" << std::endl;
}

void EntityManager::update(float dt) {
	if (spaceCraft) {
		spaceCraft->update(dt);
	}

	if (TIME_FREEZE) return;

	for (auto& star : planets) {
		star->update(dt);
	}
}

void EntityManager::handleMouseMove(double x, double y) {
	for (auto& star : planets) {
		star->cancelHover();
	}

	if (CameraManager::getCameraIDEqualTo("Default")) {
		PlanetUPtr* hoveredPlanet = detectHoverStar(x, y);
		if (hoveredPlanet) {
			(*hoveredPlanet)->hover();
		}
	}
}

void EntityManager::handleMouseOffset(double dx, double dy) {
	if (player)
		player->handleMouseOffset(dx, dy);
	if (spaceCraft) {
		spaceCraft->handleMouseOffset(dx, dy);
	}
}

void EntityManager::handleMouseClick(double x, double y) {
	for (auto& star : planets) {
		star->cancelSelect();
	}

	selectedPlanet = detectHoverStar(x, y);
	if (selectedPlanet) {
		(*selectedPlanet)->select();
	}
}

void EntityManager::handleKeyPress(int keyCode) {
	if (spaceCraft) {
		spaceCraft->handleKeyPress(keyCode);	
	}

	if (player) {
		player->handleInput(keyCode);
	}

	for (auto& astronaut : astronauts) {
		astronaut->handleInput(keyCode);
	}
}

void EntityManager::handleKeyClick(int keyCode) {
	if (spaceCraft) {
		spaceCraft->handleKeyClick(keyCode);
		
		if (keyCode == GLFW_KEY_ENTER && selectedPlanet) {
			spaceCraft->setTarget(selectedPlanet);
		}
	}
	else if (keyCode == GLFW_KEY_ENTER) {
		initSpaceCraft();
	}
	if (keyCode == GLFW_KEY_SPACE) {
		if (selectedPlanet)
			CameraManager::bindCameraToPlanet(selectedPlanet->get());
		else 
			CameraManager::bindCameraToPlanet(nullptr);
	}
}

void EntityManager::draw() {
	for (auto& star : planets) {
		star->draw(false);
	}

	if (spaceCraft) {
		spaceCraft->draw(false);
	}
}

void EntityManager::close() {
	planets.clear();
	astronauts.clear();
}

PlanetUPtr* EntityManager::detectHoverStar(double mouseX, double mouseY) {
	CVector rayOrigin, rayDir;
	InputManager::getPickingRay(mouseX, mouseY, rayOrigin, rayDir);

	PlanetUPtr* selectedStar = nullptr;
	float minDistance = FLT_MAX;

	for (auto& star : planets) {
		float t;
		if (star->raySphereIntersect(rayOrigin, rayDir, t)) {
			if (t < minDistance) {
				minDistance = t;
				selectedStar = &star;
			}
		}
	}

	return selectedStar;
}