#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>
#include <string>

namespace UI {
	
	class Widget;
	struct WindowContext;

	class InputManager {
	public:
		InputManager() = default;
		~InputManager() = default;

		// GLFW callbacks
		static WindowContext* GetWindowContext(GLFWwindow* window);
		static void MousePosCallback(GLFWwindow* window, double xPos, double yPos);
		static void MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void FileDropCallback(GLFWwindow* window, int count, const char** paths);
		void HoverHandling();

		glm::vec2 GetCursorPosition();
		Widget* GetHoveredWidget();
		
		void SetRootWidgets(const std::vector<Widget*>& roots);

	private:
		std::vector<Widget*> m_RootWidgets{};
		Widget* m_HoveredWidget = nullptr;
		Widget* m_FocusedWidget = nullptr;
		double m_MouseX{ 0 };
		double m_MouseY{ 0 };

		std::unordered_map<int, Widget*> m_MouseCaptureMap;
		std::unordered_map<int, Widget*> m_KeyCaptureMap;
		void OnMouseScroll(double xOffset, double yOffset);
		void OnMouseButton(int button, int action);
		void OnMouseMove(double xPos, double yPos);
		void OnKeyboardButton(int key, int action);
		void OnFileDrop(const std::vector<std::string>& paths);
	
	};
}

