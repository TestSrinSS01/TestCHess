#pragma once
#include <functional>
#include <Shader.h>
#include <vector>
class Button {
protected:
    const int x, y, width, height;
    float const* colour;
    std::function<void(Button* const&)> callback;
    const std::string texture;
public:
    Button(
       int const& x,
       int const& y,
       int const& width,
       int const& height,
       float const* const& colour,
       std::function<void(Button* const&)> callback,
       const std::string& texture = ""
    );
    void add_offsets(int const& _x, int const& _y);
    void click(double const& _x, double const& _y);
    [[nodiscard]] bool hover(double const& _x, double const& _y) const;
    virtual std::vector<float> get_vertices();
    virtual void bind();
};