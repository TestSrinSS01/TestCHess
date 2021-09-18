#include <Game.hpp>
#include <sstream>
#include <GL/glew.h>

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
        const std::string path = ss.str();
        Window window(600, 400, "Chess");
        Texture black_king(1, path + "textures/k.png");
        Texture white_king(2, path + "textures/K_.png");
        Texture white_queen(3, path + "textures/Q_.png");
        black_king.bind();
        white_king.bind();
        white_queen.bind();
        while (!window.should_close()) {
            window.render([&]() {
                float buffer[] = {
                    0,  0,      1, 1, 1, 1,     0, 0,     0,
                    50, 0,      1, 1, 1, 1,     1, 0,     0,
                    50, 50,     1, 1, 1, 1,     1, 1,     0,
                    50, 50,     1, 1, 1, 1,     1, 1,     0,
                    0,  50,     1, 1, 1, 1,     0, 1,     0,
                    0,  0,      1, 1, 1, 1,     0, 0,     0,

                    0,  0,      0, 0, 0, 0,     0, 0,     2,
                    50, 0,      0, 0, 0, 0,     1, 0,     2,
                    50, 50,     0, 0, 0, 0,     1, 1,     2,
                    50, 50,     0, 0, 0, 0,     1, 1,     2,
                    0,  50,     0, 0, 0, 0,     0, 1,     2,
                    0,  0,      0, 0, 0, 0,     0, 0,     2,
                };
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof buffer, buffer);
                glDrawArrays(GL_TRIANGLES, 0, sizeof buffer / sizeof * buffer);
            });
        }
    } catch (exception const& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
