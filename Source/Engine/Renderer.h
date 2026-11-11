#pragma once

// Lib
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <filesystem>
#include <unordered_map>
#include <regex>
#include <ranges>
#include <algorithm>

// Source
#include "Logger.h"
#include "../Managers/AssetManager.h"
#include "../Managers/FontManager.h"
#include "../Managers/InputManager.h"
#include "../Widgets/Common/Style.h"

#ifdef DrawText
#undef DrawText
#endif

namespace UI {
	
	enum class RenderMode { Rect = 0, Text = 1};

	enum class ShaderType { Vertex, Fragment, Program};

	enum class UniformType { Float, Vector2, Vector3, Vector4, Mat4 };

	class Shader {
	public:
		Shader(const std::filesystem::path& VertexShaderPath, const std::filesystem::path& FragmentShaderPath) {
			CheckSourcePath(VertexShaderPath, FragmentShaderPath);
			RefreshShader();
		};

		~Shader() {
			glDeleteProgram(m_ShaderProgram);
		}
		
		void RefreshShader() {
			if (!CheckSourceChanges()) return;

			// Source
			std::string vertexSourceString = AssetManager::Get().ReadFile(m_VertexPath);
			std::string fragmentSourceString = AssetManager::Get().ReadFile(m_FragmentPath);

			unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
			unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

			const char* vs = vertexSourceString.c_str();
			const char* fs = fragmentSourceString.c_str();

			glShaderSource(vertexShader, 1, &vs, nullptr);
			glShaderSource(fragmentShader, 1, &fs, nullptr);

			// Compilation
			glCompileShader(vertexShader);
			glCompileShader(fragmentShader);

			// Compilation status
			bool vertexStatus = CheckShaderCompilationStatus(ShaderType::Vertex, vertexShader, m_VertexPath);
			bool fragmentStatus = CheckShaderCompilationStatus(ShaderType::Fragment, fragmentShader, m_FragmentPath);

			if (!vertexStatus || !fragmentStatus) {
				m_CompilationStatus = false;
				return;
			}

			// Linking
			unsigned int program = glCreateProgram();
			glAttachShader(program, vertexShader);
			glAttachShader(program, fragmentShader);
			glLinkProgram(program);
		

			bool programStatus = CheckShaderCompilationStatus(ShaderType::Program, program, "");

			if (!programStatus) {
				m_CompilationStatus = false;
				return;
			}

			// Finishing
			if(m_ShaderProgram != static_cast<unsigned int>(-1)) Logger::Get().PrintToConsole("Shader hot reloaded!", LogSuccess);

			DeleteShader(ShaderType::Program, m_ShaderProgram);
			DeleteShader(ShaderType::Vertex, vertexShader);
			DeleteShader(ShaderType::Fragment, fragmentShader);
			
			m_ShaderProgram = program;
			SetUniforms(m_Uniforms);
			return;
		}

		int LoadUniformLocation(const std::string& uniform) {
			// Get uniform location
			int location = glGetUniformLocation(m_ShaderProgram, uniform.c_str());
			
			// Shader doesn't have a uniform with this name
			// if (location == -1) {
			//	 Logger::Get().PrintToConsole("Uniform: {} not found- ignore if not used in shader", LogWarning, uniform);
			// }

			// Load uniform loc
			m_UniformLocations[uniform] = location;
			return location;
		}

		int GetUniformLocation(const std::string& uniform) {
			auto it = m_UniformLocations.find(uniform);

			if (it != m_UniformLocations.end()) {
				return it->second;
			};

			int location = LoadUniformLocation(uniform);
			m_Uniforms.push_back(uniform);
			return location;
		}

		void SetUniforms(const std::vector<std::string>& uniforms) {
			m_Uniforms = uniforms;
			for (const auto& uniform : uniforms) {
				LoadUniformLocation(uniform);
			}
		}

		void SetUniform(const std::string& name, float value) {
			glUniform1f(GetUniformLocation(name), value);
		}

		void SetUniform(const std::string& name, int value) {
			glUniform1i(GetUniformLocation(name), value);
		}

		void SetUniform(const std::string& name, const glm::vec2& value) {
			glUniform2f(GetUniformLocation(name), value.x, value.y);
		}

		void SetUniform(const std::string& name, const glm::vec3& value) {
			glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
		}

		void SetUniform(const std::string& name, const glm::vec4& value) {
			glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
		}

		void SetUniform(const std::string& name, const glm::mat4& value) {
			glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
		}
	
		void Use() const {
			glUseProgram(m_ShaderProgram);
		}
	
	private:
		std::filesystem::path m_VertexPath;
		std::filesystem::path m_FragmentPath;
		std::filesystem::file_time_type m_VertexFileLastWrite;
		std::filesystem::file_time_type m_FragmentFileLastWrite;

		std::vector<std::string> m_Uniforms;
		std::unordered_map<std::string, unsigned int> m_UniformLocations;
		
		unsigned int m_ShaderProgram = -1;
		bool m_CompilationStatus = false;

		void DeleteShader(ShaderType type, int shader) const {
			switch (type) {
			case ShaderType::Vertex: glDeleteShader(shader); break;
			case ShaderType::Fragment: glDeleteShader(shader); break;
			case ShaderType::Program: glDeleteProgram(shader); break;
			};
		}

		void DetachShader(int shader) const {
			glDetachShader(m_ShaderProgram, shader);
		}

		bool CheckSourceChanges() {
			if (!CheckSourcePath(m_VertexPath, m_FragmentPath)) return false;

			std::filesystem::file_time_type vertexLast = std::filesystem::last_write_time(m_VertexPath);
			std::filesystem::file_time_type fragmentLast = std::filesystem::last_write_time(m_FragmentPath);

			if (vertexLast != m_VertexFileLastWrite || fragmentLast != m_FragmentFileLastWrite) {
				m_VertexFileLastWrite = vertexLast;
				m_FragmentFileLastWrite = fragmentLast;
				return true;
			}

			return false;
		}

		void PrintError(const std::string& info, ShaderType type, const std::filesystem::path& filePath) const {

			// Get error data
			std::regex pattern(R"((\d+)\((\d+)\)\s*:\s*(error|warning)\s+(\w+)\s*:\s*(.+))");
			std::smatch match;

			int lineNum{ 0 };
			std::string code{""};
			std::string message{ "" };
			if (std::regex_search(info, match, pattern)) {
				lineNum = std::stoi(match[2]); // 24
				code = match[4]; // "C0000"
				message = match[5]; // desc
			}

			// Get type
			std::string shaderType;
			switch (type) {
			case ShaderType::Vertex: shaderType = "Vertex"; break;
			case ShaderType::Fragment: shaderType = "Fragment"; break;
			case ShaderType::Program: shaderType = "Program"; break;
			}

			// Get nearby code rows
			std::string codeLines{"..... ]\n"};
			if (std::filesystem::exists(filePath)) {
				std::ifstream file(filePath);
				std::string line;
				unsigned int i{ 0 };
				unsigned int lineCount = 4;
				unsigned int targetLine = lineNum - 1;

				while (std::getline(file, line)) {
					if (i >= targetLine - lineCount && i <= targetLine + lineCount) {
						auto view = line |  std::views::drop_while([](unsigned char ch) {
							return std::isspace(ch);
						});
						codeLines += (i == targetLine ? "| >> " : "|    ") + std::to_string(i) + ":   " + std::string(view.begin(), view.end()) + "\n";
					}
					i++;
				}

				codeLines += +"[ .....";
			}
			Logger::Get().PrintToConsole("{} shader compile error - line {}:\n\n{}\n\n{}: {}", LogError, shaderType, lineNum, codeLines, code, message);

			return;
		};

		bool CheckShaderCompilationStatus(ShaderType type, unsigned int shader, const std::filesystem::path& filePath) const {
			int success;
			if (type == ShaderType::Program) {
				glGetProgramiv(shader, GL_LINK_STATUS, &success);
				if (!success) {
					int logLength;
					glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logLength);
					std::string infoLog(logLength, ' ');
					glGetProgramInfoLog(shader, logLength, nullptr, &infoLog[0]);
					DeleteShader(ShaderType::Program, shader);
					PrintError(infoLog, type, filePath);
				}
				return success;
			} 

			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				int logLength;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
				std::string infoLog(logLength, ' ');
				glGetShaderInfoLog(shader, logLength, nullptr, &infoLog[0]);
				DeleteShader(type, shader);
				PrintError(infoLog, type, filePath);
			}
			return success;
		}

		bool CheckSourcePath(const std::filesystem::path& VertexShaderPath, const std::filesystem::path& FragmentShaderPath) {
			if (!std::filesystem::exists(VertexShaderPath) || !std::filesystem::exists(FragmentShaderPath)) {
				Logger::Get().ParseText("Failed to load shader - files: {} or {} doesn't exist", VertexShaderPath.string(), FragmentShaderPath.string());
				return false;
			}

			m_VertexPath = VertexShaderPath;
			m_FragmentPath = FragmentShaderPath;
			return true;
		}
		
	};

	struct Vertex {
		glm::vec2 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
	};

	class Renderer {
	public:
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		static Renderer& Get() {
			static Renderer instance;
			return instance;
		}

		void BeginFrame(int screenWidth, int screenHeight);
		void DrawRect(glm::vec2 position, glm::vec2 size, const Style& style, bool debug = false, const std::string& imagePath ="");
		void DrawText(const std::string& fontName, const std::string& text, const glm::vec2& position, const glm::vec4& color, float fontSize, float availableWidth = 0, WrapMode wrapmode = WrapMode::None, bool debug = false);
		void DrawImage(glm::vec2 position, glm::vec2 size, const std::string& imagePath);
		void ShaderUpdateCheck() {
			if (lastCheckTime == 0.0f) lastCheckTime = static_cast<float>(glfwGetTime());
			float time = static_cast<float>(glfwGetTime());
			if (time - lastCheckTime > 0.5) { shader.RefreshShader(); lastCheckTime = time; };
		}

	private:

		void InitQuad();
		
		// Quad
		glm::mat4 m_Projection{ 1.0f };
		std::vector<Vertex> vertices = {
			// x, y           // r, g, b, a   // u, v
			{ { 0.0f, 0.0f }, { 1, 1, 1, 1 }, { 0.0f, 0.0f } }, // Bottom Left 
			{ { 1.0f, 0.0f }, { 1, 1, 1, 1 }, { 1.0f, 0.0f } }, // Bottom Right
			{ { 1.0f, 1.0f }, { 1, 1, 1, 1 }, { 1.0f, 1.0f } }, // Top Right
			{ { 0.0f, 1.0f }, { 1, 1, 1, 1 }, { 0.0f, 1.0f } }  // Top Left
		};
		std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0 };

		std::unordered_map<GLFWwindow*, GLuint> m_VAOs;
		unsigned int m_QuadVBO;
		unsigned int m_QuadIBO;

		GLuint GetCurrentVAO();

		// Shaders
		Shader shader = Shader(AssetManager::Get().GetShadersDir() / "shader.vs", AssetManager::Get().GetShadersDir() / "shader.fs");
		float lastCheckTime{0.0f};

		Renderer() { InitQuad(); }
		~Renderer() = default;
	};

}