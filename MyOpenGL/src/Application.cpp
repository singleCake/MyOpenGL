#include <thread>

#include "Application.h"

#include "manager/InputManager.h"
#include "manager/EntityManager.h"
#include "manager/CameraManager.h"
#include "manager/BackgroundManager.h"
#include "manager/RenderManager.h"
#include "manager/LightManager.h"
#include "manager/TextureManager.h"

GLFWwindow* Application::window = NULL;

Application::Application() 
    : FPS(30)
{
    if (!init()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(-1);
    }
    printInfo();
}

Application::~Application() {
	glfwTerminate();
    EntityManager::close();
    BackgroundManager::close();
    TextureManager::close();
}

bool Application::init() {
    /* Initialize the library */
    if (!glfwInit())
        return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "My OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return false;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Initialize textures */
    if (!TextureManager::init()) {
        std::cerr << "Failed to initialize textures." << std::endl;
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    srand(time(NULL));

    InputManager::init();
    EntityManager::init();
    CameraManager::init();
    BackgroundManager::init();
    LightManager::init();

    return true;
}

void Application::printInfo() const {
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLFW Version: " << glfwGetVersionString() << std::endl;
    std::cout << "FPS: " << FPS << std::endl;
}

void Application::update(float dt) {    
    EntityManager::update(dt);
    CameraManager::update(dt);
    BackgroundManager::update(dt);
    LightManager::update(dt);
}

static inline void handleInput() {
    InputManager::handleGlobalEvent();

    InputManager::handleAllKeyClick(
        [](int keyCode) {
			EntityManager::handleKeyClick(keyCode);
		}
	);

    InputManager::handleMouseMove(
        [](double x, double y) {
			EntityManager::handleMouseMove(x, y);
		}
	);

    InputManager::handleMouseOffset(
        [](double dx, double dy) {
            CameraManager::handleMouseOffset(dx, dy);
            EntityManager::handleMouseOffset(dx, dy);
		}
	);

    InputManager::resetMousePosition(); // 重置鼠标位置

    InputManager::handleMouseClick(
        [](double x, double y) {
            EntityManager::handleMouseClick(x, y);
        }
    );

    InputManager::handleMouseScroll(
        [](double y) {
		}
	);

    InputManager::handleAllKeyPress(
        [](int keyCode) {
            EntityManager::handleKeyPress(keyCode);
            CameraManager::handleKeyInput(keyCode);
        }
    );
}

void Application::run() {
    const double targetFrameTime = 1.0 / FPS;
    double lastTime = glfwGetTime();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        double elapsedTime = currentTime - lastTime;

        // 等待至下一帧时间到达
        if (elapsedTime < targetFrameTime) {
            std::this_thread::sleep_for(
                std::chrono::duration<double>(targetFrameTime - elapsedTime)
            );
        }

        // 更新计时器
        lastTime = glfwGetTime();

        /* Update */
        update(0.1);

        /* Render here */
        RenderManager::render();

        /* Handle input */
        handleInput();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
}