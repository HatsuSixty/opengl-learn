#pragma once

#include <string>
#include <unordered_map>

#include <GL/glew.h>

class Shader {
private:
    GLuint m_program;

    std::unordered_map<std::string, int> m_locations;

    int get_uniform_location(const std::string& name);

public:
    bool valid;

    void bind() const;
    void unbind() const;

    void set_uniform_4f(const std::string& name, 
                        float x, float y, float z, float w);

    Shader(const std::string& source_path);
    ~Shader();
};