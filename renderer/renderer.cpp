#include "renderer.h"
#include "shader.h"
#include <iostream>
#include <fstream>
#include <filesystem>

// Static member initialization
void Renderer::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Constructor
Renderer::Renderer()
    : window(nullptr), screenWidth(0), screenHeight(0),
      VAO(0), VBO(0), shader(nullptr) {
}

// Destructor
Renderer::~Renderer() {
    cleanup();
}

bool Renderer::initialize(int width, int height, const std::string& title,
                         const std::vector<float>& vertices,
                         const std::vector<glm::vec3>& cubePositions) {

    screenWidth = width;
    screenHeight = height;
    this->vertices = vertices;

    // GLFW initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    // Configure global OpenGL state
    glEnable(GL_DEPTH_TEST);

    // Debug: Check OpenGL version
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    // Try multiple paths for shaders
    std::vector<std::string> vertexPaths = {
        "shaders/6.3.coordinate_systems.vs",
        "../shaders/6.3.coordinate_systems.vs",
        "../../shaders/6.3.coordinate_systems.vs"
    };

    std::vector<std::string> fragmentPaths = {
        "shaders/6.3.coordinate_systems.fs",
        "../shaders/6.3.coordinate_systems.fs",
        "../../shaders/6.3.coordinate_systems.fs"
    };

    // Load shader
    shader = nullptr;
    bool shaderLoaded = false;

    for (size_t i = 0; i < vertexPaths.size() && i < fragmentPaths.size(); i++) {
        std::filesystem::path vPath(vertexPaths[i]);
        std::filesystem::path fPath(fragmentPaths[i]);

        if (std::filesystem::exists(vPath) && std::filesystem::exists(fPath)) {
            std::cout << "Loading shader from: " << vertexPaths[i] << " and " << fragmentPaths[i] << std::endl;
            shader = new Shader(vertexPaths[i].c_str(), fragmentPaths[i].c_str());

            if (shader->isLoaded()) {
                shaderLoaded = true;
                break;
            } else {
                delete shader;
                shader = nullptr;
            }
        }
    }

    // If shader still not loaded, try constructor anyway (it will show error)
    if (!shaderLoaded) {
        std::cout << "Trying default shader path..." << std::endl;
        shader = new Shader("shaders/6.3.coordinate_systems.vs",
                           "shaders/6.3.coordinate_systems.fs");

        if (!shader->isLoaded()) {
            std::cout << "ERROR::SHADER::Failed to load shader from any path!" << std::endl;
            delete shader;
            shader = nullptr;
            return false;
        }
    }

    std::cout << "Shader loaded successfully! ID: " << shader->ID << std::endl;

    // Setup buffers
    setupBuffers();

    // Initialize default matrices (can be overridden by main)
    viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    projectionMatrix = glm::perspective(glm::radians(45.0f),
                                       (float)width/(float)height,
                                       0.1f, 100.0f);

    std::cout << "Renderer initialized successfully!" << std::endl;
    std::cout << "Window: " << width << "x" << height << std::endl;
    std::cout << "Vertex count: " << vertices.size() / 6 << std::endl;

    return true;
}

void Renderer::setupBuffers() {
    // Generate and bind VAO/VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Debug: Check vertex data
    std::cout << "Setting up buffers..." << std::endl;
    std::cout << "Vertex count: " << vertices.size() / 6 << std::endl;
    std::cout << "Total floats: " << vertices.size() << std::endl;

    if (vertices.empty()) {
        std::cout << "ERROR: Vertex data is empty!" << std::endl;
        return;
    }

    // Upload vertex data
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 vertices.data(), GL_STATIC_DRAW);

    // Position attribute (6 floats per vertex: 3 position, 3 color)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "OpenGL Error in setupBuffers: " << err << std::endl;
    } else {
        std::cout << "Buffers set up successfully!" << std::endl;
    }
}

void Renderer::beginFrame() {
    // Clear screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use shader and set view/projection matrices
    if (shader && shader->isLoaded()) {
        shader->use();
        shader->setMat4("view", viewMatrix);
        shader->setMat4("projection", projectionMatrix);
    }
}

void Renderer::drawObject(const glm::mat4& modelMatrix) {
    // Check if shader is valid
    if (!shader || !shader->isLoaded()) {
        std::cerr << "ERROR: Cannot draw object - shader not loaded!" << std::endl;
        return;
    }

    // Check if VAO is valid
    if (VAO == 0) {
        std::cerr << "ERROR: Cannot draw object - VAO not initialized!" << std::endl;
        return;
    }

    // Set model matrix and draw
    shader->setMat4("model", modelMatrix);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 6);
    glBindVertexArray(0);  // Optional: unbind VAO

    // Check for OpenGL errors during draw
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "OpenGL Error during draw: " << err << std::endl;
    }
}

void Renderer::endFrame() {
    // Swap buffers
    glfwSwapBuffers(window);
}

void Renderer::setViewMatrix(const glm::mat4& view) {
    viewMatrix = view;
}

void Renderer::setProjectionMatrix(const glm::mat4& projection) {
    projectionMatrix = projection;
}

void Renderer::processInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

bool Renderer::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void Renderer::pollEvents() {
    glfwPollEvents();
}

void Renderer::cleanup() {
    // Cleanup OpenGL objects
    if (VAO) {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
    if (VBO) {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }

    // Cleanup shader
    if (shader) {
        delete shader;
        shader = nullptr;
    }

    // Cleanup GLFW
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    glfwTerminate();
    std::cout << "Renderer cleaned up!" << std::endl;
}
