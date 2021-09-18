#include <Box.hpp>
#include <GL/glew.h>

void Box::render() {
    if (!buttons.empty()) {
        std::vector<float> buffer;
        for (auto const& button : buttons) {
            //button->bind();
            for (auto const& f : button->get_vertices())
                buffer.push_back(f);
        }
        glBufferSubData(GL_ARRAY_BUFFER, 0, buffer.size() * sizeof buffer[0], &buffer[0]);
        glDrawArrays(GL_TRIANGLES, 0, buffer.size());
    }
}

Box::Box(const int &x, const int &y, bool const& is_active):
x(x),
y(y),
is_active(is_active) {}

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
