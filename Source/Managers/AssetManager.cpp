#include "AssetManager.h"

#include <fstream>

// Source
#include "../Engine/Logger.h"

namespace UI {

	std::string AssetManager::ReadFile(const std::filesystem::path& path) {
		std::fstream file(path);
		if (!file.is_open()) {
			Logger::Get().PrintToConsole("Opening file: {}", LogType::Error, path);
			return "";
		}

		std::string line;
		std::string content;
		while (std::getline(file, line)) {
			content += line + '\n';
		}

		return content;
	}

	std::filesystem::path AssetManager::GetBaseDir() { return m_BaseDir; }
	std::filesystem::path AssetManager::GetResourceDir() { return m_ResourceDir; }
	std::filesystem::path AssetManager::GetShadersDir() { return m_ShaderDir; }
	std::filesystem::path AssetManager::GetFontsDir() { return m_FontsDir; }
	std::filesystem::path AssetManager::GetImagesDir() { return m_ImagesDir; }
	std::filesystem::path AssetManager::GetBakedFontsDir() { return m_BakedFontsDir; }
	std::filesystem::path AssetManager::FindProjectRoot(const std::string& marker) {
		std::filesystem::path dir = std::filesystem::current_path();
		while (true) {
			if (std::filesystem::exists(dir / marker)) {
				return dir;
			}

			std::filesystem::path parent = dir.parent_path();
			if (parent == dir) {
				throw std::runtime_error("Nie znaleziono katalogu z markerem: " + marker);
			}
			dir = parent;
		}

		return dir;
	}
}