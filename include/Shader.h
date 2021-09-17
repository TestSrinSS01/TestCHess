#pragma once
#include <iostream>
class Shader {
private:
    unsigned int m_program;
public:
    Shader(std::string const& vertex_src, std::string const& fragment_src);
    ~Shader();
    void active() const;
    void setUniform1i(char const* const& name, int const& i) const; 
    void setUniform1iv(char const* const& name, int* const& i) const; 
    void setUniform1f(char const* const& name, float const& f) const;
    void setUniformMat4f(char const* const& name, float const* const& mat) const;
};
