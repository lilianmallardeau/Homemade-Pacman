//
// Created by lilian on 03/10/2021.
//

#include "Renderer.h"

#include <iostream>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Program.h"


bool Renderer::init(unsigned int width, unsigned int height, bool fullscreen) {
    // GLFW Initialization
    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed." << std::endl;
        return false;
    }

    // GLFW window hints
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Creating the GLFW window
    GLFWwindow* window;
    if (fullscreen) {
        // Get primary monitor
        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
        window = glfwCreateWindow(mode->width, mode->height, "Homemade Pacman", primaryMonitor, nullptr);
    }
    else
        window = glfwCreateWindow(width, height, "Homemade Pacman", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "GLFW failed on window creation." << std::endl;
        glfwTerminate();
        return false;
    }

    // Setting the current OpenGL context
    glfwMakeContextCurrent(window);

    // Loading GLAD (modern OpenGL)
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        glfwTerminate();
        return false;
    }

    // Enabling debug output
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(MessageCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    // Enabling blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    context = window;
    return true;
}

void Renderer::Clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    Clear();
}

void Renderer::Draw(const VertexArray &vao, const IndexBuffer &ibo, const Program &shader) {
    vao.Bind();
    ibo.Bind();
    shader.Bind();
    glDrawElements(GL_TRIANGLES, ibo.getLength(), GL_UNSIGNED_INT, nullptr);
}

// -----------------------------------------------------------------------------
// MessageCallback (for debugging purposes)
// -----------------------------------------------------------------------------
void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
  std::cerr << "GL CALLBACK:" << ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ) <<
    "type = 0x" << type <<
    ", severity = 0x" << severity <<
    ", message =" << message << "\n";
}