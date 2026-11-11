#pragma once

#include <fstream>
#include <filesystem>
#include <string>

namespace UI {
	class AssetManager {
	public:
		AssetManager(const AssetManager&) = delete;
		AssetManager& operator=(const AssetManager&) = delete;
		static AssetManager& Get() {
			static AssetManager instance;
			return instance;
		}

		std::string ReadFile(const std::filesystem::path& path);

		std::filesystem::path GetBaseDir();
		std::filesystem::path GetResourceDir();
		std::filesystem::path GetShadersDir();
		std::filesystem::path GetFontsDir();
		std::filesystem::path GetBakedFontsDir();
		std::filesystem::path GetImagesDir();

	private:
		std::filesystem::path FindProjectRoot(const std::string& marker = "Resources");

		// Base directories
		AssetManager()
			: m_BaseDir(FindProjectRoot("Resources"))
			, m_ResourceDir(m_BaseDir / "Resources")
			, m_ShaderDir(m_BaseDir / "Resources" / "Shaders")
			, m_FontsDir(m_BaseDir / "Resources" / "Fonts")
			, m_ImagesDir(m_BaseDir / "Resources" / "Images")
			, m_BakedFontsDir(m_BaseDir / "Resources" / "Fonts" / "Baked") {
		};
		~AssetManager() = default;

		std::filesystem::path m_BaseDir;
		std::filesystem::path m_ResourceDir;
		std::filesystem::path m_ShaderDir;
		std::filesystem::path m_FontsDir;
		std::filesystem::path m_ImagesDir;
		std::filesystem::path m_BakedFontsDir;
	};

}