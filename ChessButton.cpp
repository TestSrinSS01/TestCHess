#include <ChessButton.hpp>
#include <Window.hpp>

ChessButton::ChessButton(
     int const& x,
     int const& y,
     int const& width,
     int const& height,
     float const* const& colour,
     std::function<void(Button* const&)>const& callback, char piece
):
Button(x, y, width, height, colour, callback, "", 3),
colour_cpy(colour),
piece(piece),
is_target(false) {}

void ChessButton::set_chess_piece(char const& value) {
    piece = value;
}

char ChessButton::get_chess_piece() const {
    return piece;
}

void ChessButton::set_colour(float const* const& col) {
    colour = col;
}

void ChessButton::add_target(ChessButton* const& target) {
    target->is_target = true;
    targets.push_back(target);
}

void ChessButton::clear_target() {
    for (const auto &it : targets)
        it->is_target = false;
    targets.clear();
}

int ChessButton::get_index() const {
    return (y / 50) * 8 + (x / 50);
}

void ChessButton::bind() {
    if (piece) Window::textures.at(Window::piece_map.at(piece))->bind();
    if (is_target) Window::textures.at("target")->bind();
}

std::vector<float> ChessButton::get_vertices() {
    float slot = piece ? Window::textures.at(Window::piece_map.at(piece))->get_slot() : 0;
    float target_slot = is_target ? Window::textures.at("target")->get_slot() : 0;
    return std::vector<float> {
        (float)x, (float)y,                         colour[0], colour[1], colour[2], 1.0f,       0.0f, 0.0f,   0,
        (float)(x + width), (float)y,               colour[0], colour[1], colour[2], 1.0f,       1.0f, 0.0f,   0,
        (float)(x + width), (float)(y + height),    colour[0], colour[1], colour[2], 1.0f,       1.0f, 1.0f,   0,
        (float)(x + width), (float)(y + height),    colour[0], colour[1], colour[2], 1.0f,       1.0f, 1.0f,   0,
        (float)x, (float)(y + height),              colour[0], colour[1], colour[2], 1.0f,       0.0f, 1.0f,   0,
        (float)x, (float)y,                         colour[0], colour[1], colour[2], 1.0f,       0.0f, 0.0f,   0,
                                                                                     
        (float)x, (float)y,                         colour[0], colour[1], colour[2], 1.0f,       0.0f, 0.0f,   target_slot,
        (float)(x + width), (float)y,               colour[0], colour[1], colour[2], 1.0f,       1.0f, 0.0f,   target_slot,
        (float)(x + width), (float)(y + height),    colour[0], colour[1], colour[2], 1.0f,       1.0f, 1.0f,   target_slot,
        (float)(x + width), (float)(y + height),    colour[0], colour[1], colour[2], 1.0f,       1.0f, 1.0f,   target_slot,
        (float)x, (float)(y + height),              colour[0], colour[1], colour[2], 1.0f,       0.0f, 1.0f,   target_slot,
        (float)x, (float)y,                         colour[0], colour[1], colour[2], 1.0f,       0.0f, 0.0f,   target_slot,
                                                                                     
        (float)x, (float)y,                         colour[0], colour[1], colour[2], 1.0f,       0.0f, 0.0f,   slot,
        (float)(x + width), (float)y,               colour[0], colour[1], colour[2], 1.0f,       1.0f, 0.0f,   slot,
        (float)(x + width), (float)(y + height),    colour[0], colour[1], colour[2], 1.0f,       1.0f, 1.0f,   slot,
        (float)(x + width), (float)(y + height),    colour[0], colour[1], colour[2], 1.0f,       1.0f, 1.0f,   slot,
        (float)x, (float)(y + height),              colour[0], colour[1], colour[2], 1.0f,       0.0f, 1.0f,   slot,
        (float)x, (float)y,                         colour[0], colour[1], colour[2], 1.0f,       0.0f, 0.0f,   slot,
    };
}
