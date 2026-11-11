#pragma once

#include <map>
#include <memory>
#include <string>
#include <glm/glm.hpp>

#undef CreateWindow

namespace UI {

    class Window;
    class WindowTitlebar;
    class Widget;
    class InputManager;

    struct WindowContext {
        std::unique_ptr<Window> WindowInstance;
        std::unique_ptr<InputManager> Input;

        std::shared_ptr<Widget> RootWidget;
        std::shared_ptr<WindowTitlebar> TitlebarWidget;

        WindowContext();
        ~WindowContext();
        WindowContext(WindowContext&&) noexcept;
        WindowContext& operator=(WindowContext&&) noexcept;
    };

    class WidgetManager {
    public:
        static WidgetManager& Get() {
            static WidgetManager instance;
            return instance;
        }

        ~WidgetManager() = default;

        Window* CreateWindow(const std::string& label, glm::vec2 size = glm::vec2(1000,800), bool iconVisible = true);
        Window* GetWindow(Widget* widget);
        WindowContext* GetWindowContext(Window* window);

        void SetRootWidget(Window* window, std::shared_ptr<Widget> root);
        void SetWindowTitle(Window* window, const std::string& title);
        //void SetWindowConstraints(gl)

        void Render();
        void Render(Window* window);
        bool IsRunning() const;

    private:
        std::map<Window*, std::unique_ptr<WindowContext>> m_Windows;
        Window* m_FirstWindow = nullptr;
        bool m_IsRunning = true;
        WidgetManager() = default;
    };
}