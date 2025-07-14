#include "Application.h"

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);

	Application app;

	app.run();
}