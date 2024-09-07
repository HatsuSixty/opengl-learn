#include <iostream>
#include <fstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct ShaderSources {
    std::string vertex;
    std::string fragment;
};

static ShaderSources parse_shader(const std::string& file_path)
{
    std::ifstream stream(file_path);

    std::string vertex_source;
    std::string fragment_source;
    GLenum shader_mode = GL_VERTEX_SHADER;

    std::string line;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                shader_mode = GL_VERTEX_SHADER;
            else if (line.find("fragment") != std::string::npos)
                shader_mode = GL_FRAGMENT_SHADER;

            continue;
        }

        if (shader_mode == GL_VERTEX_SHADER) {
            vertex_source.append(line);
            vertex_source.append("\n");
        } else if (shader_mode == GL_FRAGMENT_SHADER) {
            fragment_source.append(line);
            fragment_source.append("\n");
        }
    }

    return {
        .vertex = vertex_source,
        .fragment = fragment_source,
    };
}

static GLuint compile_shader(GLenum type, const std::string& source)
{
    GLuint id = glCreateShader(type);
    
    const char* c_source = source.c_str();
    glShaderSource(id, 1, &c_source, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int error_length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &error_length);

        char* error = (char*)alloca(error_length);
        glGetShaderInfoLog(id, error_length, &error_length, error);

        const char* shader_type = 
            type == GL_VERTEX_SHADER ? "vertex" : "fragment";

        std::cout << "ERROR: " << shader_type << " shader compilation: " 
                  << error;

        glDeleteShader(id);

        return 0;
    }

    return id;
}

static GLuint create_shader(const std::string& shader_source)
{
    GLuint program = glCreateProgram();

    ShaderSources sources = parse_shader(shader_source);

    GLuint vert_shader = compile_shader(GL_VERTEX_SHADER, sources.vertex);
    if (vert_shader == 0) {
        glDeleteProgram(program);
        return 0;
    }

    GLuint frag_shader = compile_shader(GL_FRAGMENT_SHADER, sources.fragment);
    if (frag_shader == 0) {
        glDeleteProgram(program);
        return 0;
    }

    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);

    glLinkProgram(program);
    glValidateProgram(program);

    // Because the program was already linked and
    // validated, we don't need the vertex and fragment
    // shader anymore.
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    return program;
}

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
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

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

    // Declare the vertex buffer object.
    // This is just a chunk of data in VRAM.
    GLuint vbo;

    // Generate buffer (actually allocate the data in VRAM).
    glGenBuffers(1, &vbo);

    // Bind the VBO to the target GL_ARRAY_BUFFER.
    // This is basically setting up the VBO so we
    // can do things with it. 
    // When it's bound to GL_ARRAY_BUFFER, any operation 
    // we do with the array buffer gets applied to
    // the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Put data in the VBO
    // (the vertexes with the triangle positions).
    glBufferData(GL_ARRAY_BUFFER, 
                 sizeof(vertexes_with_positions), vertexes_with_positions, 
                 GL_STATIC_DRAW);

    // Declare the triangle index buffer (IBO, Index Buffer Object)
    // and put the indices in it.
    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), 
                 vertex_indices, GL_STATIC_DRAW);

    /*                                       *
     *   -=-= Layout the data in VRAM =-=-   *
     *                                       */

    // Define the layout of the attributes in the vertex buffer.
    // Because a vertex is like a struct, we should define the layout
    // of that struct.
    glVertexAttribPointer(// Attribute index. The vertex shader is gonna
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
                          (const void*)0);
    
    // Enable the array of vertex attribute data.
    glEnableVertexAttribArray(0);

    // Layout for color
    glVertexAttribPointer(// Attribute index. The vertex shader is gonna
                          // use this index to refer to this attribute.
                          // This is also the index of this attribute array
                          // in the vertex array object.
                          1,

                          // Amount of components in a single attribute.
                          4,

                          // Type of each component in the vertex.
                          GL_FLOAT,

                          // Specify whether OpenGL should normalize the
                          // components.
                          GL_FALSE, 

                          // Offset between each attribute.
                          sizeof(vertexes_with_positions[0]) * 6,

                          // Offset of the first component of the
                          // first vertex in the buffer.
                          (const void*)(sizeof(vertexes_with_positions[0]) * 2));

    // Enable the array of vertex attribute data.
    glEnableVertexAttribArray(1);

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

    GLuint shader = create_shader("resources/default_vertex_color.glsl");
    if (shader == 0) {
        return 1;
    }

    // Setup uniforms.
    // Uniforms are kinda like attributes, but they are not
    // tied to the vertexes and can be updated at any time.
    // It's kinda like a variable.

    // Get the location of the `u_Color` uniform.
    int color_location = glGetUniformLocation(shader, "u_Color");

    // Set the value of the uniform.
    glUniform4f(color_location, 1.0f, 0.0f, 0.0f, 1.0f);

    /*                         *
     *   -=-= Main loop =-=-   *
     *                         */
    
    // Unbind everything to avoid accidental
    // modification.
    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    float r = 0.0f; // Amount of red.
    float r_increment = 0.01f; // How much `r` will be incremented.
    
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the shader we created previously.
        glUseProgram(shader);

        // Update the color uniform.
        glUniform4f(color_location, r, 0.3f, 0.8f, 1.0f);

        // Bind the VAO before drawing.
        glBindVertexArray(vao);

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

        // Update `r`.

        if (r > 1.0f || r < 0.0f)
            r_increment *= -1;

        r += r_increment;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup: Delete VAO, VBO, IBO, shader program, and GLFW resources.
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteProgram(shader);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}