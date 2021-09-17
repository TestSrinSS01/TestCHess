#include <VertexBuffer.h>
#include <GL/glew.h>

VertexBuffer::VertexBuffer(void const* const& buffer, unsigned int const& size, unsigned int const& usage): m_vbo(0) {
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, size, buffer, usage);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &m_vbo);
}

void VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
}
