#include <iostream>
#include "Textures.h"
#include "stbi_load.h"      // Algum bug, deve ser incluido no .cpp

/////////////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
Texture::Texture() {

};
Texture::~Texture() {
};

/////////////////////////////////////////////////////////////////////////////////
// OpenGL Texture
void Texture::CreateBasicTexture(i32 texture_slot, i32 texture_type, i32 texture_interpolation_type, const char* path_image, bool flip_image, bool unbind) {
    CreateTexture(texture_slot, texture_type);
    ConfigTexture(texture_type, texture_interpolation_type);
    LoadImage(path_image, flip_image);
    ProcessImage(unbind);
};

void Texture::CreateTexture(i32 texture_slot, i32 texture_type) {
    glGenTextures(1, &TextureID);
    glActiveTexture(texture_slot);           // slot de textura que vamos modificar, um ID pode ter 16 slots
    // ativar antes do glBindTexture()
    glBindTexture(texture_type, TextureID);
    // gera a textura, ainda não preenchida
}

void Texture::ConfigTexture(i32 texture_type, i32 texture_interpolation_type) {
    glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, GL_REPEAT); 
    glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, texture_interpolation_type);     
    glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, texture_interpolation_type);                
}

/////////////////////////////////////////////////////////////////////////////////
// Load Assets

void Texture::LoadImage(const char* path_image, bool flip_image) {
    if (flip_image == true)
        stbi_set_flip_vertically_on_load(true);     // diz para carregar de forma invertida

    ImageData = stbi_load(path_image, &ImageWidth, &ImageHeight, &nColorChannel, 4);  // 4 é padrão
    if (!ImageData)
        InfoLog = "TEXTURES::FAILED_TO_LOAD";
}

void Texture::ProcessImage(bool unbind) {

    if (ImageData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ImageWidth, ImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImageData);       // transfere da RAM para a VRAM
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        InfoLog = "TEXTURES::FAILED_TO_PROCESS";
    }
    stbi_image_free(ImageData);  // Desaloca espaço da imagem

    if (unbind) 
        glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::ReleaseTexture() {
    if (TextureID != 0)
        glDeleteTextures(1, &TextureID);
}

/////////////////////////////////////////////////////////////////////////////////
// Getters

GLuint Texture::GetTextureID() {
    return TextureID;
}