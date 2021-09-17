#include <chess.hpp>
#include <sstream>
#include <vector>

bool is_digit(char const& ch) { return ch >= '0' && ch <= '9'; }

int digit_char_to_int(char const& ch) { return ch - 48; }

bool is_sliding_piece(const char& ch) { return ch == 'r' || ch == 'b' || ch == 'q' || ch == 'R' || ch == 'B' || ch == 'Q'; }

int cell_to_index(const std::string &cell) {
    if (cell == "-") return -1;
    int rank = digit_char_to_int(cell[1]) - 1;
    int file = cell[0] - 'a';
    return rank * 8 + file;
}

Chess::Chess(const std::string &fen, const char &turn, const std::string &castle, std::string const& en_passant):
pieces({
    {'r', "♖"},
    {'n', "♘"},
    {'b', "♗"},
    {'q', "♕"},
    {'k', "♔"},
    {'R', "♜"},
    {'N', "♞"},
    {'B', "♝"},
    {'Q', "♛"},
    {'K', "♚"},
    {'p', "♙"},
    {'P', "♟"}
}),
is_king_in_check(false),
board { 0 },
turn(turn),
can_black_castle { false, false },
can_white_castle { false, false },
offsets { 8, -8, 1, -1, 7, -7, 9, -9 },
knight_offsets { { 2, -1 }, { -2, 1 }, { -1, -2 }, { 1, 2 }, { 2, 1 }, { -2, -1 }, { 1, -2 }, { -1, 2 } },
king_offsets { { 1, 0 }, { -1, 0 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { -1, 1 }, { 1, 1 }, { -1, -1 } },
number_of_squares_to_edge_cache(new unsigned int*[64]),
en_passant(cell_to_index(en_passant)) {
    int file = 0, rank = 7;
    for (const auto &it : fen) {
        if (it == '/') { rank--; file = 0; }
        else if (is_digit(it)) {
            file += digit_char_to_int(it);
        } else {
            board[rank * 8 + file] = it;
            file++;
        }
    }
    if (castle.find('K')) can_white_castle.king_side = true;
    if (castle.find('Q')) can_white_castle.queen_side = true;
    if (castle.find('k')) can_black_castle.king_side = true;
    if (castle.find('q')) can_black_castle.queen_side = true;
    for (rank = 0; rank < 8; ++rank) {
        for (file = 0; file < 8; ++file) {
            unsigned int north = 7 - rank;
            unsigned int south = rank;
            unsigned int west = file;
            unsigned int east = 7 - file;
            number_of_squares_to_edge_cache[rank * 8 + file] = new unsigned int[8] {
                north, south, east, west, std::min(north, west), std::min(south, east), std::min(north, east), std::min(south, west)
            };
        }
    }
}

void Chess::render() const {
    std::stringstream files;
    files << " |";
    for (int i = 7; i >= 0; --i) {
        files << "\t" << (char) ('a' + (7 - i));
        std::stringstream rank;
        for (int j = 0; j <= 7; ++j) {
            char piece = board[i * 8 + j];
            if (piece == 0) rank << "⃞" << "\t";
            else rank << pieces.at(piece) << "\t";
        }
        std::cout << i + 1 << "|\t" << rank.str() << std::endl;
    }
    std::cout << "__________________________________" << std::endl;
    std::cout << files.str() << std::endl;
}

Chess::~Chess() {
    for (int i = 0; i < 64; i++)
        delete[] number_of_squares_to_edge_cache[i];
    delete[] number_of_squares_to_edge_cache;
}

bool is_white(const char& piece) {
    return piece >= 'A' && piece <= 'Z';
}

bool is_friendly_piece(const char& piece1, const char& piece2) {
    return (!is_white(piece1) && !is_white(piece2)) || (is_white(piece1) && is_white(piece2));
}

std::vector<int> Chess::get_pawn_piece_moves(int const& cell, const char &piece) {
    std::vector<int> moves;
    Square square {};
    square.rank = cell >> 3;
    square.file = cell;
    int directions[] = { piece == 'P'? 4: 5, piece == 'P'? 6: 7 };
    int n = (piece == 'p' && 7u - square.rank == 1u) || (piece == 'P' && square.rank == 1u)? 2: 1;
    for (int i = 1; i <= n; ++i) {
        int target_cell = cell + offsets[piece == 'P'? 0: 1] * i;
        if (board[target_cell] == 0) moves.push_back(target_cell);
    }
    for (const auto &direction : directions) {
        int target_cell = cell + offsets[direction];
        char piece_at_target_cell = board[target_cell];
        if ((piece_at_target_cell != 0 && !is_friendly_piece(piece, piece_at_target_cell)) || target_cell == en_passant)
            moves.push_back(target_cell | 64);
    }
    return moves;
}

std::vector<int> Chess::get_non_sliding_piece_moves(int const& cell, char const& piece) {
    std::vector<int> moves;
    Square cell_square {};
    cell_square.rank = cell >> 3;
    cell_square.file = cell;
    bool is_knight = piece == 'n' || piece == 'N';
    for (int direction = 0; direction < 8; ++direction) {
        int target_cell;
        auto offset = is_knight? knight_offsets[direction]: king_offsets[direction];
        int rank = (int) cell_square.rank + offset[0];
        int file = (int) cell_square.file + offset[1];
        if (rank < 0 || rank > 7 || file < 0 || file > 7) continue;
        target_cell = rank * 8 + file;
        char piece_at_target_cell = board[target_cell];
        if (piece_at_target_cell != 0 && is_friendly_piece(piece, piece_at_target_cell)) continue;
        if (piece_at_target_cell != 0 && !is_friendly_piece(piece, piece_at_target_cell)) {
            moves.push_back(target_cell | 64);
            continue;
        }
        moves.push_back(target_cell);
    }
    return moves;
}

std::vector<int> Chess::get_sliding_piece_moves(int const& cell, char const& piece) {
    int start_direction_index = piece == 'b' || piece == 'B'? 4: 0;
    int end_direction_index = piece == 'r' || piece == 'R'? 4: 8;
    std::vector<int> moves;
    for (int direction = start_direction_index; direction < end_direction_index; ++direction) {
        for (int number_of_squares = 0; number_of_squares < number_of_squares_to_edge_cache[cell][direction]; ++number_of_squares) {
            int target_cell = cell + offsets[direction] * (number_of_squares + 1);
            char piece_at_target_cell = board[target_cell];
            if (piece_at_target_cell != 0 && is_friendly_piece(piece, piece_at_target_cell)) break;
            else if (piece_at_target_cell != 0 && !is_friendly_piece(piece, piece_at_target_cell)) {
                moves.push_back(target_cell | 64);
                break;
            }
            else moves.push_back(target_cell);
        }
    }
    return moves;
}

std::vector<int> Chess::show(const char &player, const int& start_index) {
    std::vector<int> moves;
    if (player != turn) return moves;
    char start_piece = board[start_index];
    if ((turn == 'w' && !is_white(start_piece)) || (turn == 'b' && is_white(start_piece))) return moves;
    if (start_piece == 0) return moves;
    if (is_sliding_piece(start_piece)) {
        moves = get_sliding_piece_moves(start_index, start_piece);
    } else if (start_piece == 'p' || start_piece == 'P') {
        moves = get_pawn_piece_moves(start_index, start_piece);
    } else {
        moves = get_non_sliding_piece_moves(start_index, start_piece);
    } return moves;
}

bool Chess::move(const char &player, const int& start_index, const int& end_index) {
    auto moves = show(player, start_index);
    if (moves.empty()) return false;
    char start_piece = board[start_index];
    char end_piece = board[end_index];
    if (end_piece != 0 && is_friendly_piece(start_piece, end_piece)) return false;
    if (start_piece == 'p' || start_piece == 'P') {
        int start_rank = start_index >> 3;
        int end_rank = end_index >> 3;
        if (abs(end_rank - start_rank) == 2) {
            en_passant = start_index + offsets[start_piece == 'P'? 0: 1];
        } else {
            if (end_index == en_passant) {
                int offset = offsets[start_piece == 'P'? 1: 0];
                std::cout << "en_passant_move, piece at " << end_index + offset << " = " << board[end_index + offset] << std::endl;
                board[end_index + offset] = 0;
                std::cout << "and now it is " << board[end_index + offset] << std::endl;
            }
            en_passant = -1;
        }
    } else en_passant = -1;
    for (auto& item : moves) {
        if (item >> 6) {
            int6_t n = item;
            item = (int)n.a;
        }
    }
    if (std::find(moves.begin(), moves.end(), end_index) == moves.end()) return false;
    board[end_index] = start_piece;
    board[start_index] = 0;
    turn = turn == 'w'? 'b': 'w';
    return true;
}

std::string Chess::get_fen() const {
    std::stringstream ss;
    for (int i = 7; i >= 0; --i) {
        int blank = 0;
        for (int j = 0; j < 8; ++j) {
            auto piece = board[i * 8 + j];
            if (piece != 0) {
                if (blank) ss << blank;
                ss << piece;
                blank = 0;
            } else if (j + 1 == 8 && blank != 0)
                ss << blank + 1;
            else blank++;
        } ss << (i == 0? " ": "/");
    } ss << turn << " - ";
    if (en_passant > 0) {
        Square sq {};
        sq.rank = en_passant >> 3;
        sq.file = en_passant;
        char str[3] = { 0 };
        str[0] = (char) ('a' + (int) sq.file);
        str[1] = (char) (sq.rank + 49u);
        ss << str;
    } else ss << "-";
    return ss.str();
}

int Chess::get_en_passant() const {
    return en_passant;
}
