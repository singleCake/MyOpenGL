#include "CameraManager.h"
#include "../math/CEuler.h"
#include "../math/CQuaternion.h"

unsigned int CameraManager::index = 0;
CameraUPtr CameraManager::default_camera = std::make_unique<DefaultCamera>();
std::vector<CameraUPtr*> CameraManager::all_cameras;
Camera* CameraManager::camera = nullptr;
CMatrix CameraManager::projection;

void CameraManager::init() {
    all_cameras.emplace_back(&default_camera);
    camera = all_cameras.back()->get();
	myReShape();
}

void CameraManager::update(float dt) {
    camera->update(dt);
}

void CameraManager::myReShape() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    projection = CMatrix::perspectiveMatrix(FOV, 1.0f, 0.01f, 100.0f);
    glLoadMatrixf(projection);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void CameraManager::handleKeyInput(int keyCode) {
    camera->handleKeyInput(keyCode);
}

void CameraManager::handleMouseOffset(double dx, double dy) {
    camera->handleMouseOffset(dx, dy);
}

void CameraManager::bindCameraToPlanet(Planet* planet) {
    if (getCameraIDEqualTo("Default")) {
        camera->bindViewPoint(planet);
    }
}

void CameraManager::switchCamera() {
    index = (index + 1) % all_cameras.size();
    camera = all_cameras[index]->get();
}

void CameraManager::addCamera(CameraUPtr* camera) {
	all_cameras.emplace_back(camera);
}

void CameraManager::removeEmptyCameras() {
	all_cameras.erase(std::remove_if(all_cameras.begin(), all_cameras.end(),
        		[](const std::unique_ptr<Camera>* cam) { return cam->get() == nullptr; }), all_cameras.end());
    if (camera == nullptr) {
        camera = all_cameras[0]->get(); // 保证默认相机不会删除
    }
}