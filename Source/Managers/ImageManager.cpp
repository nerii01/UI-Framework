#include "ImageManager.h"
#include "AssetManager.h"
#include "stb_image.h"
#include <glad/glad.h>

namespace UI {

    unsigned int ImageManager::GetImage(const std::string& imagePath) {
        auto it = m_LoadedImages.find(imagePath);
        if (it != m_LoadedImages.end()) {
            return it->second;
        }

        auto img = LoadImage(imagePath);
        if (!img.buffer) return 0; // Zabezpieczenie przed brakiem pliku

        unsigned int textureID = GenerateTexture(img);

        // Zapisujemy wygenerowane ID do cache!
        m_LoadedImages[imagePath] = textureID;
        return textureID;
    }

    ImageData ImageManager::LoadImage(const std::string& imagePath) {
        std::filesystem::path imageFile = AssetManager::Get().GetImagesDir() / (imagePath + ".png");

        if (!std::filesystem::exists(imageFile)) return {};

        ImageData image{};
        // Wymuszamy 4 kanały (RGBA) - uniwersalne dla większości obrazków i ikonek
        image.buffer = stbi_load(imageFile.string().c_str(), &image.Width, &image.Height, &image.channels, STBI_rgb_alpha);

        // NIE wywołujemy stbi_image_free tutaj!
        return image;
    }

    unsigned int ImageManager::GenerateTexture(ImageData& imageBuffer) {
        if (!imageBuffer.buffer) return 0;

        glGenTextures(1, &imageBuffer.ID);
        glBindTexture(GL_TEXTURE_2D, imageBuffer.ID);

        // GL_CLAMP_TO_EDGE zapobiega powstawaniu dziwnych ramek na krawędziach przezroczystości
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // ZMIANA: Wyłączamy filtrowanie z mipmapami!
      // Dla obu filtrów użyj GL_LINEAR:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, imageBuffer.Width, imageBuffer.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer.buffer);


        // Dopiero po przesłaniu pikseli do VRAM zwalniamy RAM z stb_image
        stbi_image_free(imageBuffer.buffer);
        imageBuffer.buffer = nullptr;

        return imageBuffer.ID;
    }
}