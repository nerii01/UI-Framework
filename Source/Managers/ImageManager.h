#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <unordered_map>

namespace UI {

	struct ImageData {
		int Width, Height, channels;
		unsigned int ID = 0;
		unsigned char* buffer;
	};

	class ImageManager {
	public:
		ImageManager(const ImageManager&) = delete;
		ImageManager& operator=(const ImageManager&) = delete;

		static ImageManager& Get() {
			static ImageManager instance;
			return instance;
		}

		unsigned int GetImage(const std::string& imagePath);

	private:

		unsigned int GenerateTexture(ImageData& imageBuffer);
		ImageData LoadImage(const std::string& imagePath);
		std::unordered_map<std::string, unsigned int> m_LoadedImages;
		ImageManager() = default;
		~ImageManager() = default;
	};

}

