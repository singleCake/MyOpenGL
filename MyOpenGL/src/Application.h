#pragma once
#include <GLFW/glfw3.h>
#include "../glut.h"
#include <iostream>

#include "geometry/Geometry.h"

class Application {
public:
	static const int WIDTH = 1080;
	static const int HEIGHT = 1080;

	Application();
	~Application();

	void run();

private:
	bool init();
	void update(float dt);
	void printInfo() const;
public:
	static GLFWwindow* window;

private:
	int FPS;
};