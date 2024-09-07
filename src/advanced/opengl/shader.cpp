#include "shader.hpp"

#include <iostream>
#include <fstream>
#include <string>

#include "errors.hpp"

static GLuint compile_shader(GLenum type, const std::string& source)
{
    GLuint id;
    gl_call(id = glCreateShader(type));
    
    const char* c_source = source.c_str();
    gl(ShaderSource, id, 1, &c_source, nullptr);
    gl(CompileShader, id);

    int result;
    gl(GetShaderiv, id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int error_length;
        gl(GetShaderiv, id, GL_INFO_LOG_LENGTH, &error_length);

        char* error = (char*)alloca(error_length);
        gl(GetShaderInfoLog, id, error_length, &error_length, error);

        const char* shader_type = 
            type == GL_VERTEX_SHADER ? "vertex" : "fragment";

        std::cerr << "ERROR: " << shader_type << " shader compilation: " 
                  << error;

        gl(DeleteShader, id);

        return 0;
    }

    return id;
}

Shader::Shader(const std::string& source_path)
{
    valid = true;
    
    /*                  *
     *   Parse shader   *
     *                  */
    
    std::ifstream stream(source_path);

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

    /*                             *
     *   Compile and link shader   *
     *                             */

    gl_call(m_program = glCreateProgram());

    GLuint vert_shader = compile_shader(GL_VERTEX_SHADER, vertex_source);
    if (vert_shader == 0) {
        gl(DeleteProgram, m_program);
        valid = true;
    }

    GLuint frag_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_source);
    if (frag_shader == 0) {
        gl(DeleteProgram, m_program);
        valid = true;
    }    

    gl(AttachShader, m_program, vert_shader);
    gl(AttachShader, m_program, frag_shader);

    gl(LinkProgram, m_program);
    gl(ValidateProgram, m_program);

    gl(DeleteShader, vert_shader);
    gl(DeleteShader, frag_shader);
}

Shader::~Shader()
{
    gl(DeleteProgram, m_program);
}

int Shader::get_uniform_location(const char* name)
{
    GLint loc;
    gl_call(loc = glGetUniformLocation(m_program, name));
    return loc;
}

void Shader::set_uniform_4f(int loc, float x, float y, float z, float w)
{
    glUniform4f(loc, x, y, z, w);
}

void Shader::bind() const
{
    gl(UseProgram, m_program);
}

void Shader::unbind() const
{
    gl(UseProgram, 0);
}