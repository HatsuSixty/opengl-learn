#include <iostream>
#include <fstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "opengl/vertex_array.hpp"
#include "opengl/shader.hpp"

int main()
{
    if (!glfwInit()) {
        std::cerr << "ERROR: could not initialize GLFW" << std::endl;
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Application",
                                          nullptr, nullptr);
    if (!window) {
        std::cerr << "ERROR: could not create GLFW window" << std::endl;
        return 1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "ERROR: could not initialize GLEW" << std::endl;
        return 1;
    }

    std::cout << "INFO: OpenGL initialized successfully" << std::endl;
    std::cout << " > OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    /*                                      *
     *   -=-= Setup Data in the VRAM =-=-   *
     *                                      */

    // Generate and bind a Vertex Array Object (VAO).
    // VAOs store the configuration and layouts of vertex buffers.
    // This is useful because, if we bind the VAO, we don't need to
    // specify the layout of the vertex buffers again.
    VertexArray* va = new VertexArray();
    va->bind();

    // Define the positions of the points in the triangle.
    // This is an array of vertexes (or simply, the vertex buffer).
    // Vertexes are a chunk of data (kinda like a struct)
    // with the attributes (parameters) that are gonna get sent 
    // to the vertex shader.
    float vertexes_with_positions[] = {
        // x    y      // color
        -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, // Vertex 1
        +0.0f, +0.5f,  0.0f, 0.0f, 1.0f, 1.0f, // Vertex 2
        +0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, // Vertex 3
    };

    // Define what indexes of the vertex array
    // we're actually gonna use.
    unsigned int vertex_indices[] = {
        0, 1, 2,
    };

    VertexBuffer* vb = va->bind_vertex_buffer(vertexes_with_positions, 
                                              sizeof(vertexes_with_positions));
    IndexBuffer* ib = va->bind_index_buffer(vertex_indices, 
                                            sizeof(vertex_indices) / sizeof(vertex_indices[0]));
    (void)ib;

    /*                                       *
     *   -=-= Layout the data in VRAM =-=-   *
     *                                       */

    // Define the layout of the attributes in the vertex buffer.
    // Because a vertex is like a struct, we should define the layout
    // of that struct.

    // Layout for position.
    vb->set_attribute_layout(// Attribute index. The vertex shader is gonna
                             // use this index to refer to this attribute.
                             // This is also the index of this attribute array
                             // in the vertex array object.
                             0,

                             // Amount of components in a single attribute.
                             2,

                             // Type of each component in the vertex.
                             GL_FLOAT,

                             // Specify whether OpenGL should normalize the
                             // components.
                             GL_FALSE,

                             // Offset between each attribute.
                             sizeof(vertexes_with_positions[0]) * 6,

                             // Offset of the first component of the
                             // first vertex in the buffer.
                             0);

    // Layout for color.
    vb->set_attribute_layout(1, 4, GL_FLOAT, GL_FALSE, 
                             sizeof(vertexes_with_positions[0]) * 6,
                             sizeof(vertexes_with_positions[0]) * 2);

    // Unbind vertex array.
    va->unbind_all();

    /*                            *
     *   -=-= Shader setup =-=-   *
     *                            */

    // The vertex shader will compute the positions 
    // based on the positions we provided to the shader
    // via the vertexes.
    // The fragment shader will set the color for each
    // pixel in our triangle.

    // In this case, we don't have any computation to do,
    // so we'll just set `gl_Position` to the provided position.
    // See ./resources/default_vertex_color.glsl

    Shader* shader = new Shader("resources/default_vertex_color.glsl");
    if (!shader->valid) {
        return 1;
    }

    /*                         *
     *   -=-= Main loop =-=-   *
     *                         */
    
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the shader we created previously.
        shader->bind();

        // Bind the VAO before drawing.
        va->bind();

        // Draw the vertexes in the element (index) array buffer
        // (In this case, the triangle index buffer. Because the triangle
        //  index buffer is what is currently bound to the element array buffer).
        glDrawElements(// Drawing mode.
                       GL_TRIANGLES,

                       // Number of elements to be rendered.
                       sizeof(vertex_indices) / sizeof(vertex_indices[0]),
                       
                       // Type of the indices.
                       GL_UNSIGNED_INT,

                       // Offset of the first index in the array.
                       (const void*)0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup: Delete VAO, VBO, IBO, shader program, and GLFW resources.
    delete va;
    delete shader;
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}