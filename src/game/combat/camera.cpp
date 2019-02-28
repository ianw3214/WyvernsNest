#include "camera.hpp"

Camera::Camera()
: viewMatrix{ 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f }
, position{ 0.f, 0.f, 1.f } { 
    update();
}

void Camera::update() {
    viewMatrix = {
        position[2], 0.f, 0.f, -position[0],
        0.f, position[2], 0.f, -position[1],
        0.f, 0.f, 1.f, 0,
        0.f, 0.f, 0.f, 1.f
    };
}

void Camera::setPosition(float x, float y, float z) {
    position[0] = x;
    position[1] = y;
    position[2] = z;

    update();
}