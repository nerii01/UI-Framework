#include "ImageUtils.h"

// Source
#include "../../Source/Managers/AssetManager.h"
#include <iostream>
#include <limits>
#include <stb_image.h>
#include <stb_image_write.h>

namespace ImageUtils {

	// Read / Write
	Image ImageUtils::ReadImage(const std::filesystem::path& path, ImageChannels channels) {
		Image image;
		int nrChannels{ 0 };
		unsigned char* buffer = stbi_load(path.string().c_str(), &image.Width, &image.Height, &nrChannels, static_cast<int>(channels));
		if (!buffer) return image;
		int loadedChannels = (channels == ImageChannels::Auto) ? nrChannels : static_cast<int>(channels);
		
		image.Channels = static_cast<ImageChannels>(loadedChannels);
		image.Pixels.assign(buffer, buffer + (image.Width * image.Height * loadedChannels));
	
		stbi_image_free(buffer);
		return image;

	}

	void ImageUtils::WriteImage(const std::filesystem::path& path, const Image& image) {
		if (image.Pixels.empty() || image.Width <= 0 || image.Height <= 0) return;
		int channelsCount = static_cast<int>(image.Channels);
		int stride = image.Width * channelsCount;
		stbi_write_png(path.string().c_str(), image.Width, image.Height, channelsCount, image.Pixels.data(), stride);
	}

	// Rewrite - Single Distance Field
	Image ImageUtils::GenerateSDF(const Image& image, int maxDistance) {
		const int w = image.Width;
		const int h = image.Height;
		const int nrChannels = static_cast<int>(image.Channels);
		const int targetChannel = nrChannels == 4 ? 3 : 0;
		const uint8_t threshold = 128;
		constexpr float INF = 1e20f;

		Image sdfImg;
		sdfImg.Channels = ImageChannels::R;
		sdfImg.Width = w;
		sdfImg.Height = h;
		std::vector<unsigned char> sdfPixels(static_cast<size_t>(w) * h);

		// --- Binarna maska "inside" ---
		std::vector<uint8_t> insideMask(static_cast<size_t>(w) * h);
		for (int row = 0; row < h; ++row) {
			const unsigned char* srcRow = &image.Pixels[static_cast<size_t>(row) * w * nrChannels];
			uint8_t* dstRow = &insideMask[static_cast<size_t>(row) * w];
			for (int col = 0; col < w; ++col) {
				dstRow[col] = srcRow[col * nrChannels + targetChannel] > threshold ? 1 : 0;
			}
		}

		// --- Bufory robocze algorytmu Felzenszwalta-Huttenlochera ---
		const int n = std::max(w, h);
		std::vector<float> f(n), d(n), z(n + 1);
		std::vector<int> v(n);

		// 1D dolna obwiednia parabol - kwadrat odległości do najbliższego "feature" (f==0)
		auto dt1d = [&](int len) {
			int k = 0;
			v[0] = 0;
			z[0] = -INF;
			z[1] = INF;
			for (int q = 1; q < len; ++q) {
				float s;
				for (;;) {
					const float fq = f[q] + static_cast<float>(q) * q;
					const float fv = f[v[k]] + static_cast<float>(v[k]) * v[k];
					s = (fq - fv) / (2.0f * static_cast<float>(q - v[k]));
					if (s <= z[k]) --k;
					else break;
				}
				++k;
				v[k] = q;
				z[k] = s;
				z[k + 1] = INF;
			}
			k = 0;
			for (int q = 0; q < len; ++q) {
				while (z[k + 1] < static_cast<float>(q)) ++k;
				const float diff = static_cast<float>(q - v[k]);
				d[q] = diff * diff + f[v[k]];
			}
			};

		std::vector<float> tmp(static_cast<size_t>(w) * h);

		// Pełny separowalny 2D EDT: najpierw kolumny, potem wiersze.
		// isFeature(x,y) == true  ->  ten piksel jest "źródłem" (odległość 0)
		auto computeEDT = [&](auto isFeature, std::vector<float>& out) {
			for (int x = 0; x < w; ++x) {
				for (int y = 0; y < h; ++y) f[y] = isFeature(x, y) ? 0.0f : INF;
				dt1d(h);
				for (int y = 0; y < h; ++y) tmp[static_cast<size_t>(y) * w + x] = d[y];
			}
			for (int y = 0; y < h; ++y) {
				for (int x = 0; x < w; ++x) f[x] = tmp[static_cast<size_t>(y) * w + x];
				dt1d(w);
				for (int x = 0; x < w; ++x) out[static_cast<size_t>(y) * w + x] = d[x];
			}
			};

		// distBg: dla pikseli "inside" -> kwadrat odległości do najbliższego tła
		// distFg: dla pikseli "outside" -> kwadrat odległości do najbliższego pierwszego planu
		std::vector<float> distBg(static_cast<size_t>(w) * h);
		std::vector<float> distFg(static_cast<size_t>(w) * h);

		computeEDT([&](int x, int y) { return insideMask[static_cast<size_t>(y) * w + x] == 0; }, distBg);
		computeEDT([&](int x, int y) { return insideMask[static_cast<size_t>(y) * w + x] != 0; }, distFg);

		// --- Złożenie SDF ---
		const float maxDistF = static_cast<float>(maxDistance);
		const float invMaxDist = 1.0f / maxDistF;

		for (int row = 0; row < h; ++row) {
			const size_t base = static_cast<size_t>(row) * w;
			for (int col = 0; col < w; ++col) {
				const bool inside = insideMask[base + col] != 0;
				const float distSq = inside ? distBg[base + col] : distFg[base + col];

				float dist = std::sqrt(distSq);
				dist = std::min(dist, maxDistF);
				dist = inside ? dist : -dist;

				float normalized = std::clamp((dist * invMaxDist + 1.0f) * 0.5f, 0.0f, 1.0f);
				sdfPixels[base + col] = static_cast<unsigned char>(normalized * 255.0f);
			}
		}

		sdfImg.Pixels = std::move(sdfPixels);
		return sdfImg;
	}
	
	// Effects
	void ImageUtils::ApplyPadding(Image& image, int padding) {
		int nrChannels = static_cast<int>(image.Channels);
		int nw = image.Width + (2 * padding);
		int nh = image.Height + (2 * padding);

		std::vector<unsigned char> pixels;
		pixels.assign(nw * nh * nrChannels, 0);

		for (auto row{ 0 }; row < image.Height; row++) {
			for (auto col{ 0 }; col < image.Width; col++) {
				for (int ch = 0; ch < nrChannels; ch++) {
					int srcIdx = (row * image.Width + col) * nrChannels + ch;
					int dstIdx = ((row + padding) * nw + (col + padding)) * nrChannels + ch;
					pixels[dstIdx] = image.Pixels[srcIdx];
				}
			}
		}

		image.Width = nw;
		image.Height = nh;
		image.Pixels = std::move(pixels);
	}

	void ImageUtils::Downscale(Image& image) {
		int nrChannels = static_cast<int>(image.Channels);
		int nw = std::round(image.Width / 2);
		int nh = std::round(image.Height / 2);
		std::vector<unsigned char> pixels;
		pixels.assign(nw * nh * nrChannels, 0);
		
		for (auto row{ 0 }; row < nh; row++) {
			for (auto col{ 0 }; col < nw; col++) {
				for (auto ch{ 0 }; ch < nrChannels; ch++) {
					// Calculate AVG
					unsigned char p1 = image.Pixels[((row * 2) * image.Width + (col * 2)) * nrChannels + ch];
					unsigned char p2 = image.Pixels[((row * 2) * image.Width + (col * 2 + 1)) * nrChannels + ch];
					unsigned char p3 = image.Pixels[((row * 2 + 1) * image.Width + (col * 2)) * nrChannels + ch];
					unsigned char p4 = image.Pixels[((row * 2 + 1) * image.Width + (col * 2 + 1)) * nrChannels + ch];
					unsigned char avg = static_cast<unsigned char>((p1 + p2 + p3 + p4) / 4);

					int dstIdx = (row * nw + col) * nrChannels + ch;
					pixels[dstIdx] = avg;
				}
			}
		}

		image.Width = nw;
		image.Height = nh;
		image.Pixels = std::move(pixels);
	}

	// Atlas
	Atlas ImageUtils::GenerateAtlas(const std::vector<AtlasEntry>& entries, int width, int height, int spacing) {
		Atlas atlas;
		if (entries.empty() || width == 0 || height == 0) return atlas;

		int padding = 32;
		int nrChannels = static_cast<int>(entries[0].image.Channels);
		int rowHeight {padding}, offsetX{ padding }, offsetY{ padding };
		std::vector<unsigned char> pixels(width*height*nrChannels,0);

		for (auto i{ 0 }; i < entries.size(); ++i) {
			const AtlasEntry& entry = entries[i];
			const Image& img = entry.image;
			if (img.Pixels.empty()) continue;
			if (static_cast<int>(img.Channels) != nrChannels) continue;

			// Overflow
			if (img.Width + spacing + offsetX > width) {
				offsetX = padding;
				offsetY += rowHeight + spacing;
				rowHeight = padding;
			}

			if (offsetY + img.Height > height) break;

			// Image -> Atlas
			for (auto row{ 0 }; row < img.Height; ++row) {
				for (auto col{ 0 }; col < img.Width; ++col) {
					int srcIdx = (row * img.Width + col) * nrChannels;
					int dstX = offsetX + col;
					int dstY = offsetY + row;
					int dstIdx = (dstY * width + dstX) * nrChannels;
					pixels[dstIdx] = img.Pixels[srcIdx];
				}
			}

			Rect imgRect;
			imgRect.x = offsetX;
			imgRect.y = offsetY;
			imgRect.Width = img.Width;
			imgRect.Height = img.Height;
			atlas.Rects[entry.ID] = std::move(imgRect);


			offsetX += img.Width + spacing;
			rowHeight = std::max(img.Height, rowHeight);
		}

		atlas.Image.Pixels = std::move(pixels);
		atlas.Image.Channels = entries[0].image.Channels;
		atlas.Image.Width = width;
		atlas.Image.Height = height;

		return atlas;	
	}

}
