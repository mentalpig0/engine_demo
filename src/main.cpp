#include "renderer.h"
#include <camera.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>

const int SCRN_WIDTH = 1000;
const int SCRN_HEIGHT = 1000;

struct Sphere {
    glm::vec3 position;
    float radius;
    glm::vec3 color;

    Sphere(glm::vec3 pos = glm::vec3(0.0f), float rad = 0.5f, glm::vec3 col = glm::vec3(1.0f, 0.5f, 0.2f))
        : position(pos), radius(rad), color(col) {}

    glm::mat4 getModelMatrix() const {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::scale(model, glm::vec3(radius * 2.0f)); // Diameter
        return model;
    }
};

void addVertex(std::vector<float>& vertices, const glm::vec3& pos, const glm::vec3& color) {
    vertices.push_back(pos.x);
    vertices.push_back(pos.y);
    vertices.push_back(pos.z);
    vertices.push_back(color.r);
    vertices.push_back(color.g);
    vertices.push_back(color.b);
}

std::vector<float> generateSphereVertices(float radius = 0.5f, int sectors = 32, int stacks = 16) {
    std::vector<float> vertices;
    const float PI = 3.14159265358979323846f;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> colors;

    for (int i = 0; i <= stacks; ++i) {
        float stackAngle = PI / 2.0f - i * (PI / stacks);
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectors; ++j) {
            float sectorAngle = j * 2.0f * PI / sectors;
            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);

            positions.push_back(glm::vec3(x, y, z));
            // Simple color based on position
            colors.push_back(glm::vec3(0.5f + 0.5f * x/radius,
                                       0.5f + 0.5f * y/radius,
                                       0.5f + 0.5f * z/radius));
        }
    }

    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < sectors; ++j) {
            int k1 = i * (sectors + 1) + j;
            int k2 = k1 + sectors + 1;

            addVertex(vertices, positions[k1], colors[k1]);
            addVertex(vertices, positions[k2], colors[k2]);
            addVertex(vertices, positions[k1 + 1], colors[k1 + 1]);

            addVertex(vertices, positions[k1 + 1], colors[k1 + 1]);
            addVertex(vertices, positions[k2], colors[k2]);
            addVertex(vertices, positions[k2 + 1], colors[k2 + 1]);
        }
    }

    return vertices;
}

Camera camera;
bool firstMouse = true;
float lastX = 500.0f, lastY = 500.0f;

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.look(xoffset, yoffset);
}

int main() {
    std::cout << "Controls: WASD + Mouse to move, ESC to exit" << std::endl;

    std::vector<float> sphereVertices = generateSphereVertices();

    std::vector<Sphere> spheres;

    // Sphere 1 - Big red (center)
    spheres.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f),  // position
                         1.0f,                         // radius
                         glm::vec3(1.0f, 0.2f, 0.2f)); // color

    std::vector<glm::vec3> colors = {
        glm::vec3(0.2f, 1.0f, 0.2f), // Green
        glm::vec3(0.2f, 0.2f, 1.0f), // Blue
        glm::vec3(1.0f, 1.0f, 0.2f), // Yellow
        glm::vec3(1.0f, 0.2f, 1.0f), // Magenta
        glm::vec3(0.2f, 1.0f, 1.0f), // Cyan
        glm::vec3(1.0f, 0.5f, 0.2f)  // Orange
    };

    for (int i = 0; i < 6; i++) {
        float angle = (i * 60.0f) * 3.14159f / 180.0f;
        float radius = 4.0f + i * 0.5f;
        float height = 0.5f + i * 0.3f;

        spheres.emplace_back(
            glm::vec3(sinf(angle) * radius, height, cosf(angle) * radius),
            0.3f + static_cast<float>(i) * 0.05f,
            colors[i]
        );
    }

    std::cout << "Created " << spheres.size() << " render spheres" << std::endl;

    std::vector<glm::vec3> placeholder;
    Renderer renderer;
    if (!renderer.initialize(SCRN_WIDTH, SCRN_HEIGHT, "Untitled", sphereVertices, placeholder)) {
        return -1;
    }

    GLFWwindow* window = renderer.getWindow();
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSwapInterval(1);

    camera = Camera(glm::vec3(0.0f, 5.0f, 15.0f));

    float aspect = static_cast<float>(SCRN_WIDTH) / SCRN_HEIGHT;
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        aspect,
        0.1f, 100.0f
    );
    renderer.setProjectionMatrix(projection);

    float lastTime = glfwGetTime();
    float animationTime = 0.0f;

    int frameCount = 0;
    float fpsTimer = 0.0f;

    while (!renderer.shouldClose()) {
        float currentTime = glfwGetTime();
        float frameTime = currentTime - lastTime;
        lastTime = currentTime;

        animationTime += frameTime;

        frameCount++;
        fpsTimer += frameTime;
        if (fpsTimer >= 1.0f) {
            std::cout << "FPS: " << frameCount << std::endl;
            frameCount = 0;
            fpsTimer = 0.0f;
        }

        float cameraSpeed = camera.movementSpeed * frameTime;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.moveForward(cameraSpeed);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.moveBackward(cameraSpeed);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.moveLeft(cameraSpeed);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.moveRight(cameraSpeed);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera.moveUp(cameraSpeed);
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            camera.moveDown(cameraSpeed);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        renderer.setViewMatrix(camera.getViewMatrix());

        renderer.beginFrame();
        for (const auto& sphere : spheres) {
            renderer.drawObject(sphere.getModelMatrix());
        }
        renderer.endFrame();

        renderer.pollEvents();
    }

    std::cout << "Untitled demo ended!" << std::endl;
    return 0;
}
