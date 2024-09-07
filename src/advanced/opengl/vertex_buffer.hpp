#pragma once

#include <GL/glew.h>

#include <cstddef>

class VertexBuffer {
private:
    GLuint m_vbo;

public:
    VertexBuffer(const void* data, std::size_t size);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

    void set_attribute_layout(int index, int component_count, GLenum component_type,
                              bool normalized, std::size_t stride, 
                              std::size_t offset);
};