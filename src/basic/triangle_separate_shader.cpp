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

    // Define the positions of the points in the triangle.
    // This is an array of vertexes.
    // Vertexes are a chunk of data (kinda like a struct)
    // with the attributes (parameters) that are gonna get sent 
    // to the vertex shader.
    float vertexes_with_positions[] = {
        // x    y      // color
        -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, // Vertex 1
        +0.0f, +0.5f,  0.0f, 0.0f, 1.0f, 1.0f, // Vertex 2
        +0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, // Vertex 3
    };

    // Declare triangle buffer.
    // This is just a chunk of data in VRAM.
    GLuint triangle_buffer;

    // Generate buffer
    // (actually allocate the data in VRAM).
    glGenBuffers(1, &triangle_buffer);

    // Bind the triangle buffer to the target GL_ARRAY_BUFFER.
    // This is basically setting up the triangle buffer so
    // we can do things with it. 
    // When it's bound to GL_ARRAY_BUFFER, any operation 
    // we do with the array buffer gets applied to
    // the triangle buffer.
    glBindBuffer(GL_ARRAY_BUFFER, triangle_buffer);

    // Put data in the triangle buffer
    // (The vertexes with the triangle positions).
    glBufferData(GL_ARRAY_BUFFER, 
                 sizeof(vertexes_with_positions), vertexes_with_positions, 
                 GL_STATIC_DRAW);

    /*                                       *
     *   -=-= Layout the data in VRAM =-=-   *
     *                                       */

    // Define the layout of the attributes in the vertexes array.
    // Because a vertex is like a struct, we should define the layout
    // of that struct.

    // Layout for position
    glVertexAttribPointer(// Attribute index. The vertex shader is gonna
                          // use this index to refer to this attribute.
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

    glUseProgram(shader);

    /*                         *
     *   -=-= Main loop =-=-   *
     *                         */

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the vertexes in the array buffer
        // (In this case, the triangle buffer. Because the triangle 
        //  buffer is what is currently bound to the array buffer).
        glDrawArrays(GL_TRIANGLES, 0, 
                     3 /* 6 floats, but 3 vertexes (1 vertex = 2 floats) */);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();

    return 0;
}