#pragma once


#define GLFW_EXPOSE_NATIVE_WIN32

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>

struct HWND__;
typedef HWND__* HWND;

namespace UI {

	class Window
	{
	public:
		// Initialization
		Window() = default;
		~Window();

		bool Init(const std::string& title, int width, int height, GLFWwindow* share = nullptr);

		// Title
		void SetWindowTitle(const std::string& title);

		// Sizing
		glm::vec<2, int> GetWindowSize();
		void SetWindowSize(glm::vec2 size);
		void SetWindowConstraints(glm::vec2 minSize, glm::vec2 maxSize); // vec2(0,0) == no constraint

		// GLFW 
		static void PollEvents();
		bool ShouldClose();
		void SetContext();
		void UpdateViewport();
		void ClearBuffer();
		void SwapBuffers();
		GLFWwindow* GetWindowInstance();

		// Control
		void Minimise();
		void Maximise();
		void Close();


		// Native
		void StartDrag();
		void ToggleWindowsStyle();

	private:
		GLFWwindow* m_Window = nullptr;

		// Native
		HWND m_WindowHandle = nullptr;
		WNDPROC m_OriginalWndProc = nullptr;

		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};

}