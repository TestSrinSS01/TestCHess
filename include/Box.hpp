#pragma once
#include <Button.hpp>
#include <VertexArray.h>

class Box {
private:
    std::vector<Button*> buttons;
    int x, y;
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