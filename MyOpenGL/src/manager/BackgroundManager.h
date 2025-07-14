#pragma once

#include "../background/Star.h"
#include <memory>

using StarsUPtr = std::unique_ptr<Star[]>;

class BackgroundManager {
public:
	BackgroundManager() = default;
	~BackgroundManager() = default;

	static void init();

	static void update(float dt);
	static void draw();
	static void close();


private:
	static StarsUPtr stars;

	static void initStar();
};