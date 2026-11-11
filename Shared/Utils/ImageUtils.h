#pragma once

// Source
#include <vector>
#include <unordered_map>
#include <filesystem>

namespace ImageUtils {

	enum class ImageChannels {
		Auto = 0,
		R = 1, 
		RG = 2,
		RGB = 3,
		RGBA = 4 
	};

	struct Image {
		int Width, Height;
		ImageChannels Channels = ImageChannels::Auto;
		std::vector<unsigned char> Pixels;
	};

	struct Rect {
		int x, y;
		int Width, Height;
	};

	struct AtlasEntry {
		uint16_t ID;
		Image image;
	};

	struct Atlas {
		Image Image;
		std::unordered_map<uint16_t, Rect> Rects;
	};

	// Read / Write
	Image ReadImage(const std::filesystem::path& path, ImageChannels channels = ImageChannels::Auto);
	void WriteImage(const std::filesystem::path& path, const Image& image);

	// Single Distance Field
	Image GenerateSDF(const Image& image, int maxDistance);

	// Effects
	void ApplyPadding(Image& image, int padding);
	void Downscale(Image& image);

	// Atlas
	Atlas GenerateAtlas(const std::vector<AtlasEntry>& entries, int width = 4096, int height = 4096, int spacing = 16);
	//Atlas ReadAtlasData(const std::filesystem::path& path);
	//void SaveAtlasData(const Atlas& atlas, const std::filesystem::path& path);
}
