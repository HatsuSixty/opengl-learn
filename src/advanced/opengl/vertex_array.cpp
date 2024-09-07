#include "vertex_array.hpp"

#include "errors.hpp"

VertexArray::VertexArray()
{
    gl(GenVertexArrays, 1, &m_vao);
}

VertexArray::~VertexArray()
{
    for (auto* ib : m_index_buffers) {
        delete ib;
    }

    for (auto* vb : m_vertex_buffers) {
        delete vb;
    }
}

void VertexArray::bind() const
{
    gl(BindVertexArray, m_vao);
}

void VertexArray::unbind() const
{
    gl(BindVertexArray, 0);
}

void VertexArray::unbind_all() const
{
    gl(BindVertexArray, 0);
    gl(BindBuffer, GL_ELEMENT_ARRAY_BUFFER, 0);
    gl(BindBuffer, GL_ARRAY_BUFFER, 0);
}

VertexBuffer* VertexArray::bind_vertex_buffer(const void* data, std::size_t size)
{
    VertexBuffer* vb = new VertexBuffer(data, size);
    vb->bind();

    m_vertex_buffers.push_back(vb);

    return vb;
}

IndexBuffer* VertexArray::bind_index_buffer(const unsigned int* indices, std::size_t count)
{
    IndexBuffer* ib = new IndexBuffer(indices, count);
    ib->bind();

    m_index_buffers.push_back(ib);

    return ib;
}