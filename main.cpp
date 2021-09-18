#include <Game.hpp>
#include <sstream>

using namespace std;

int main(int n, char** args) {
    /*try {
        Game game;
        game.launch();
    }
    catch (exception const& e) {
        std::cout << e.what() << std::endl;
    }
    */
    try {
        stringstream ss;
        ss << args[0] << "\\..\\";
        Window window(600, 400, "Chess");
        std::cout << "loading textures..." << std::endl;
        Window::load_texture(1, ss.str(), "r");
        Window::load_texture(2, ss.str(), "n");
        Window::load_texture(3, ss.str(), "b");
        Window::load_texture(4, ss.str(), "q");
        Window::load_texture(5, ss.str(), "k");
        Window::load_texture(6, ss.str(), "R_");
        /*Window::load_texture(7, ss.str(), "N_");
        Window::load_texture(8, ss.str(), "B_");
        Window::load_texture(9, ss.str(), "Q_");
        Window::load_texture(10, ss.str(), "K_");
        Window::load_texture(11, ss.str(), "P_"); 
        Window::load_texture(12, ss.str(), "p"); 
        Window::load_texture(13, ss.str(), "dot"); 
        Window::load_texture(14, ss.str(), "target"); */
        std::cout << "done loading textures." << std::endl;
        Box* box1 = new Box(0, 0, true);
        float black[] = { 0, 0, 0 };
        float white[] = { 1, 1, 1 };
        Chess chess("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", 'w', "-", "-");
        for (int rank = 0; rank < 8; rank++) {
            for (int file = 0; file < 8; file++) {
                box1->add_button(new ChessButton(file * 50, rank * 50, 50, 50, (rank + file) % 2 == 0 ? black : white, [&, rank, file](Button* const& it) {
                    std::cout << chess.board[rank * 8 + file] << std::endl;
                }, 'k')); // chess.board[rank * 8 + file]
            }
        }
        /*Box* box2 = new Box(405, 10, true);
        float blue[] = { 0, 0, 1 };
        box2->add_button(new Button(0, 0, 200 - 10 - 30, 30, blue, [](Button* const& it) {
            std::cout << "resetting" << std::endl;
        }));
        box2->add_button(new Button(200 - 15 - 30, 0, 30, 30, black, [&](Button* const& it) {
            std::cout << chess.get_fen() << std::endl;
        }, "download"));*/
        Window::add_box(box1);
        //Window::add_box(box2);
        while (!window.should_close()) {
            window.render();
        }
    } catch (exception const& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
