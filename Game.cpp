#include <Game.hpp>
#include <sstream>
#include <array>
#include <ChessButton.hpp>

void Game::normalise(float* const& colour) {
    colour[0] /= 225.0f;
    colour[1] /= 225.0f;
    colour[2] /= 225.0f;
}

Game::Game():
chess (new Chess("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", 'w', "-", "-")),
window (600, 400, "Chess"),
black (new float[] { 119.0f, 149.0f, 86.0f }),
white (new float[] { 238.0f, 238.0f, 210.0f }),
yellow (new float[] { 246, 246, 105 }),
orange (new float[] { 255, 165, 0 }),
blue (new float[] { 88, 101, 242 }),
null (new float[] { 1, 1, 1 }),
board (new Box(0, 0, true)),
turn('w') {
    normalise(black);
    normalise(white);
    normalise(yellow);
    normalise(orange);
    normalise(blue);
    cout << "loading textures..." << endl;
    Window::load_texture(
        0,
        "R_","N_","B_","Q_","K_","P_",
        "p","r","n","b","q","k","dot","target", "download"
    );
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            board->add_button(new ChessButton(file * 50, rank * 50, 50, 50, (rank + file) % 2 == 0? white: black, [&, rank, file](Button* const& it){
                auto current = (ChessButton*)it;
                auto prev = (ChessButton*) board->previous_button;
                auto current_index = rank * 8 + file;
                if (prev && prev->get_chess_piece()) {
                    hide(prev);
                    bool was_en_passant = current_index == chess->get_en_passant();
                    if (chess->move(turn, prev->get_index(), current_index)) {
                        if (was_en_passant) {
                            int offset = prev->get_chess_piece() == 'P'? -1: +1;
                            auto button = (ChessButton *) (*board)[(rank + offset) * 8 + file];
                            button->set_chess_piece(0);
                        }
                        set_chess_piece_from(prev, current);
                    } else unselect();
                    board->previous_button = nullptr;
                } else {
                    unselect();
                    auto p = chess->show(turn, current_index);
                    if (!p.empty()) {
                        std::copy(p.begin(), p.end(), back_inserter(path));
                        current->set_colour(yellow);
                        selected_pieces.push_back(current);
                        show(current);
                    }
                    board->previous_button = current;
                }
            }, chess->board[rank * 8 + file]));
        }
    }
    Box* reset_button = new Box(405, 10, true);
    reset_button->add_button(new Button(0, 0, 200 - 15 - 30, 30, blue, [&](Button* const& it) {
        reset();
    }, ""));
    reset_button->add_button(new Button(200 - 10 - 30, 0, 30, 30, null, [&](Button* const& it) {
        std::cout << chess->get_fen() << std::endl;
    }, "dot"));
    
    Window::add_box(board);
    Window::add_box(reset_button);
}

Game::~Game() {
    delete [] black;
    delete [] white;
    delete [] yellow;
    delete [] orange;
    delete [] null;
    delete chess;
}

void Game::hide(ChessButton* & button) {
    button->clear_target();
    for (auto const& it : path) {
        if (!(it >> 6)) ((ChessButton *) (*board)[it])->set_chess_piece(0);
    }
    path.clear();
}

void Game::show(ChessButton* & button) {
    for (const auto &it : path) {
        ChessButton* piece;
        if (it >> 6) {
            int6_t n = it;
            piece = (ChessButton *) (*board)[n.a];
            button->add_target(piece);
        } else {
            piece = (ChessButton *) (*board)[it];
            piece->set_chess_piece('d');
        }
    }
}

void Game::unselect() {
    if (!selected_pieces.empty()) {
        for (ChessButton* const& button : selected_pieces) {
            button->set_colour(button->colour_cpy);
        }
        selected_pieces.clear();
    }
}

void Game::reset() {
    std::cout << "resetting..." << std::endl;
    board->previous_button = nullptr;
    turn = 'w';
    unselect();
    path.clear();
    delete chess;
    chess = new Chess("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", 'w', "-", "-");
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            auto piece = (ChessButton *) (*board)[rank * 8 + file];
            piece->set_chess_piece(chess->board[rank * 8 + file]);
            piece->clear_target();
        }
    }
}

void Game::set_chess_piece_from(ChessButton * const& from, ChessButton * const& to) {
    turn = turn == 'w'? 'b': 'w';
    to->set_chess_piece(from->get_chess_piece());
    from->set_chess_piece(0);
    selected_pieces[0]->set_colour(orange);
    to->set_colour(yellow);
    selected_pieces.push_back(to);
}

void Game::launch() {
    while (!window.should_close()) window.render();
}