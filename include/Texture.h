#pragma once
#include <iostream>
class Texture {
private:
    unsigned int m_texture;
    const int slot;
public:
    explicit Texture(int const& slot, std::string const& path);
    ~Texture();
    int get_slot() const;
    void bind() const;
    void un_bind() const;
};
