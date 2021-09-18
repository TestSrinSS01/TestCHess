#include <Button.hpp>
#include <Window.hpp>

Button::Button (
    const int &x,
    const int &y,
    const int &width,
    const int &height,
    const float *const &colour,
    std::function<void(Button *const &)> callback,
    const std::string& texture
):
x(x),
y(y),
width(width),
height(height),
colour(colour),
callback(std::move(callback)),
texture(texture) {}

void Button::click(const double &_x, const double &_y) {
    if (hover(_x, _y)) callback(this);
}

bool Button::hover(const double &_x, const double &_y) const {
    return (_x >= this->x) && (_x <= (this->x + width)) && (_y >= this->y) && (_y <= (this->y + height));
}

std::vector<float> Button::get_vertices() {
    float slot = texture.empty()? 0: Window::textures.at(texture)->get_slot();
    auto quad = std::vector<float>{
        (float)x, (float)y,                       colour[0], colour[1], colour[2], 1.0f,    0.0f, 0.0f,    0,
        (float)(x + width), (float)y,             colour[0], colour[1], colour[2], 1.0f,    1.0f, 0.0f,    0,
        (float)(x + width), (float)(y + height),  colour[0], colour[1], colour[2], 1.0f,    1.0f, 1.0f,    0,
        (float)(x + width), (float)(y + height),  colour[0], colour[1], colour[2], 1.0f,    1.0f, 1.0f,    0,
        (float)x, (float)(y + height),            colour[0], colour[1], colour[2], 1.0f,    0.0f, 1.0f,    0,
        (float)x, (float)y,                       colour[0], colour[1], colour[2], 1.0f,    0.0f, 0.0f,    0,
    };
    if (!texture.empty()) {
        float slot = Window::textures.at(texture)->get_slot();
        auto piece = std::vector<float>{
            (float)x, (float)y,                       colour[0], colour[1], colour[2], 1.0f,    0.0f, 0.0f,    slot,
            (float)(x + width), (float)y,             colour[0], colour[1], colour[2], 1.0f,    1.0f, 0.0f,    slot,
            (float)(x + width), (float)(y + height),  colour[0], colour[1], colour[2], 1.0f,    1.0f, 1.0f,    slot,
            (float)(x + width), (float)(y + height),  colour[0], colour[1], colour[2], 1.0f,    1.0f, 1.0f,    slot,
            (float)x, (float)(y + height),            colour[0], colour[1], colour[2], 1.0f,    0.0f, 1.0f,    slot,
            (float)x, (float)y,                       colour[0], colour[1], colour[2], 1.0f,    0.0f, 0.0f,    slot,
        };
        quad.insert(quad.end(), piece.begin(), piece.end());
    }
    return quad;
}

void Button::bind() {
    if (!texture.empty()) Window::textures.at(texture)->bind();
}

void Button::add_offsets(const int &_x, const int &_y) {
    *(int*)&x += _x;
    *(int*)&y += _y;
}