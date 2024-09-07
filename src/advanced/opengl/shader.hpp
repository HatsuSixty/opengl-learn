#pragma once

#include <string>

#include <GL/glew.h>

class Shader {
private:
    GLuint m_program;

public:
    bool valid;

    void bind() const;
    void unbind() const;

    int get_uniform_location(const char* name);
    void set_uniform_4f(int loc, float x, float y, float z, float w);

    Shader(const std::string& source_path);
    ~Shader();
};