#pragma once

#include <GL/glew.h>

#include <cstddef>

class IndexBuffer {
private:
    GLuint m_ibo;
    std::size_t m_count;

public:
    IndexBuffer(const unsigned int* indices, std::size_t count);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

    inline std::size_t get_count() const { return m_count; }
};