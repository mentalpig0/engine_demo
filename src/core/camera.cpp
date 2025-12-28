#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 pos)
    : position(pos), worldUp(glm::vec3(0.0f, 1.0f, 0.0f)) {
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    updateVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

void Camera::moveForward(float speed) {
    position += front * speed;
}

void Camera::moveBackward(float speed) {
    position -= front * speed;
}

void Camera::moveLeft(float speed) {
    position -= right * speed;
}

void Camera::moveRight(float speed) {
    position += right * speed;
}

void Camera::moveUp(float speed) {
    position += worldUp * speed;
}

void Camera::moveDown(float speed) {
    position -= worldUp * speed;
}

void Camera::look(float xoffset, float yoffset) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Constrain pitch to prevent flipping
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    updateVectors();
}

void Camera::updateVectors() {
    // Calculate new front vector
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    // Recalculate right and up vectors
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
