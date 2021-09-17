#include <Box.hpp>
#include <GL/glew.h>

void Box::render() {
    if (!buttons.empty()) {
        shader.active();
        auto proj = glm::ortho(0.0f, 600.0f, 0.0f, 400.0f);
        shader.setUniformMat4f("mvp", glm::value_ptr(proj));
        shader.setUniform1iv("tex", tex);
        int len = buttons.size() * buttons[0]->get_vertices().size();
        float* buffer = new float[len];
        int i = 0;
        for (auto const& button : buttons) {
            button->bind();
            for (auto const& f : button->get_vertices())
                if (i != len) buffer[i++] = f;
        }
        glBufferSubData(GL_ARRAY_BUFFER, 0, len * sizeof(float), buffer);
        glDrawArrays(GL_TRIANGLES, 0, len);
        delete[] buffer;
    }
}

Box::Box(const int &x, const int &y, bool const& is_active):
tex { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 },
x(x),
y(y),
is_active(is_active),
shader {
    R"(
        #version 460 core
        layout(location = 0) in vec4 a_pos;
        layout(location = 1) in vec4 a_colour;
        layout(location = 2) in vec2 a_texCords;
        layout(location = 3) in float a_tex_index;
        uniform mat4 mvp;
        out vec2 v_texCords;
        out vec4 v_colour;
        out float v_tex_index;
        void main() {
            gl_Position = mvp * a_pos;
            v_texCords = a_texCords;
            v_colour = a_colour;
            v_tex_index = a_tex_index;
        }
    )",
    R"(
        #version 460 core
        in vec2 v_texCords;
        in vec4 v_colour;
        in float v_tex_index;
        uniform sampler2D tex[32];
        out vec4 colour;
        void main() {
            int index = int(v_tex_index);
            if (index == 0) colour = v_colour;
            else colour = texture(tex[index], v_texCords);
        }
    )"
} {}

void Box::add_button(Button* const& button) {
    button->add_offsets(x, y);
    buttons.push_back(button);
}

void Box::click(double const& _x, double const& _y) {
    if (!buttons.empty())
        for (const auto &button : buttons) button->click(_x, _y);
}

Box::~Box() {
    for (const auto &button : buttons) delete button;
}

Button *Box::operator[](int index) {
    return buttons[index];
}
