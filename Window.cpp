#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Window.hpp>
#include <iostream>

using namespace std;

vector<Box*> Window::boxes;
map<std::string, Texture*> Window::textures;
map<char, std::string> Window::piece_map = {
    {'r', "r"},
    {'n', "n"},
    {'b', "b"},
    {'q', "q"},
    {'k', "k"},
    {'R', "R_"},
    {'N', "N_"},
    {'B', "B_"},
    {'Q', "Q_"},
    {'K', "K_"},
    {'p', "p"},
    {'P', "P_"},
    {'d', "dot"},
    {'t', "target"}
};

InitOpenGL::InitOpenGL(int const& width, int const& height, string const& title) {
    if (!glfwInit()) {
        cerr << "[ERROR] unable to init glfw" << endl;
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwSetErrorCallback([](int e, const char* msg) { cerr << "[ERROR] (" << e << "): " << msg << endl; });
    glfwWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!glfwWindow) {
        cerr << "[ERROR] unable to create window" << endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent((GLFWwindow*) glfwWindow);
    if (glewInit() != GLEW_OK) {
        cerr << "[ERROR] unable to init glew";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
}

Window::Window(int const& width, int const& height, string const& title):
InitOpenGL(width, height, title),
vao {},
vbo {nullptr, 160000 * sizeof(float), GL_DYNAMIC_DRAW } {
    glfwSetKeyCallback((GLFWwindow*) glfwWindow, [](GLFWwindow* win, int key, int scancode, int action, int mods) {
        if (action == GLFW_RELEASE && key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(win, true);
        }
    });
    auto vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos((GLFWwindow*) glfwWindow, (vidmode->width - width) / 2, (vidmode->height - height) / 2);
    glfwSwapInterval(1);
    glfwShowWindow((GLFWwindow *) glfwWindow);
    glViewport(0, 0, width, height);
    glfwSetMouseButtonCallback((GLFWwindow *) glfwWindow, [](GLFWwindow* win, int button, int action, int mod) {
        auto pos = get_cursor_pos(win);
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !boxes.empty())
            for (const auto &item : boxes) if (item->is_active) item->click(pos.first, pos.second);
    });
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Layout layout;
    layout.add(2, nullptr);
    layout.add(4, (void*)(2 * sizeof(float)));
    layout.add(2, (void*)(6 * sizeof(float)));
    layout.add(1, (void*)(8 * sizeof(float)));
    vao.add_layout(vbo, layout);
}

Window::~Window() {
    for (const auto &item : boxes) delete item;
    glfwDestroyWindow((GLFWwindow *) glfwWindow);
    glfwTerminate();
    glfwSetErrorCallback(nullptr);
}

pair<double, double> Window::get_cursor_pos(void* const& win) {
    double x, y;
    glfwGetCursorPos((GLFWwindow*) win, &x, &y);
    return { x, 400 - y };
}

void Window::render(function<void()> const& callback) {
    glClear(GL_COLOR_BUFFER_BIT);
    vao.bind();
    callback();
    if (!boxes.empty())
        for (const auto &it : boxes) it->render();
    glfwSwapBuffers((GLFWwindow *) glfwWindow);
    glfwPollEvents();
}

void Window::add_box(Box* const& box) {
    Window::boxes.push_back(box);
}

bool Window::should_close() {
    return glfwWindowShouldClose((GLFWwindow *) glfwWindow);
}