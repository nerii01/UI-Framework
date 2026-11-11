#include "InputManager.h"
#include "../Widgets/Widget.h"
#include "WidgetManager.h"

namespace UI {

	//GLFW callbacks
	WindowContext* InputManager::GetWindowContext(GLFWwindow* window) {
		auto* context = static_cast<WindowContext*>(glfwGetWindowUserPointer(window));
		return context;
	}

	void InputManager::MousePosCallback(GLFWwindow* window, double xPos, double yPos) {
		auto* context = GetWindowContext(window);
		if (context == nullptr) return;

		InputManager* input = context->Input.get();
		if (input == nullptr) return;
		
		input->OnMouseMove(xPos, yPos);
	}

	void InputManager::MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
		auto* context = GetWindowContext(window);
		if (context == nullptr) return;

		InputManager* input = context->Input.get();
		if (input == nullptr) return;

		input->OnMouseScroll(xOffset, yOffset);
	}
	
	void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
		auto* context = GetWindowContext(window);
		if (context == nullptr) return;

		InputManager* input = context->Input.get();
		if (input == nullptr) return;

		input->OnMouseButton(button, action);
	}
	
	void InputManager::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		auto* context = GetWindowContext(window);
		if (context == nullptr) return;

		InputManager* input = context->Input.get();
		if (input == nullptr) return;

		input->OnKeyboardButton(key, action);
	}
	
	void InputManager::FileDropCallback(GLFWwindow* window, int count, const char** paths) {
		auto* context = GetWindowContext(window);
		if (context == nullptr) return;

		InputManager* input = context->Input.get();
		if (input == nullptr) return;
	
		std::vector<std::string> filePaths;
		filePaths.reserve(count);
		for (int i = 0; i < count; ++i) {
			filePaths.emplace_back(paths[i]);
		}
	
		input->OnFileDrop(filePaths);
	}
	
	glm::vec2  InputManager::GetCursorPosition() { return { m_MouseX, m_MouseY }; }
	
	Widget* InputManager::GetHoveredWidget() {
		// Obtain hovered widget
		glm::vec2 cursorPos = GetCursorPosition();
		Widget* hoveredWidget = nullptr;

		for (const auto& root : m_RootWidgets) {
			if (root == nullptr) continue;

			hoveredWidget = root->HitTest(cursorPos);
			if (hoveredWidget) return hoveredWidget;
		}

		return nullptr;
	}

	void InputManager::SetRootWidgets(const std::vector<Widget*>& roots) {
		m_RootWidgets.clear();

		for (auto root : roots) {
			if (root == nullptr) continue;
			m_RootWidgets.push_back(root);
		}
	}

	void InputManager::OnMouseMove(double xPos, double yPos) {
		m_MouseX = xPos;
		m_MouseY = yPos;
	}

	// Handlers

	void InputManager::HoverHandling() {
		Widget* hoveredWidget = GetHoveredWidget();
		if (hoveredWidget == m_HoveredWidget) return;

		// Old hovered target
		if (m_HoveredWidget != nullptr) {
			m_HoveredWidget->OnHoverLeave();
			for (Widget* ancestor = m_HoveredWidget->GetParent(); ancestor != nullptr; ancestor = ancestor->GetParent())
				ancestor->SetChildrenHovered(false);
		}

		m_HoveredWidget = hoveredWidget;

		// New hovered target
		if (m_HoveredWidget != nullptr) {
			m_HoveredWidget->OnHover();
			for (Widget* ancestor = m_HoveredWidget->GetParent(); ancestor != nullptr; ancestor = ancestor->GetParent())
				ancestor->SetChildrenHovered(true);
		}
	}
	
	void InputManager::OnMouseButton(int button, int action) {
		Widget* hoveredWidget = GetHoveredWidget();

		if (action == GLFW_PRESS) {
			if (hoveredWidget != m_FocusedWidget) {
				if (m_FocusedWidget != nullptr) {
					m_FocusedWidget->OnFocusLost();
					for (Widget* ancestor = m_FocusedWidget->GetParent(); ancestor != nullptr; ancestor = ancestor->GetParent())
						ancestor->SetChildrenFocused(false);
				}

				m_FocusedWidget = hoveredWidget;

				if (m_FocusedWidget != nullptr) {
					m_FocusedWidget->OnFocus();
					for (Widget* ancestor = m_FocusedWidget->GetParent(); ancestor != nullptr; ancestor = ancestor->GetParent())
						ancestor->SetChildrenFocused(true);
				}
			}

			Widget* target = hoveredWidget;
			while (target != nullptr) {
				if (target->OnPress(button)) {
					m_MouseCaptureMap[button] = target;
					return;
				}
				target = target->GetParent();
			}
			return;
		}

		if (action == GLFW_RELEASE) {
			auto it = m_MouseCaptureMap.find(button);
			if (it == m_MouseCaptureMap.end()) return; 
			it->second->OnRelease(button);
			m_MouseCaptureMap.erase(it);
		}
	}

	void InputManager::OnKeyboardButton(int key, int action) {
		// Press
		if (action == GLFW_PRESS) {
			Widget* target = m_FocusedWidget;
			
			while (target != nullptr) {
				if (target->OnKeyPress(key)) {
					m_KeyCaptureMap[key] = target;
					return;
				}
				target = target->GetParent();
			}
		}

		// Release
		if (action == GLFW_RELEASE) { 
			auto it = m_KeyCaptureMap.find(key);
			if (it == m_KeyCaptureMap.end()) return;
			it->second->OnKeyRelease(key);
			m_KeyCaptureMap.erase(it);
		};
	}

	void InputManager::OnMouseScroll(double xOffset, double yOffset)
	{
		Widget* target = m_HoveredWidget;

		while (target != nullptr) {
			if (target->OnScrollInput(glm::vec2(xOffset, yOffset))) return;
			target = target->GetParent();
		}
	}
	
	void InputManager::OnFileDrop(const std::vector<std::string>& paths)
	{
		for (auto p : paths) {
			Logger::Get().PrintToConsole("Dropped file: {}", LogInfo, p);
		}
	}
}