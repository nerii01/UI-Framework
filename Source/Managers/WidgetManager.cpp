
#include "WidgetManager.h"
#include "../Engine/Window.h"   
#include "../Engine/Time.h"   
#include "../Widgets/Widget.h"
#include "InputManager.h"


#include "../Widgets/Native/WindowTitlebar.h"



#undef CreateWindow;

namespace UI {

    WindowContext::WindowContext() = default;
    WindowContext::~WindowContext() = default;
    WindowContext::WindowContext(WindowContext&&) noexcept = default;
    WindowContext& WindowContext::operator=(WindowContext&&) noexcept = default;

    Window* WidgetManager::CreateWindow(const std::string& label, glm::vec2 size, bool iconVisible) {
        auto window = std::make_unique<Window>();
        window->Init(label, size.x, size.y, m_FirstWindow ? m_FirstWindow->GetWindowInstance() : nullptr);
        if (m_Windows.empty()) m_FirstWindow = window.get();

        Window* windowPtr = window.get();

        auto titlebarWidget = std::make_shared<WindowTitlebar>(windowPtr);
        titlebarWidget->SetTitle(label);
        titlebarWidget->SetIconVisible(iconVisible);
        GLFWwindow* windowInstance = window->GetWindowInstance();

        // Context
        auto context = std::make_unique<WindowContext>();

        // Window / TitleBar
        context->WindowInstance = std::move(window);
        context->TitlebarWidget = std::move(titlebarWidget);
        
        // Input
        context->Input = std::make_unique<InputManager>();
        context->Input->SetRootWidgets({ context->TitlebarWidget.get()});
        
        // Callbacks
        glfwSetWindowUserPointer(windowInstance, context.get());
        glfwSetCursorPosCallback(windowInstance, InputManager::MousePosCallback);
        glfwSetMouseButtonCallback(windowInstance, InputManager::MouseButtonCallback);
        glfwSetKeyCallback(windowInstance, InputManager::KeyboardCallback);
        glfwSetDropCallback(windowInstance, InputManager::FileDropCallback);
        glfwSetScrollCallback(windowInstance, InputManager::MouseScrollCallback);

        // Resize callback
        glfwSetFramebufferSizeCallback(windowInstance, [](GLFWwindow* glfwWin, int width, int height) {
            auto* context = static_cast<WindowContext*>(glfwGetWindowUserPointer(glfwWin));
            if (context) WidgetManager::Get().Render(context->WindowInstance.get());
        });

        m_Windows[windowPtr] = std::move(context);
        return windowPtr;
    }

    Window* WidgetManager::GetWindow(Widget* widget) {
        if (widget == nullptr) return nullptr;
        Widget* root = widget->GetRoot();

        for (const auto& [window, context] : m_Windows) {
            if (context->RootWidget.get() == root) {
                return window;
            }
        }

        return nullptr;
    }

    WindowContext* WidgetManager::GetWindowContext(Window* window) {
        auto it = m_Windows.find(window);
        if (it != m_Windows.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    void WidgetManager::SetRootWidget(Window* window, std::shared_ptr<Widget> root) {
        if (auto* ctx = GetWindowContext(window)) {
            ctx->Input->SetRootWidgets({ ctx->TitlebarWidget.get(), root.get()});
            ctx->RootWidget = std::move(root);
        }
    }

    void WidgetManager::Render() {
        Time::Update();
        Window::PollEvents();
        Renderer::Get().ShaderUpdateCheck();

        if (glfwWindowShouldClose(m_FirstWindow->GetWindowInstance())) {
            m_IsRunning = false;
            return;
        }

        // Czyszczenie konsoli przed każdą klatką (Sekwencja ANSI)
        std::cout << "\033[H\033[2J" << std::flush;

        unsigned i = 0;
        for (auto it = m_Windows.begin(); it != m_Windows.end(); ) {
            auto& [window, context] = *it;

            if (window->ShouldClose()) {
                it = m_Windows.erase(it);
                continue;
            }

            // --- PRZED RENDEROWANIEM OKNA ---
            // 1. Zapisujemy jaki kontekst był ustawiony przed wywołaniem Render(window)
            GLFWwindow* rawGlfwWin = window->GetWindowInstance();
            GLFWwindow* currentCtxBefore = glfwGetCurrentContext();

            // Wywołujemy renderowanie okna (tam powinno nastąpić window->SetContext())
            Render(window);

            // --- PO RENDEROWANIU OKNA (ODCZYT STANU OPENGL & GLFW) ---
            GLFWwindow* currentCtxAfter = glfwGetCurrentContext();

            // Pobieramy stany z OpenGL
            GLint activeProgram = 0;
            GLint activeVAO = 0;
            GLint viewport[4] = { 0, 0, 0, 0 };

            glGetIntegerv(GL_CURRENT_PROGRAM, &activeProgram);
            glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &activeVAO);
            glGetIntegerv(GL_VIEWPORT, viewport);

            // Sprawdzamy czy OpenGL rzucił jakimś błędem podczas renderowania tego okna
            GLenum err = glGetError();
            std::string errStr = (err == GL_NO_ERROR) ? "OK" : "GL_ERROR: " + std::to_string(err);

            // Flagi GLFW
            bool isCurrent = (currentCtxAfter == rawGlfwWin);
            bool isVisible = glfwGetWindowAttrib(rawGlfwWin, GLFW_VISIBLE);
            bool isFocused = glfwGetWindowAttrib(rawGlfwWin, GLFW_FOCUSED);

            // LOGOWANIE DANYCH
            //Logger::Get().PrintToConsole(
            //    "| Window #{} [{}] | CtxValid: {} | Vis: {} | Shader ID: {} | VAO ID: {} | Viewport: {}x{} | Error: {}",
            //    LogInfo,
            //    i,
            //    context->TitlebarWidget ? context->TitlebarWidget->GetTitle() : "NoTitle",
            //    (isCurrent ? "YES" : "NO! (STOLEN)"),
            //    (isVisible ? "YES" : "NO"),
            //    activeProgram,
            //    activeVAO,
            //    viewport[2], viewport[3],
            //    errStr
            //);

            ++it;
            i++;
        }
    }

    void WidgetManager::Render(Window* window) {
        if (window == nullptr) return;
        auto it = m_Windows.find(window);
        if (it == m_Windows.end()) return;
        auto& [targetWindow, context] = *it;

        float deltaTime = Time::GetDeltaTime();
        window->SetContext();
        window->UpdateViewport();
        window->ClearBuffer();
        glm::ivec2 windowSize = window->GetWindowSize();

        // Shader setup
        Renderer::Get().BeginFrame(windowSize.x, windowSize.y);

        // Widgets
        auto root = context->RootWidget.get();
        auto titlebar = context->TitlebarWidget.get();

        // Titlebar layout
        if (titlebar != nullptr) {
            titlebar->Measure(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));
            titlebar->Layout(0, 0);
        }

        // Root layout
        float titlebarHeight = titlebar == nullptr ? 0 : titlebar->GetSize().y - 2;
        if (root != nullptr) {
            
            root->Measure(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y) - titlebarHeight);
            root->Layout(0, titlebarHeight);
        }

        context->Input->HoverHandling();

        // Root Render
        if (root) {
            root->Update(deltaTime);
            root->Render();
        }

        // Titlebar Render
        if (titlebar) {
            titlebar->Update(deltaTime);
            titlebar->Render();
        }

        // Custom Rendering
        glm::vec2 textSize = FontManager::Get().MeasureText(std::to_string(int(Time::GetFPS())), "InterRegular", 20, 200, WrapMode::None);
        Renderer::Get().DrawText("InterRegular", std::to_string(int(Time::GetFPS())), glm::vec2(windowSize.x - textSize.x - 5, 5 + titlebarHeight), glm::vec4(1, 1, 1, 0.4f), 20);

        // Swap buffers
        window->SwapBuffers();
    }

    bool WidgetManager::IsRunning() const {
        return m_IsRunning;
    }
}