#include <iostream>
#include <fstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "opengl/errors.hpp"
#include "opengl/vertex_array.hpp"

int main()
{
    if (!glfwInit())
        return -1;

    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", 
                                          nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    
    if (glewInit() != GLEW_OK) {
        std::cerr << "ERROR: could not initialize GLEW" << std::endl;
        return 1;
    }

    float vertexes[] = {
        // x    y
        -0.5f, -0.5f,
        +0.0f, +0.5f,
        +0.5f, -0.5f,
    };

    unsigned int indices[] = {
        0, 1, 2
    };

    VertexArray* va = new VertexArray();

    va->bind();

    VertexBuffer* vb = va->bind_vertex_buffer(vertexes, sizeof(vertexes));
    vb->set_attribute_layout(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertexes[0]) * 2, 0);

    IndexBuffer* ib = va->bind_index_buffer(indices, sizeof(indices) / sizeof(indices[0]));
    (void)ib;

    va->unbind_all();

    while (!glfwWindowShouldClose(window)) {
        gl(Clear, GL_COLOR_BUFFER_BIT);

        va->bind();
        gl(DrawElements, GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
        va->unbind();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete va;
    glfwTerminate();
    
    return 0;
}