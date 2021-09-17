#include <iostream>
#include <vector>
#include <map>
struct int6_t { 
public:
    unsigned int a : 6; 
    int6_t() : a(0) {}
    int6_t(unsigned int const& a) : a(a) {}
};
class Chess {
public:
    char board[64];
private:
    struct Square {
        unsigned int rank: 3;
        unsigned int file: 3;
    };
    char turn;
    struct can_castle {
        [[maybe_unused]] bool queen_side, king_side;
    } can_black_castle, can_white_castle;
    int offsets[8];
    int knight_offsets[8][2];
    int king_offsets[8][2];
    unsigned int **number_of_squares_to_edge_cache;
    int en_passant;
    [[maybe_unused]] bool is_king_in_check;
    const std::map<char, std::string> pieces;
public:
    Chess(const std::string& fen, const char& turn, const std::string& castle, std::string const& en_passant);
    ~Chess();
    void render() const;
    [[nodiscard]] int get_en_passant() const;
    bool move(const char& player, const int& start_index, const int& end_index);
    std::vector<int> show(const char& player, const int& start_index);
    std::vector<int> get_sliding_piece_moves(const int& cell, const char& piece);
    std::vector<int> get_non_sliding_piece_moves(const int& cell, const char& piece);
    std::vector<int> get_pawn_piece_moves(int const& cell, char const& piece);
    [[nodiscard]] std::string get_fen() const;
};