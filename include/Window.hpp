#pragma once
#include <ChessButton.hpp>
#include <map>
#include <iostream>
#include <Shader.h>
#include <VertexArray.h>
#include <Texture.h>
#include <functional>
#include <Box.hpp>

using namespace std;

class InitOpenGL {
protected:
    void* glfwWindow;
public:
    InitOpenGL(int const& width, int const& height, string const& title);
};

class Window: InitOpenGL {
private:
    static vector<Box*> boxes;
    VertexArray vao;
    VertexBuffer vbo;
    Shader shader;
    int tex[31];
public:
    static map<std::string, Texture*> textures;
    static map<char, std::string> piece_map;
public:
    Window(int const& width, int const& height, string const& title);
    ~Window();
    static pair<double, double> get_cursor_pos(void* const& win);
    void render(function<void()> const& = [](){});
    static void add_box(Box* const& box);
    bool should_close();
    template<typename... T> static void load_texture(int const& slot, std::string path, T &... names) {
        vector<string> arr { names... };
        for (const auto &name : arr) {
            Window::textures.insert({ name, new Texture(slot,  path + "textures\\" + name + ".png") });
        }
    }
};