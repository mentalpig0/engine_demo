#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>

class Shader;

class Renderer {
public:
    Renderer();
    ~Renderer();

    // Initialization
    bool initialize(int width, int height, const std::string& title,
                    const std::vector<float>& vertices,
                    const std::vector<glm::vec3>& cubePositions);

    // Frame control
    void beginFrame();
    void endFrame();

    // Drawing
    void drawObject(const glm::mat4& modelMatrix);

    // Matrix setters
    void setViewMatrix(const glm::mat4& view);
    void setProjectionMatrix(const glm::mat4& projection);

    // Input/Window
    void processInput();
    bool shouldClose() const;
    void pollEvents();

    GLFWwindow* getWindow() const { return window; }

    // Cleanup
    void cleanup();

private:
    GLFWwindow* window;
    int screenWidth, screenHeight;

    // OpenGL objects
    unsigned int VAO, VBO;
    Shader* shader;

    // Matrices
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    // Vertex data
    std::vector<float> vertices;

    // Internal methods
    void setupBuffers();
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    // Non-copyable
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
};

#endif // RENDERER_H
