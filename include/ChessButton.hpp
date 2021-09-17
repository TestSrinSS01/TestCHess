#pragma once
#include <Button.hpp>

class ChessButton: public Button {
private:
    std::vector<ChessButton*> targets;
    bool is_target;
    char piece;
public:
    float const* colour_cpy;
public:
    ChessButton(
        int const& x,
        int const& y,
        int const& width,
        int const& height,
        float const* const& colour,
        std::function<void(Button* const&)> const& callback,
        char piece
    );
    void add_target(ChessButton* const&);
    void clear_target();
    void set_colour(float const* const&);
    void set_chess_piece(char const& value);
    [[nodiscard]] char get_chess_piece() const;
    [[nodiscard]] int get_index() const;
    void bind() override;
    std::vector<float> get_vertices() override;
};
