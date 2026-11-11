#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

namespace UI {
	enum class LogType{
		Info,
		Warning,
		Error,
		Success,
		Debug
	};

	// Shortcuts
	inline constexpr auto LogInfo = LogType::Info;
	inline constexpr auto LogWarning = LogType::Warning;
	inline constexpr auto LogError = LogType::Error;
	inline constexpr auto LogSuccess = LogType::Success;
	inline constexpr auto LogDebug = LogType::Debug;

	class Logger {
	public:
		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;
		static Logger& Get() {
			static Logger instance;
			return instance;
		}

		template <typename... Args>
		void PrintToConsole(const std::string& text, const LogType logType, Args... args) {
			const char* c_RESET = "\033[0m";
			const char* c_INFO = "\033[38;2;135;206;250m";
			const char* c_WARNING = "\033[38;2;255;225;53m";
			const char* c_ERROR = "\033[38;2;255;105;97m";
			const char* c_SUCCESS = "\033[38;2;144;238;144m";
			const char* c_DEBUG = "\033[38;2;255;182;193m";

			const char* color = "";
			const char* label = "";

			switch (logType) {
			case LogType::Info:    label = "INFO";    color = c_INFO;   break;
			case LogType::Warning: label = "WARNING"; color = c_WARNING; break;
			case LogType::Error:   label = "ERROR";   color = c_ERROR;    break;
			case LogType::Success: label = "SUCCESS"; color = c_SUCCESS;  break;
			case LogType::Debug: label = "DEBUG"; color = c_DEBUG;  break;
			}

			std::string info = ParseText(text, args...);
			std::cout << color << "[ " << label << " ] " << info << c_RESET << std::endl;
		}

		template <typename... Args>
		std::string ParseText(const std::string& text, Args... args) {
			std::stringstream stream;
			ParseText_Impl(text, stream, args...);
			return stream.str();
		}

	private:

		// General
		template <typename T, typename... Args>
		void ParseText_Impl(const std::string& text, std::stringstream& stream, T arg, Args... args) {
			size_t index = text.find("{");
			if (index == std::string::npos || text.size() < index + 2) {
				stream << text;
				return;
			}
			stream << text.substr(0, index);
			stream << "[ " << arg << " ]";
			ParseText_Impl(text.substr(index + 2), stream, args...);
		}

		// Vector
		template <typename Element, typename... Args>
		void ParseText_Impl(const std::string& text, std::stringstream& stream, const std::vector<Element>& vec, Args... args) {
			size_t index = text.find("{");
			if (index == std::string::npos || text.size() < index + 2) {
				stream << text;
				return;
			}
			stream << text.substr(0, index);
			stream << "[ ";
			for (size_t i = 0; i < vec.size(); ++i) {
				stream << vec[i];
				if (i + 1 < vec.size()) stream << ", ";
			}
			stream << " ]";
			ParseText_Impl(text.substr(index + 2), stream, args...);
		}

		// No args
		void ParseText_Impl(const std::string& text, std::stringstream& stream) {
			stream << text;
		}

		Logger() = default;
		~Logger() = default;
	};

}
