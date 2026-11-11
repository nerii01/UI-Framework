#pragma once
#include "stb_truetype.h"

#include <string>
#include <vector>
#include <map>
#include <filesystem>

class FontKit {
public:
	struct Glyph {
		Glyph(int c, int w, int h, int xo, int yo, int pad, int ax, int ay, int xa)
			: code(c), width(w), height(h), xoff(xo), yoff(yo),
			padding(pad), atlasX(ax), atlasY(ay), xAdvance(xa) {
		}

		Glyph() = default;

		std::vector<unsigned char> pixels;
		int code, width, height, xoff, yoff, padding, atlasX, atlasY, xAdvance;
	};

	struct Font {
		std::map<unsigned int, Glyph> glyphs;
		std::vector<unsigned char> atlasPixels;
		int ascent, descent, lineGap;
		bool loaded;
		float fontSize;
		int atlasWidth, atlasHeight;
		unsigned int atlasTexture = 0;
	};

	static bool CreateFont(const std::filesystem::path& fontFile, const std::filesystem::path& bakedFontsDir);
	static bool ReadFont(const std::filesystem::path& fontDir, Font& font);

private:
	// Processing
	static void ConvertToSDF(Glyph& glyph, int maxDistance);
	static void Downsample2X(Glyph& glyph);
	static void ApplyPadding(Glyph& glyph, int padding);

	// Loading / Reading
	static bool LoadGlyph(char character, stbtt_fontinfo& fontInfo, float scale, Glyph& glyph);
	static std::vector<unsigned char> ReadFontBuffer(const std::filesystem::path& path);
	static bool ReadFontGlyphs(const std::filesystem::path& path, std::map<unsigned int, FontKit::Glyph>& glyphs);
	static bool ReadFontData(const std::filesystem::path& path, Font& font);
	static std::string GetFontName(const std::filesystem::path& path);

	// Saving
	static void SaveFontAtlas(const std::filesystem::path& path, std::vector<Glyph>& glyphs, int maxWidth);
	static void SaveGlpyhsData(const std::filesystem::path& path, const std::vector<Glyph>& glyphs);
	static void SaveFontData(const std::filesystem::path& path, stbtt_fontinfo& fontInfo);

	// Helpers
	static void PrintDebugGlyphsData(const std::vector<Glyph>& glyphs);

	// Variables
	static constexpr int m_AtlasSize = 4096;
	static constexpr float m_FontSize = 512.0f;
};


