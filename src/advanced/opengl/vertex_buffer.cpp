#include "vertex_buffer.hpp"

#include "errors.hpp"

VertexBuffer::VertexBuffer(const void* data, std::size_t size)
{
    gl(GenBuffers, 1, &m_vbo);
    gl(BindBuffer, GL_ARRAY_BUFFER, m_vbo);
    gl(BufferData, GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    gl(BindBuffer, GL_ARRAY_BUFFER, 0);
}

VertexBuffer::~VertexBuffer()
{
    gl(DeleteBuffers, 1, &m_vbo);
}

void VertexBuffer::set_attribute_layout(int index, int component_count, GLenum component_type,
                                        bool normalized, std::size_t stride, 
                                        std::size_t offset)
{
    gl(VertexAttribPointer, index, component_count, component_type, 
                            normalized, stride, (void*)offset);
    gl(EnableVertexAttribArray, index);
}

void VertexBuffer::bind() const
{
    gl(BindBuffer, GL_ARRAY_BUFFER, m_vbo);
}

void VertexBuffer::unbind() const
{
    gl(BindBuffer, GL_ARRAY_BUFFER, 0);
}