#include <VertexArray.h>
#include <GL/glew.h>

void Layout::add(unsigned int const& count, void const* const& offset) {
    m_elements.push_back({count, offset});
    m_stride += count * sizeof(float);
}

unsigned int Layout::get_stride() const {
    return m_stride;
}

std::vector<Element> Layout::get_elements() const {
    return m_elements;
}

VertexArray::VertexArray(): m_vao(0) {
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &m_vao);
}

void VertexArray::bind() const {
    glBindVertexArray(m_vao);
}

void VertexArray::add_layout(VertexBuffer const& vbo, Layout const& layout) const {
    bind();
    vbo.bind();
    auto elements = layout.get_elements();
    for (int i = 0; i < elements.size(); i++) {
        auto element = elements[i];
        glVertexAttribPointer(i, (int) element.count, GL_FLOAT, GL_FALSE, (int) layout.get_stride(), element.offset);
        glEnableVertexAttribArray(i);
    }
}
