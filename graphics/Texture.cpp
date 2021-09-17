#include <Texture.h>
#include <GL/glew.h>
#include <stb_image.h>

Texture::Texture(int const& slot, std::string const& path): m_texture(0), slot(slot) {
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    int width, height, channel;
    stbi_set_flip_vertically_on_load(true);
    auto buffer = stbi_load(path.c_str(), &width, &height, &channel, 4);
    if (buffer) {
        glTexImage2D(
             GL_TEXTURE_2D,
             0,
             GL_RGBA,
             width,
             height,
             0,
             GL_RGBA,
             GL_UNSIGNED_BYTE,
             buffer
         );
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load image at " << path << "!!!" << std::endl;
    }
    stbi_image_free(buffer);
}

void Texture::bind() const {
    glBindTextureUnit(slot, m_texture);
}

void Texture::un_bind() const {
    glBindTextureUnit(slot, m_texture);
}

int Texture::get_slot() const {
    return slot;
}