#include "stb_image.h"
#include "stb_image_write.h"
#include "FontKit.h"  
#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <ranges>


// Public
bool FontKit::CreateFont(const std::filesystem::path& fontFile, const std::filesystem::path& bakedFontsDir) {
	// Read font binary
	std::vector<unsigned char> buffer = ReadFontBuffer(fontFile);
	if (buffer.empty()) return false;
	std::string fontName = GetFontName(fontFile);

	// Init stbtt font
	stbtt_fontinfo fontInfo;
	stbtt_InitFont(&fontInfo, buffer.data(), 0);

	// Glyph generation
	float fontScale = stbtt_ScaleForPixelHeight(&fontInfo, m_FontSize);

	std::vector<Glyph> glyphs;
	for (int i{ 0 }; i <= 94; ++i) {
		char Character = char(32 + i);
		std::string fileName = "Glyphs/" + std::to_string(Character) + ".png";

		Glyph glyph;
		LoadGlyph(Character, fontInfo, fontScale, glyph);
		ApplyPadding(glyph, 16);
		ConvertToSDF(glyph, 8);
		//Downsample2X(glyph);
		glyphs.push_back(glyph);
		//stbi_write_png(fileName.c_str(), glyph.width, glyph.height, 1, glyph.pixels.data(), 0);
	}
	//PrintDebugGlyphsData(glyphs);
	std::filesystem::create_directories(bakedFontsDir / fontName);
	std::filesystem::path atlasDir = bakedFontsDir / fontName / "atlas.png";
	std::filesystem::path glyphsDataDir = bakedFontsDir / fontName / "data.glyphs";
	std::filesystem::path fontDataDir = bakedFontsDir / fontName / "data.font";

	SaveFontAtlas(atlasDir, glyphs, m_AtlasSize);
	SaveGlpyhsData(glyphsDataDir, glyphs);
	SaveFontData(fontDataDir, fontInfo);
	return true;
}
bool FontKit::ReadFont(const std::filesystem::path& fontDir, Font& font) {
	if (!std::filesystem::exists(fontDir)) {
		std::cout << "[ Error ] Invalid font directory: " << fontDir << '\n';
		return false;
	}

	std::string fontName = GetFontName(fontDir);

	// Glyphs
	std::map<unsigned int, Glyph> glyphs;
	if (!ReadFontGlyphs(fontDir / "data.glyphs", glyphs)) return false;
	
	// Atlas
	std::filesystem::path atlasPath(fontDir / "atlas.png");
	int width, height, channels;
	unsigned char* data = stbi_load(atlasPath.string().c_str(), &width, &height, &channels, 1);
	if (!data) {
		std::cout << "[ Error ] Cannot load atlas: " << fontDir << '\n';
		return false;
	}

	// Font
	if(!ReadFontData(fontDir / "data.font", font)) return false;
	font.loaded = true;
	font.glyphs = glyphs;
	font.atlasPixels.assign(data, data + (width * height));
	font.atlasWidth = width;
	font.atlasHeight = height;

	return true;
}

// Private
// Processing
void FontKit::ConvertToSDF(Glyph& glyph, int maxDistance) {
	std::vector<unsigned char> SDFBitmap(glyph.width * glyph.height, 0);

	for (unsigned int y1{ 0 }; y1 < glyph.height; ++y1) {
		for (unsigned int x1{ 0 }; x1 < glyph.width; ++x1) {

			// Pixel and its inside
			unsigned char pixel1 = glyph.pixels[(y1 * glyph.width) + x1];
			bool inside1 = pixel1 > 127;
			float best = glyph.width * glyph.height;

			unsigned int y2 = std::max(0, int(y1) - maxDistance);

			unsigned int y2Limit = std::min(glyph.height, int(y1) + maxDistance);
			unsigned int x2Limit = std::min(glyph.width, int(x1) + maxDistance);

			for (y2; y2 < y2Limit; ++y2) {
				unsigned int x2 = std::max(0, int(x1) - maxDistance);
				for (x2; x2 < x2Limit; ++x2) {
					unsigned char pixel2 = glyph.pixels[(y2 * glyph.width) + x2];
					bool inside2 = pixel2 > 127;
					if (inside1 == inside2) continue;

					float distanceX = (float)x1 - (float)x2;
					float distanceY = (float)y1 - (float)y2;
					float distance = distanceX * distanceX + distanceY * distanceY;
					best = std::min(best, distance);
				}
			}

			best = std::sqrt(best);
			float signedDist = inside1 ? best : -best;
			float normalized = signedDist / maxDistance;
			SDFBitmap[(y1 * glyph.width) + x1] = (uint8_t)std::clamp(normalized * 127 + 127, 0.0f, 255.0f);
		}
	}

	glyph.pixels = std::move(SDFBitmap);
}
void FontKit::Downsample2X(Glyph& glyph) {
	int downscaledWidth = glyph.width / 2;
	int downscaledHeight = glyph.height / 2;
	std::vector<unsigned char> downsampledBitmap(downscaledWidth * downscaledHeight, 0);


	for (unsigned int y{ 0 }; y < glyph.height; y += 2) {
		for (unsigned int x{ 0 }; x < glyph.width; x += 2) {

			float p1 = glyph.pixels[(y * glyph.width) + x];
			float p2 = glyph.pixels[(y * glyph.width) + x + 1];
			float p3 = glyph.pixels[((y + 1) * glyph.width) + x];
			float p4 = glyph.pixels[((y + 1) * glyph.width) + x + 1];
			float avg = (p1 + p2 + p3 + p4) / 4;
			downsampledBitmap[(y / 2) * downscaledWidth + (x / 2)] = avg;
		}
	}

	glyph.width = downscaledWidth;
	glyph.height = downscaledHeight;
	glyph.pixels = std::move(downsampledBitmap);
	glyph.xoff /= 2;
	glyph.yoff /= 2;
	glyph.xAdvance /= 2;


	//std::cout << "GLYPH [ " << glyph.code << " ] " << downscaledWidth << " x " << downscaledHeight << '\n';
}
void FontKit::ApplyPadding(Glyph& glyph, int padding) {
	int paddedW = glyph.width + (2 * padding);
	int paddedH = glyph.height + (2 * padding);
	if (paddedW % 2 != 0) paddedW++;
	if (paddedH % 2 != 0) paddedH++;
	std::vector<unsigned char> paddedBitmap(paddedW * paddedH, 0);

	for (unsigned int y{ 0 }; y < glyph.height; ++y) {
		for (unsigned int x{ 0 }; x < glyph.width; ++x) {
			unsigned char pixel = glyph.pixels[(y * glyph.width) + x];
			paddedBitmap[((y + padding) * paddedW) + x + padding] = pixel;
		}
	}

	glyph.pixels = std::move(paddedBitmap);
	glyph.width = paddedW;
	glyph.height = paddedH;
	glyph.padding = padding;
	glyph.xoff -= padding;
	glyph.yoff -= padding;
}

// Loading / Reading
bool FontKit::LoadGlyph(char character, stbtt_fontinfo& fontInfo, float scale, Glyph& glyph) {
	int code = stbtt_FindGlyphIndex(&fontInfo, (int)character);
	if (code < 0) return false;
	int w, h, xoff, yoff, xAdvance, ascent, descent, lineGap;
	unsigned char* bitmap = stbtt_GetGlyphBitmap(&fontInfo, scale, scale, code, &w, &h, &xoff, &yoff);

	stbtt_GetGlyphHMetrics(&fontInfo, code, &xAdvance, 0);

	glyph.width = w;
	glyph.height = h;
	glyph.xoff = xoff;
	glyph.yoff = yoff;
	glyph.code = int(character);
	glyph.xAdvance = (int)std::round(xAdvance * scale);
	glyph.pixels.assign(bitmap, bitmap + (w * h));

	stbtt_FreeBitmap(bitmap, nullptr);

	return true;
}
bool FontKit::ReadFontGlyphs(const std::filesystem::path& path, std::map<unsigned int, FontKit::Glyph>& glyphs) {
	std::fstream file(path);
	if (!file.is_open()) {
		std::cout << "[ Error ] Cannot open: " << path.string() << '\n';
		return false;
	}

	std::string line;
	bool skipFirst{ true };

	while (std::getline(file, line)) {
		if (skipFirst) { skipFirst = false; continue; };

		std::vector<std::string> parts;
		std::string lineFinal = line.c_str() + 2;

		for (auto part : lineFinal | std::views::split('|')) {
			parts.emplace_back(part.begin(), part.end());
		}


		int code = int(line[0]);
		int atlasX = std::stoi(parts[0]);
		int atlasY = std::stoi(parts[1]);
		int width = std::stoi(parts[2]);
		int height = std::stoi(parts[3]);
		int xoff = std::stoi(parts[4]);
		int yoff = std::stoi(parts[5]);
		int xAdvance = std::stoi(parts[6]);
		int padding = std::stoi(parts[7]);

		Glyph newGlyph = Glyph(code, width, height, xoff, yoff, padding, atlasX, atlasY, xAdvance);
		glyphs[code] = newGlyph;
	}

	return true;
}
std::vector<unsigned char> FontKit::ReadFontBuffer(const std::filesystem::path& path) {
	std::ifstream file(path, std::ios::binary | std::ios::ate);
	if (!file.is_open()) {
		std::cout << "[ Error ] Cannot open: " << path << '\n';
		return {};
	}
	std::streamsize fileSize = file.tellg();
	std::vector<unsigned char> fileBuffer(fileSize, 0);
	file.seekg(0, std::ios::beg);
	file.read(reinterpret_cast<char*>(fileBuffer.data()), fileSize);
	return fileBuffer;
}
bool FontKit::ReadFontData(const std::filesystem::path& path, Font& font) {
	std::fstream file(path);
	if (!file.is_open()) {
		std::cout << "[ Error ] Cannot open: " << path << '\n';
		return false;
	}
	std::string line;

	bool skipFirst{ true };
	while (std::getline(file, line)) {
		if (skipFirst) { skipFirst = false; continue; };

		std::vector<std::string> parts;

		for (auto part : line | std::views::split('|')) {
			parts.emplace_back(part.begin(), part.end());
		}

		font.ascent = std::stoi(parts[0]);
		font.descent = std::stoi(parts[1]);
		font.lineGap = std::stoi(parts[2]);
		font.fontSize = std::stoi(parts[3]);
	}

	return true;
}
std::string FontKit::GetFontName(const std::filesystem::path& path) {
	return path.stem().string();
}

// Saving
void FontKit::SaveFontAtlas(const std::filesystem::path& path, std::vector<Glyph>& glyphs, int maxWidth) {
	// Measure glyphs and determine available size - TODO LATER NOT THAT MUCH OF A DEAL
	unsigned int height = maxWidth;
	std::vector<unsigned char> atlasBitmap(maxWidth * height, 0);

	int margin = 64;
	int rowHeight = 0;
	int offsetX = 0;
	int offsetY = 0;

	for (auto& glyph : glyphs) {

		if ((offsetX + glyph.width) > maxWidth) {
			offsetX = 0;
			offsetY += rowHeight;
			rowHeight = 0;
		}

		for (unsigned int y{ 0 }; y < glyph.height; y++) {
			for (unsigned int x{ 0 }; x < glyph.width; x++) {

				unsigned char pixel = glyph.pixels[(y * glyph.width) + x];
				atlasBitmap[((y + offsetY) * maxWidth) + offsetX + x] = pixel;
			}
		}

		glyph.atlasX = offsetX;
		glyph.atlasY = offsetY;

		offsetX += glyph.width + margin;
		rowHeight = std::max(rowHeight, glyph.height + margin);
	}

	stbi_write_png(path.string().c_str(), maxWidth, height, 1, atlasBitmap.data(), 0);
}
void FontKit::SaveGlpyhsData(const std::filesystem::path& path, const std::vector<Glyph>& glyphs) {
	std::ofstream file(path);
	std::string fileContent;
	std::string initialMessage{ "code | atlasX | atlasY | width | height | xoff | yoff | xAdvance | padding  ------ SKIP FIRST LINE -------\n" };
	file << initialMessage.c_str();
	char separator = '|';

	for (const auto& glyph : glyphs) {
		std::string line{ "" };
		line += char(glyph.code);
		line += separator;
		line += std::to_string(glyph.atlasX);
		line += separator;
		line += std::to_string(glyph.atlasY);
		line += separator;
		line += std::to_string(glyph.width);
		line += separator;
		line += std::to_string(glyph.height);
		line += separator;
		line += std::to_string(glyph.xoff);
		line += separator;
		line += std::to_string(glyph.yoff);
		line += separator;
		line += std::to_string(glyph.xAdvance);
		line += separator;
		line += std::to_string(glyph.padding);
		fileContent += line + '\n';
	}

	file << fileContent.c_str();
}
void FontKit::SaveFontData(const std::filesystem::path& path, stbtt_fontinfo& fontInfo) {
	std::ofstream file(path);
	std::string fileContent;
	std::string initialMessage{ "ascent | descent | lineGap | fontSize  ------ SKIP FIRST LINE -------\n" };
	file << initialMessage.c_str();
	char separator = '|';

	int ascentRaw, descentRaw, lineGapRaw;
	stbtt_GetFontVMetrics(&fontInfo, &ascentRaw, &descentRaw, &lineGapRaw);
	float scale = m_FontSize / (ascentRaw - descentRaw);


	int ascent = (int)std::round(ascentRaw * scale);
	int descent = (int)std::round(descentRaw * scale);
	int lineGap = (int)std::round(lineGapRaw * scale);

	std::string line{ "" };
	line += std::to_string(ascent);
	line += separator;
	line += std::to_string(descent);
	line += separator;
	line += std::to_string(lineGap);
	line += separator;
	line += std::to_string(m_FontSize);
	
	fileContent += line + '\n';
	file << fileContent.c_str();
}

// Helpers
void FontKit::PrintDebugGlyphsData(const std::vector<Glyph>& glyphs) {
	for (auto& glyph : glyphs) {
		std::cout << "[ ";
		std::cout << char(glyph.code);
		std::cout << " ] " << "x: " << glyph.atlasX << " y: " << glyph.atlasY << "    -    size: " << glyph.width << "x" << glyph.height << "    -   off: " << glyph.xoff << "x" << glyph.yoff << '\n';
	}
}