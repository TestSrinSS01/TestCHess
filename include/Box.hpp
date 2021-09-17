#pragma once
#include <Button.hpp>
#include <VertexArray.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Box {
private:
    std::vector<Button*> buttons;
    Shader shader;
    int x, y;
    int tex[32];
public:
    bool is_active;
    Button* previous_button = nullptr;
public:
    Box(int const& x, int const& y, bool const& is_active);
    ~Box();
    void render();
    void add_button(Button* const& button);
    void click(double const& _x, double const& _y);
    Button* operator[](int index);
};