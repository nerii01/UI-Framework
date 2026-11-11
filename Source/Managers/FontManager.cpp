#include "FontManager.h"

#include <glad/glad.h>
#include <ranges>

// Source
#include "../Engine/Logger.h"
#include "AssetManager.h"

namespace UI {
		
		std::vector<std::string> FontManager::WrapText(const std::string& text, const std::string& fontName, float fontSize, float availableWidth, WrapMode wrapMode) {
			return CalculateTextLayout(text, fontName, fontSize, availableWidth, wrapMode).Lines;
		}

		glm::vec2 FontManager::MeasureText(const std::string& text, const std::string& fontName, float fontSize, float availableWidth, WrapMode wrapMode) {
			return CalculateTextLayout(text, fontName, fontSize, availableWidth, wrapMode).Size;
		}

		FontKit::Font* FontManager::GetFont(const std::string& fontName) {
			// Loaded
			auto it = m_LoadedFonts.find(fontName);
			if (it != m_LoadedFonts.end()) {
				return it->second.get();
			}

			auto& assetMgr = AssetManager::Get();
			std::filesystem::path bakedFontDir = assetMgr.GetBakedFontsDir() / fontName;

			// Search for TTF
			if (!std::filesystem::exists(bakedFontDir)) {
				std::filesystem::path fontFile = assetMgr.GetFontsDir() / (fontName + ".ttf");

				if (!std::filesystem::exists(fontFile) || !FontKit::CreateFont(fontFile, assetMgr.GetBakedFontsDir())) {
					Logger::Get().PrintToConsole("Font {} doesn't exist in {}", LogType::Error, fontName, fontFile.string());
					return nullptr;
				}
			}

			// Baked font reading
			auto font = std::make_unique<FontKit::Font>();

			if (FontKit::ReadFont(bakedFontDir, *font)) {
				GenerateFontAtlas(*font);

				FontKit::Font* fontPtr = font.get();
				m_LoadedFonts[fontName] = std::move(font);
				return fontPtr;
			}

			return nullptr;
		}

		std::vector<std::string> FontManager::GetLoadedFonts() {
			std::vector<std::string> loadedFonts{};
			std::string loadedFontsString{ "" };
			loadedFonts.reserve(m_LoadedFonts.size());

			for (const auto& [fontName, font] : m_LoadedFonts) {
				loadedFonts.push_back(fontName);
				loadedFontsString += fontName + ' ';
			}

			Logger::Get().PrintToConsole("Loaded fonts: {}", LogType::Info, loadedFontsString);
			return loadedFonts;
		}

		TextData FontManager::CalculateTextLayout(std::string_view text, const std::string& fontName, float fontSize, float availableWidth, WrapMode wrapMode)
		{
			auto font = GetFont(fontName);
			if (!font) return { .Lines = {}, .Size = {0.0f, 0.0f} };
			TextData data;
			data.Lines.emplace_back();

			float scale = fontSize / font->fontSize;
			float lineHeight = std::abs(font->ascent - font->descent) * scale;

			float lineX = 0.0f;
			float lineY = 0.0f;

			float maxWidth = 0.0f;
			float height = lineHeight;

			float top = 0.0f;
			float bottom = 0.0f;

			int currentLine = 0;
			std::string_view originText = text;
			
			// Wrapping
			std::string_view currentWord;
			int lastSpace = -1;
			float lastWordSize = 0;
	
			for (size_t i = 0; i < originText.size(); ++i) {
				char character = originText[i];
				bool isLastChar = (i + 1 == originText.size());

				auto it = font->glyphs.find(character);
				if (it == font->glyphs.end()) continue;
				const auto& glyph = it->second;
				float charWidth = glyph.xAdvance * scale;

				// Word
				if (wrapMode == WrapMode::Word) {
					lastWordSize += charWidth;
					if (character != ' ' && !isLastChar) continue;

					// Word
					int wordStart = std::max(0, lastSpace + 1);
					currentWord = originText.substr(wordStart, i - lastSpace);

					// Overflows
					if (lastWordSize + lineX > availableWidth) {
						data.Lines.emplace_back(currentWord);
						++currentLine;
						lineX = lastWordSize;
						height += lineHeight;
					}
					else {
						data.Lines[currentLine] += currentWord;
						lineX += lastWordSize;
					}

					maxWidth = std::max(lineX, maxWidth);
					lastSpace = i;
					lastWordSize = 0;
					continue;
				}

				// Elipsis
				if (wrapMode == WrapMode::Elipsis && (lineX + charWidth) > availableWidth) {
					float ellipsisWidth = 0.0f;
					if (auto dotIt = font->glyphs.find('.'); dotIt != font->glyphs.end()) {
						ellipsisWidth = dotIt->second.xAdvance * scale * 3.0f;
					}

					size_t fitCount = i;
					while (fitCount > 0 && (lineX + ellipsisWidth) > availableWidth) {
						char prevChar = originText[fitCount - 1];
						if (auto prevIt = font->glyphs.find(prevChar); prevIt != font->glyphs.end()) {
							lineX -= prevIt->second.xAdvance * scale;
						}
						--fitCount;
					}

					data.Lines[currentLine] = originText.substr(0, fitCount);
					data.Lines[currentLine] += "...";
					maxWidth = lineX + ellipsisWidth;
					break;
				}

				// Char
				if (wrapMode == WrapMode::Char && std::round(lineX + charWidth) > availableWidth) {
					data.Lines.push_back({ character });
					maxWidth = std::max(maxWidth, lineX);
					lineX = charWidth;
					++currentLine;
					height += lineHeight;
					continue;
				}

				// Default
				lineX += charWidth;
				data.Lines[currentLine] += character;
				maxWidth = std::max(maxWidth, lineX);
			};
			
			// Fix underline 
			height = height - (scale * 100.0f);

			data.Size = { maxWidth, height  };
			return data;
		}
		
		void  FontManager::GenerateFontAtlas(FontKit::Font& font) {
			//Generate atlas texture
			unsigned int texture{ 0 };
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, font.atlasWidth, font.atlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, font.atlasPixels.data());
			glGenerateMipmap(GL_TEXTURE_2D);
			font.atlasPixels.clear();
			font.atlasTexture = texture;
		}
}