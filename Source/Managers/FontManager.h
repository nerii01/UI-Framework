#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "../../Thirdparty/FontKit/FontKit.h"

namespace UI {

	enum class WrapMode {
		None,
		Word,
		Char,
		Elipsis
	};

	struct TextData {
		std::vector<std::string> Lines;
		glm::vec2 Size;
	};

	class FontManager {
	public:
		FontManager(const FontManager&) = delete;
		FontManager& operator=(const FontManager&) = delete;

		static FontManager& Get() {
			static FontManager instance;
			return instance;
		}

		std::vector<std::string> GetLoadedFonts();
		FontKit::Font* GetFont(const std::string& fontName);

		std::vector<std::string> WrapText(const std::string& text, const std::string& fontName, float fontSize, float availableWidth, WrapMode wrapMode);
		glm::vec2 MeasureText(const std::string& text, const std::string& fontName, float fontSize, float availableWidth, WrapMode wrapMode);

	private:
		
		TextData CalculateTextLayout(std::string_view text, const std::string& fontName, float fontSize, float availableWidth, WrapMode wrapMode);
		void  GenerateFontAtlas(FontKit::Font& font);
		std::map<std::string, std::unique_ptr<FontKit::Font>> m_LoadedFonts;
		FontManager() = default;
		~FontManager() = default;
	};

}

