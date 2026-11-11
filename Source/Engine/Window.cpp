#include "Window.h"

#define GLFW_EXPOSE_NATIVE_WIN32

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

#ifdef DrawText
#undef DrawText
#endif

#include "Logger.h"
#include "../Managers/InputManager.h"
#include "../Managers/WidgetManager.h"
#include "Time.h"

namespace UI {

		Window::~Window() {
			if (m_Window) {
				glfwDestroyWindow(m_Window);
				m_Window = nullptr;
			}
		}

		bool Window::Init(const std::string& title, int width, int height, GLFWwindow* share) {
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, share);
			m_WindowHandle = glfwGetWin32Window(m_Window);
			ToggleWindowsStyle();

			if (m_Window == nullptr) {
				UI::Logger::Get().PrintToConsole("Failed to create GLFW m_Window", UI::LogType::Error);
				glfwTerminate();
				return false;
			}

			glfwMakeContextCurrent(m_Window);

			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
				UI::Logger::Get().PrintToConsole("Failed to initialize GLAD", UI::LogType::Error);
				return false;
			}

			glViewport(0, 0, width, height);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDisable(GL_DEPTH_TEST);

			glfwSwapInterval(0);

			return true;
		}

		void Window::SetWindowTitle(const std::string& title) {
			
			glfwSetWindowTitle(m_Window, title.c_str());
		}

		bool Window::ShouldClose() { return glfwWindowShouldClose(m_Window); };

		glm::vec<2, int>  Window::GetWindowSize() {
			int w, h;
			glfwGetWindowSize(m_Window, &w, &h);
			return { w, h };
		}

		void Window::PollEvents() {
			glfwPollEvents();
		}

		void  Window::SetContext() {
			glfwMakeContextCurrent(m_Window);
		}

		void  Window::ClearBuffer() {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void  Window::UpdateViewport() {
			glm::vec2 windowSize = GetWindowSize();
			glViewport(0, 0, (GLsizei)windowSize.x, (GLsizei)windowSize.y);
		}

		void  Window::SwapBuffers() {
			glfwSwapBuffers(m_Window);
		}

		void Window::Minimise() {
			glfwIconifyWindow(m_Window);
		}

		void Window::Maximise() {
			if (glfwGetWindowAttrib(m_Window, GLFW_MAXIMIZED)) {
				glfwRestoreWindow(m_Window);
			}
			else {
				glfwMaximizeWindow(m_Window);
			}
		}

		void Window::Close() {
			glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
		}

		void Window::StartDrag() {
			HWND hwnd = glfwGetWin32Window(m_Window);
			if (!hwnd) return;

			static float lastClickTime = 0;
			float currentClickTime = Time::GetTime();

			if (currentClickTime - lastClickTime < 0.2) {
				lastClickTime = 0.0;

				if (IsZoomed(hwnd)) {
					ShowWindow(hwnd, SW_RESTORE);
				}
				else {
					ShowWindow(hwnd, SW_MAXIMIZE);
				}
				return;
			}
			lastClickTime = currentClickTime;

			ReleaseCapture();
			SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
		}

		GLFWwindow* Window::GetWindowInstance() { return m_Window; }
		
		void Window::ToggleWindowsStyle() {
			HWND hwnd = glfwGetWin32Window(m_Window);
			if (!hwnd) return;

			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

			m_OriginalWndProc = reinterpret_cast<WNDPROC>(
				SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::WindowProc))
				);

			LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);
			style |= WS_THICKFRAME | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
			SetWindowLongPtr(hwnd, GWL_STYLE, style);

			MARGINS margins = { 1, 1, 1, 1 };
			DwmExtendFrameIntoClientArea(hwnd, &margins);

			DWM_WINDOW_CORNER_PREFERENCE preference = DWMWCP_ROUND;
			DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &preference, sizeof(preference));

			SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
				SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
		}

		LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
			Window* self = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

			switch (uMsg) {
			case WM_NCCALCSIZE: {
				if (wParam == TRUE) {
					if (IsZoomed(hwnd)) {
						HMONITOR hMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
						MONITORINFO monitorInfo = { sizeof(MONITORINFO) };

						if (GetMonitorInfo(hMonitor, &monitorInfo)) {
							NCCALCSIZE_PARAMS* params = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
							params->rgrc[0] = monitorInfo.rcWork;
						}
					}
					return 0;
				}
				break;
			}
			case WM_NCHITTEST: {
				if (!self) break;

				if (IsZoomed(hwnd)) return HTCLIENT;

				int x = static_cast<short>(LOWORD(lParam));
				int y = static_cast<short>(HIWORD(lParam));

				RECT rect;
				GetWindowRect(hwnd, &rect);

				const int border = 8;

				bool left = x >= rect.left && x < rect.left + border;
				bool right = x < rect.right && x >= rect.right - border;
				bool top = y >= rect.top && y < rect.top + border;
				bool bottom = y < rect.bottom && y >= rect.bottom - border;

				if (top && left)     return HTTOPLEFT;
				if (top && right)    return HTTOPRIGHT;
				if (bottom && left)  return HTBOTTOMLEFT;
				if (bottom && right) return HTBOTTOMRIGHT;
				if (top)             return HTTOP;
				if (bottom)          return HTBOTTOM;
				if (left)            return HTLEFT;
				if (right)           return HTRIGHT;

				return HTCLIENT; 
			}
			}

			if (self && self->m_OriginalWndProc) {
				return CallWindowProc(self->m_OriginalWndProc, hwnd, uMsg, wParam, lParam);
			}
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}

}