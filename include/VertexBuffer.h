#pragma once
class VertexBuffer {
private:
    unsigned int m_vbo;
public:
    VertexBuffer(void const* const& buffer, unsigned int const& size, unsigned int const& usage);
    ~VertexBuffer();
    void bind() const;
};
