#include "..\include\Shader.h"
#include <Shader.h>
#include <GL/glew.h>

unsigned int compile(const unsigned int& type, char const* const& src) {
    auto shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        int len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        char* msg = new char[len];
        glGetShaderInfoLog(shader, len, &len, msg);
        std::cerr << "unable to compile " << ((type == GL_VERTEX_SHADER)? "vertex": "fragment") << "\n" << msg << std::endl;
        delete msg;
    }
    return shader;
}

void check_err(unsigned int const& program, unsigned int const& p_name) {
    int success;
    glGetProgramiv(program, p_name, &success);
    if (!success) {
        int len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        char* msg = new char[len];
        glGetProgramInfoLog(program, len, &len, msg);
        std::cerr << msg << std::endl;
        delete msg;
    }
}

unsigned int create_program(std::string const& vertex_src, std::string const& fragment_src) {
    auto program = glCreateProgram();
    auto vertex = compile(GL_VERTEX_SHADER, vertex_src.c_str());
    auto fragment = compile(GL_FRAGMENT_SHADER, fragment_src.c_str());
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    check_err(program, GL_LINK_STATUS);
    glValidateProgram(program);
    check_err(program, GL_VALIDATE_STATUS);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return program;
}

Shader::Shader(std::string const& vertex_src, std::string const& fragment_src):
m_program(create_program(vertex_src, fragment_src)) {}

Shader::~Shader() {
    glDeleteProgram(m_program);
}

void Shader::active() const {
    glUseProgram(m_program);
}

void Shader::setUniformMat4f(char const* const& name, float const* const& mat) const {
    active();
    glUniformMatrix4fv(glGetUniformLocation(m_program, name), 1, GL_FALSE, mat);
}

void Shader::setUniform1f(char const* const& name, float const& f) const {
    active();
    glUniform1f(glGetUniformLocation(m_program, name), f);
}

void Shader::setUniform1i(char const* const& name, int const& i) const {
    active();
    glUniform1i(glGetUniformLocation(m_program, name), i);
}

void Shader::setUniform1iv(char const* const& name, int* const& i) const {
    active();
    glUniform1iv(glGetUniformLocation(m_program, name), 2, i);
}
