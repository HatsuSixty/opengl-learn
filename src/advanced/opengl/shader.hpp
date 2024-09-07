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

    Shader(const std::string& source_path);
    ~Shader();
};