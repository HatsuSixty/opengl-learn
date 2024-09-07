#include "index_buffer.hpp"

#include "errors.hpp"

IndexBuffer::IndexBuffer(const unsigned int* indices, std::size_t count)
    : m_count(count)
{
    gl(GenBuffers, 1, &m_ibo);
    gl(BindBuffer, GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    gl(BufferData, GL_ELEMENT_ARRAY_BUFFER, count * sizeof(*indices), 
                   indices, GL_STATIC_DRAW);
    gl(BindBuffer, GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBuffer::~IndexBuffer()
{
    gl(BindBuffer, GL_ELEMENT_ARRAY_BUFFER, 0);
    gl(DeleteBuffers, 1, &m_ibo);
}

void IndexBuffer::bind() const
{
    gl(BindBuffer, GL_ELEMENT_ARRAY_BUFFER, m_ibo);
}

void IndexBuffer::unbind() const
{
    gl(BindBuffer, GL_ELEMENT_ARRAY_BUFFER, 0);
}