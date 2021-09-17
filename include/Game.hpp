#pragma once
#include <Window.hpp>
#include <chess.hpp>

class Game {
private:
    Window window;
    float * const black, * const white, * const yellow, * const orange, * const blue, const * null;
    vector<ChessButton*> selected_pieces;
    vector<int> path;
    Chess* chess;
    Box* board;
    char turn;
private:
    void unselect();
    void reset();
    void hide(ChessButton* & button);
    void show(ChessButton* & button);
    static void normalise(float* const&);
    void set_chess_piece_from(ChessButton* const& from, ChessButton* const& to);
public:
    Game();
    ~Game();
    void launch();
};
