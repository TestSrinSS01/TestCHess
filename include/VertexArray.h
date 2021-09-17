#pragma once
#include <vector>
#include <VertexBuffer.h>

struct Element {
    unsigned int count;
    const void* offset;
};

class Layout {
private:
    std::vector<Element> m_elements;
    unsigned int m_stride = 0;
public:
    void add(unsigned int const& count, void const* const& offset);
    [[nodiscard]] unsigned int get_stride() const;
    [[nodiscard]] std::vector<Element> get_elements() const;
};

class VertexArray {
private:
    unsigned int m_vao;
    
public:
    VertexArray();
    ~VertexArray();
    void add_layout(VertexBuffer const& vbo, Layout const& layout) const;
    void bind() const;
};
