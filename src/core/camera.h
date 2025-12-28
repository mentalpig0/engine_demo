#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f));

    // Get view matrix
    glm::mat4 getViewMatrix() const;

    // Basic movement
    void moveForward(float speed);
    void moveBackward(float speed);
    void moveLeft(float speed);
    void moveRight(float speed);
    void moveUp(float speed);
    void moveDown(float speed);

    // Mouse look
    void look(float xoffset, float yoffset);

    // Get position
    glm::vec3 getPosition() const { return position; }

    // Camera speed
    float movementSpeed = 5.0f;
    float mouseSensitivity = 0.1f;

private:
    void updateVectors();

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw = -90.0f;   // Start looking along -Z axis
    float pitch = 0.0f;
};

#endif // CAMERA_H
