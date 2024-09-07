#pragma once

#include <GL/glew.h>

#include <vector>
#include <cstddef>

#include "index_buffer.hpp"
#include "vertex_buffer.hpp"

class VertexArray {
private:
    GLuint m_vao;

    std::vector<IndexBuffer*> m_index_buffers;
    std::vector<VertexBuffer*> m_vertex_buffers;

public:
    VertexArray();
    ~VertexArray();

    void bind() const;
    void unbind() const;
    void unbind_all() const;

    VertexBuffer* bind_vertex_buffer(const void* data, std::size_t size);
    IndexBuffer* bind_index_buffer(const unsigned int* indices, std::size_t count);
};