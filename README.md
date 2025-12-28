# OpenGL Physics Engine Demo

## 🎯 Project Overview
A real-time 3D physics engine demonstration with OpenGL rendering, featuring sphere collision visualization and first-person camera controls.

## 🚀 Features
- **Real-time 3D Rendering**: 7 colored spheres with smooth 75 FPS performance
- **First-person Camera**: WASD movement + mouse look controls
- **Sphere Physics Visualization**: Arranged in orbital patterns
- **Modern OpenGL**: Shader-based rendering (GLSL 3.3)
- **Performance Optimized**: Vertex sharing, efficient draw calls

## 📊 Performance
- **Target Hardware**: NVIDIA GeForce MX330 (Entry-level mobile GPU)
- **Frame Rate**: 75 FPS (VSync) / 370 FPS (No sync)
- **Vertex Count**: 3,072 vertices per sphere (shared)
- **Draw Calls**: 7 per frame
- **Throughput**: ~8 million vertices/second

## 🛠️ Technical Stack
- **Language**: C++17
- **Graphics**: OpenGL 3.3 Core Profile
- **Math Library**: GLM (OpenGL Mathematics)
- **Windowing**: GLFW 3.3
- **Build System**: CMake 3.10+
- **Shader Language**: GLSL 330 core

## 📁 Project Structure
openGL/
├── CMakeLists.txt # Build configuration
├── renderer/ # OpenGL renderer abstraction
│ ├── renderer.h # Renderer interface
│ └── renderer.cpp # OpenGL implementation
├── src/
│ ├── core/ # Core systems
│ │ ├── camera.h/cpp # First-person camera
│ │ └── shader.h/cpp # Shader management
│ └── glad.c # OpenGL loader
├── include/ # Third-party headers
│ ├── glad/ # OpenGL function pointers
│ └── KHR/ # Khronos platform definitions
├── shaders/ # GLSL shaders
│ ├── 6.3.coordinate_systems.vs # Vertex shader
│ └── 6.3.coordinate_systems.fs # Fragment shader
└── assets/ # Models, textures (future)

text

## 🔧 Build Instructions

### **Prerequisites**
```bash
# Ubuntu/Debian
sudo apt install build-essential cmake libglfw3-dev libglm-dev

# Arch Linux
sudo pacman -S base-devel cmake glfw-x11 glm

# macOS
brew install glfw glm
