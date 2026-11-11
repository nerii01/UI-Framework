#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifdef DrawText
#undef DrawText
#endif

#define GLFW_EXPOSE_NATIVE_WIN32
#include <dwmapi.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>    


// STD
#include <unordered_map>
#include <bitset>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "stb_truetype.h"
#include <set>
#include <fstream>
#include <memory>
#include <chrono>
#include <algorithm>
#include <ranges>
#include <optional>
#include <functional>
#include <filesystem>
#include <execution>

// Source
#include "Thirdparty/FontKit/FontKit.h"

#include "Source/Engine/Logger.h"
#include "Source/Engine/Time.h"
#include "Source/Engine/Window.h"
#include "Source/Managers/WidgetManager.h"
#include "Source/Managers/InputManager.h"

// Widgets
#include "Source/Widgets/Widget.h"
#include "Source/Widgets/Common/Style.h"
#include "Source/Widgets/Common/Color.h"

// Layout 
#include "Source/Widgets/Layout/Border.h"
#include "Source/Widgets/Layout/HStack.h"
#include "Source/Widgets/Layout/VStack.h"
#include "Source/Widgets/Layout/ScrollBox.h"

// Content
#include "Source/Widgets/Content/Text.h"
#include "Source/Widgets/Content/Image.h"

// ImageUTILS
#include "Shared/Utils/ImageUtils.h"

// Colors
namespace UI {
    // --- NEUTRALS & UTILITY ---
    const glm::vec4 transparent(0.0f, 0.0f, 0.0f, 0.0f);
    const glm::vec4 white(1.0f, 1.0f, 1.0f, 1.0f);
    const glm::vec4 black(0.0f, 0.0f, 0.0f, 1.0f);

    // --- GRAY ---
    const glm::vec4 grayLight(0.75f, 0.75f, 0.78f, 1.0f);
    const glm::vec4 grayRegular(0.45f, 0.45f, 0.50f, 1.0f);
    const glm::vec4 grayDark(0.20f, 0.20f, 0.25f, 1.0f);
    const glm::vec4 grayVeryDark(0.08f, 0.08f, 0.10f, 1.0f);

    // --- BROWN ---
    const glm::vec4 brownLight(0.72f, 0.55f, 0.42f, 1.0f);
    const glm::vec4 brownRegular(0.52f, 0.37f, 0.26f, 1.0f);
    const glm::vec4 brownDark(0.32f, 0.21f, 0.14f, 1.0f);

    // --- RED ---
    const glm::vec4 redLight(0.98f, 0.45f, 0.45f, 1.0f);
    const glm::vec4 redRegular(0.88f, 0.25f, 0.25f, 1.0f);
    const glm::vec4 redDark(0.60f, 0.12f, 0.12f, 1.0f);

    // --- GREEN ---
    const glm::vec4 greenLight(0.45f, 0.88f, 0.55f, 1.0f);
    const glm::vec4 greenRegular(0.22f, 0.70f, 0.35f, 1.0f);
    const glm::vec4 greenDark(0.12f, 0.48f, 0.22f, 1.0f);

    // --- BLUE ---
    const glm::vec4 blueLight(0.45f, 0.70f, 0.98f, 1.0f);
    const glm::vec4 blueRegular(0.22f, 0.50f, 0.90f, 1.0f);
    const glm::vec4 blueDark(0.12f, 0.30f, 0.65f, 1.0f);

    // --- YELLOW ---
    const glm::vec4 yellowLight(0.98f, 0.88f, 0.45f, 1.0f);
    const glm::vec4 yellowRegular(0.92f, 0.75f, 0.20f, 1.0f);
    const glm::vec4 yellowDark(0.70f, 0.52f, 0.08f, 1.0f);

    // --- ORANGE ---
    const glm::vec4 orangeLight(0.98f, 0.65f, 0.35f, 1.0f);
    const glm::vec4 orangeRegular(0.90f, 0.48f, 0.15f, 1.0f);
    const glm::vec4 orangeDark(0.68f, 0.30f, 0.05f, 1.0f);

    // --- PURPLE ---
    const glm::vec4 purpleLight(0.75f, 0.55f, 0.98f, 1.0f);
    const glm::vec4 purpleRegular(0.58f, 0.32f, 0.85f, 1.0f);
    const glm::vec4 purpleDark(0.38f, 0.18f, 0.60f, 1.0f);

    // --- PINK ---
    const glm::vec4 pinkLight(0.98f, 0.60f, 0.75f, 1.0f);
    const glm::vec4 pinkRegular(0.88f, 0.32f, 0.55f, 1.0f);
    const glm::vec4 pinkDark(0.62f, 0.18f, 0.38f, 1.0f);

    // --- CYAN ---
    const glm::vec4 cyanLight(0.45f, 0.90f, 0.95f, 1.0f);
    const glm::vec4 cyanRegular(0.18f, 0.72f, 0.80f, 1.0f);
    const glm::vec4 cyanDark(0.08f, 0.48f, 0.55f, 1.0f);

    const glm::vec4 colBgRoot(0.035f, 0.035f, 0.05f, 1.0f);
    const glm::vec4 colSidebarBg(0.06f, 0.055f, 0.08f, 1.0f);
    const glm::vec4 colContentBg(0.045f, 0.045f, 0.06f, 1.0f);
    const glm::vec4 colBorder(1.0f, 1.0f, 1.0f, 0.07f);
    const glm::vec4 colAccent(0.60f, 0.45f, 0.98f, 1.0f);
    const glm::vec4 colAccentDim(0.60f, 0.45f, 0.98f, 0.15f);
    const glm::vec4 colTextMain(0.92f, 0.92f, 0.96f, 1.0f);
    const glm::vec4 colTextDim(0.55f, 0.55f, 0.62f, 1.0f);
    const glm::vec4 colTextFaint(0.38f, 0.38f, 0.45f, 1.0f);
    const glm::vec4 colGreen(0.35f, 0.85f, 0.55f, 1.0f);
    const glm::vec4 colYellow(0.95f, 0.75f, 0.30f, 1.0f);
    const glm::vec4 colRed(0.95f, 0.35f, 0.40f, 1.0f);
    const glm::vec4 colTransparent(0.0f, 0.0f, 0.0f, 0.0f);

    auto makeText = [=](const std::string& str, unsigned int size, glm::vec4 color, SizeMode wMode = SizeMode::Auto, SizeMode yMode = SizeMode::Auto, WrapMode wrap = WrapMode::Word) {
        auto t = std::make_shared<Text>();
        t->SetFontSize(std::max(20u, size));
        t->SetText(str);
        t->SetSizing(wMode, yMode);
        t->SetWrapMode(wrap);
        t->SetStyle(State::Default, { .BackgroundColor = colTransparent, .Color = color });
        t->SetStyle(State::Hovered, { .BackgroundColor = colTransparent, .Color = colTextDim });
        return t;
        };
}

// Warp - AI DEMO ( from source code )
std::shared_ptr<UI::Border> CreateWarpUI()
{
    using namespace UI;


    // ================= PALETA =================
    // NOWE KOLORY - GDY BEDZIESZ TERAZ COS ROBIL KORZYSTAJ Z TEGO ZAMIAST colBg itd itp
    // Color::Red(opacity, brightness) - Wysle ci liste kolorow 
    

    // Helper: Text z wyłączonym tłem, wymuszonym fontem i minimalnym rozmiarem 24
  

    // ================= ROOT =================
    auto root = std::make_shared<Border>();
    root->SetSizing(SizeMode::Fill, SizeMode::Fill);
    root->SetStyle(State::Default, { .BackgroundColor = colBgRoot, .Padding = Edges(0.0f)});

    auto mainRow = std::make_shared<HStack>();
    mainRow->SetSizing(SizeMode::Fill, SizeMode::Fill);
    mainRow->SetSpacing(8.0f);

    // ================= SIDEBAR =================
    auto sidebar = std::make_shared<Border>();
    sidebar->SetSizing(SizeMode::Fixed, SizeMode::Fill);
    sidebar->SetFixedSize(320.0f, 0.0f);
    sidebar->SetStyle(State::Default, {
        .BackgroundColor = colSidebarBg,
        .Border = BorderData(1.0f, colBorder, 0.0f)
        });

    auto sidebarStack = std::make_shared<VStack>();
    sidebarStack->SetSizing(SizeMode::Fill, SizeMode::Fill);
    sidebarStack->SetSpacing(10.0f);
    sidebarStack->SetStyle(State::Default, { .Padding = Edges(20.0f) });

    // --- Logo ---
    auto logoWrap = std::make_shared<Border>();
    logoWrap->SetSizing(SizeMode::Fill, SizeMode::Auto);
    logoWrap->SetStyle(State::Default, { .Padding = Edges(0.0f, 16.0f) });
    logoWrap->SetChild(makeText("Coffee", 46u, colTextMain));
    sidebarStack->AddChild(logoWrap);

    // --- Przycisk "Nowa sesja" ---
    auto newTabBtn = std::make_shared<Border>();
    newTabBtn->SetSizing(SizeMode::Fill, SizeMode::Auto);
    newTabBtn->SetAlignment(AlignMode::Center, AlignMode::Center);
    newTabBtn->SetStyle(State::Default, {
        .BackgroundColor = colAccentDim,
        .Border = BorderData(1.0f, glm::vec4(colAccent.r, colAccent.g, colAccent.b, 0.35f), 10.0f),
        .Padding = Edges(18.0f, 14.0f)
        });
    newTabBtn->SetStyle(State::Hovered, {
        .BackgroundColor = glm::vec4(colAccent.r, colAccent.g, colAccent.b, 0.28f),
        .Border = BorderData(1.0f, colAccent, 10.0f)
        });
    newTabBtn->SetStyle(State::Pressed, {
        .BackgroundColor = glm::vec4(colAccent.r, colAccent.g, colAccent.b, 0.4f),
        .Border = BorderData(1.0f, colAccent, 10.0f)
        });
    newTabBtn->SetChild(makeText("+  New Session", 24, colAccent));
    sidebarStack->AddChild(newTabBtn);

    // --- Nagłówek sekcji ---
    auto sectionLabelWrap = std::make_shared<Border>();
    sectionLabelWrap->SetSizing(SizeMode::Fill, SizeMode::Auto);
    sectionLabelWrap->SetStyle(State::Default, { .Padding = Edges(6.0f, 14.0f) });
    sectionLabelWrap->SetChild(makeText("SESSIONS", 24, colTextFaint));
    sidebarStack->AddChild(sectionLabelWrap);

    // --- Lista sesji (klikalne, przełączają "aktywną") ---
    std::vector<std::shared_ptr<Border>> sessionTabs;

    auto makeSessionTab = [&](const std::string& title, const std::string& path, glm::vec4 dotColor, bool active) {
        auto tab = std::make_shared<Border>();
        tab->SetSizing(SizeMode::Fill, SizeMode::Auto);
        tab->SetStyle(State::Default, {
            .BackgroundColor = active ? colAccentDim : colTransparent,
            .Border = BorderData(active ? 1.0f : 0.0f,
                                  active ? glm::vec4(colAccent.r, colAccent.g, colAccent.b, 0.4f) : colTransparent,
                                  10.0f),
            .Padding = Edges(16.0f, 14.0f)
            });
        tab->SetStyle(State::Hovered, { .BackgroundColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.05f) });
        tab->SetStyle(State::Pressed, { .BackgroundColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.08f) });

        auto row = std::make_shared<HStack>();
        row->SetSizing(SizeMode::Fill, SizeMode::Auto);
        row->SetSpacing(12.0f);
        row->SetAlignment(AlignMode::Start, AlignMode::Center);
        row->SetHitTestable(false);

        auto dot = std::make_shared<Border>();
        dot->SetSizing(SizeMode::Fixed, SizeMode::Fixed);
        dot->SetFixedSize(8.0f, 8.0f);
        dot->SetAlignment(AlignMode::Center, AlignMode::Center);
        dot->SetStyle(State::Default, { .BackgroundColor = dotColor, .Border = BorderData(0.0f, glm::vec4(0.0f), 4.0f) });
        dot->SetHitTestable(false);

        auto textCol = std::make_shared<VStack>();
        textCol->SetSizing(SizeMode::Fill, SizeMode::Auto);
        textCol->SetSpacing(4.0f);
        textCol->AddChild(makeText(title, 24, colTextMain, SizeMode::Fill));
        textCol->AddChild(makeText(path, 24, colTextDim, SizeMode::Fill));

        row->AddChild(dot);
        row->AddChild(textCol);
        tab->SetChild(row);

        return tab;
        };

    sessionTabs.push_back(makeSessionTab("ui-framework", "~/dev/cpp/UserInterface", colGreen, true));
    sessionTabs.push_back(makeSessionTab("api-server", "~/dev/go/api-server", colTextFaint, false));
    sessionTabs.push_back(makeSessionTab("dotfiles", "~/dotfiles", colTextFaint, false));
    sessionTabs.push_back(makeSessionTab("prod-01", "ssh root@10.0.0.14", colYellow, false));

    for (auto& tab : sessionTabs) sidebarStack->AddChild(tab);

    // Klik na zakładkę -> ona staje się aktywna, reszta wraca do stanu domyślnego
    for (auto& tabPtr : sessionTabs) {
        tabPtr->SetCallback(Callback::OnPressStart, [=](Event) {
            for (auto& t : sessionTabs) {
                bool active = (t == tabPtr);
                t->SetStyle(State::Default, {
                    .BackgroundColor = active ? colAccentDim : colTransparent,
                    .Border = BorderData(active ? 1.0f : 0.0f,
                                          active ? glm::vec4(colAccent.r, colAccent.g, colAccent.b, 0.4f) : colTransparent,
                                          10.0f)
                    });
            }
            });
    }

    // --- Rozpychacz wypychający "Settings" na dół ---
    auto sideSpacer = std::make_shared<Border>();
    sideSpacer->SetSizing(SizeMode::Fill, SizeMode::Fill);
    sideSpacer->SetHitTestable(false);
    sidebarStack->AddChild(sideSpacer);

    sidebarStack->AddChild(makeSessionTab("Settings", "change your settings", colTextFaint, false));

    sidebar->SetChild(sidebarStack);

    // ================= CONTENT (Terminal) =================

    auto content = std::make_shared<Border>();
    content->SetSizing(SizeMode::Fill, SizeMode::Fill);
    content->SetStyle(State::Default, { .BackgroundColor = colContentBg, .Border = BorderData(1.0f, glm::vec4(1,1,1,0.0f), 16.0f), .Padding = Edges(12.0f) });


    auto contentStack = std::make_shared<VStack>();
    contentStack->SetStyle(State::Default, { .BackgroundColor = colTransparent });
    contentStack->SetSizing(SizeMode::Fill, SizeMode::Fill);
    contentStack->SetSpacing(8.0f);

    // --- Górny pasek: ścieżka + shell ---
    auto topBar = std::make_shared<HStack>();
    topBar->SetSizing(SizeMode::Fill, SizeMode::Auto);
    topBar->SetAlignment(AlignMode::Start, AlignMode::Center);
    topBar->AddChild(makeText("~/dev/cpp/UserInterface", 28, colAccent, SizeMode::Fill));

    auto shellTag = std::make_shared<Border>();
    shellTag->SetSizing(SizeMode::Auto, SizeMode::Auto);
    shellTag->SetHitTestable(false);
    shellTag->SetStyle(State::Default, {
        .BackgroundColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.05f),
        .Border = BorderData(1.0f, colBorder, 8.0f),
        .Padding = Edges(14.0f, 6.0f)
        });
    shellTag->SetChild(makeText("zsh", 24, colTextDim));
    topBar->AddChild(shellTag);

    contentStack->AddChild(topBar);

    // --- Separator ---
    auto divider = std::make_shared<Border>();
    divider->SetSizing(SizeMode::Fill, SizeMode::Fixed);
    divider->SetFixedSize(0.0f, 1.0f);
    divider->SetStyle(State::Default, { .BackgroundColor = colBorder });
    contentStack->AddChild(divider);

    // --- Historia terminala ---
    auto history = std::make_shared<VStack>();
    history->SetSizing(SizeMode::Fill, SizeMode::Auto);
    history->SetSpacing(8.0f);

    history->AddChild(makeText("->  ui-framework git:(main) cmake --build build --target UI", 26, colGreen, SizeMode::Fill));
    history->AddChild(makeText("[100%] Built target UI  •  finished in 3.4s", 24, colTextDim, SizeMode::Fill));
    history->AddChild(makeText("->  ui-framework git:(main) ./build/UI.exe --debug", 26, colGreen, SizeMode::Fill));
    history->AddChild(makeText("[Renderer] GLSL shaders hot-reloaded (widget.frag)", 24, colTextDim, SizeMode::Fill));
    history->AddChild(makeText("[FontKit]  atlas 4096x4096 baked in 218ms", 24, colTextDim, SizeMode::Fill));
    history->AddChild(makeText("Type 'help' to see available commands.", 24, colTextFaint, SizeMode::Fill));
    


    auto ScrollVieew = std::make_shared<ScrollBox>();
    ScrollVieew->SetChild(history);
    ScrollVieew->SetStyle(State::Default, { .BackgroundColor = colTransparent, .Border = BorderData(1.0f, glm::vec4(1,1,1,0.0f), 12.0f), .Padding = Edges(0.0f)});
    ScrollVieew->SetSizing(SizeMode::Fill, SizeMode::Fill);

    contentStack->AddChild(ScrollVieew);

    // --- Rozpychacz przed paskiem poleceń ---
    //auto fillSpacer = std::make_shared<Border>();
    //fillSpacer->SetSizing(SizeMode::Fill, SizeMode::Fill);
    //fillSpacer->SetHitTestable(false);
    //contentStack->AddChild(fillSpacer);

    // --- Pasek wpisywania poleceń ---
    auto inputBar = std::make_shared<Border>();
    inputBar->SetSizing(SizeMode::Fill, SizeMode::Fixed);
    inputBar->SetFixedSize(0.0f, 50.0f);
    inputBar->SetAlignment(AlignMode::Start, AlignMode::Center);
    inputBar->SetStyle(State::Default, {
        .BackgroundColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.04f),
        .Border = BorderData(1.0f, colBorder, 12.0f),
        .Padding = Edges(20.0f, 0.0f)
        });
    inputBar->SetStyle(State::Hovered, {
        .Border = BorderData(1.0f, glm::vec4(colAccent.r, colAccent.g, colAccent.b, 0.5f), 12.0f)
        });
    inputBar->SetStyle(State::Focused, {
        .Border = BorderData(1.5f, colAccent, 12.0f)
        });

    auto inputRow = std::make_shared<HStack>();
    inputRow->SetSizing(SizeMode::Fill, SizeMode::Fill);
    inputRow->SetSpacing(14.0f);
  
    auto prompt = makeText(">_", 28, colAccent);

    auto inputDisplay = makeText("Enter command", 26, colTextFaint, SizeMode::Fill);

    auto hintTag = std::make_shared<Border>();
    hintTag->SetSizing(SizeMode::Auto, SizeMode::Auto);
    hintTag->SetStyle(State::Default, {
        .BackgroundColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.06f),
        .Border = BorderData(1.0f, colBorder, 8.0f),
        .Padding = Edges(12.0f, 5.0f)
        });

    hintTag->SetStyle(State::Pressed, {
    .BackgroundColor = glm::vec4(0.2f, 0.6f, 1.0f, 0.09f),
    .Border = BorderData(1.0f, colBorder, 8.0f),
    .Padding = Edges(12.0f, 5.0f)
        });
    hintTag->SetChild(makeText("Enter", 24, colTextDim));

    std::vector<std::shared_ptr<Widget>> widgets = { prompt, inputDisplay, hintTag };
    
    auto commandBuffer = std::make_shared<std::string>();
    auto handleSubmit = [=](Event event, bool press = false) {
        Logger::Get().PrintToConsole("Event {} {}", LogInfo, event.key.value_or(0), (int)event.EventType);
            const int kBackspace = 259;
            const int kEnter = 257;
            int key = event.key.value_or(0);


            if (key == kBackspace) {
                if (!commandBuffer->empty()) commandBuffer->pop_back();
            }
            else if (key == kEnter || key == 0) {
                std::string cmd = *commandBuffer;
                commandBuffer->clear();

                if (!cmd.empty()) {
                    // nie dziala
                    history->AddChild(makeText("->  ui-framework git:(main) " + cmd, 26, colGreen, SizeMode::Fill));

                  
                    if (cmd == "HELP") {
                        history->AddChild(makeText("Available commands:", 24, colTextMain, SizeMode::Fill));
                        history->AddChild(makeText("  | HELP     Show this message", 24, colTextDim, SizeMode::Fill));
                        history->AddChild(makeText("  | CLEAR    Clear text", 24, colTextDim, SizeMode::Fill));
                    }

                    if (cmd == "CLEAR") {
                    }

                    if (cmd == "OPEN") {


                        auto w = WidgetManager::Get().CreateWindow("Opened");
                        //WidgetManager::Get().SetRootWidget(w, CreateTestPanel());
                    }
                    else {
                        history->AddChild(makeText("zsh: command not found: " + cmd, 24, colRed, SizeMode::Fill));
                    }
                }
            }
            else if (key >= 32 && key < 127) {
                commandBuffer->push_back((char)key);
            }

            if (commandBuffer->empty()) {
                inputDisplay->SetText("Type a command...");
                inputDisplay->SetStyle(State::Default, { .Color = colTextFaint });
            }
            else {
                inputDisplay->SetText(*commandBuffer);
                inputDisplay->SetStyle(State::Default, { .Color = colTextMain });
            }

            ScrollVieew->ScrollToBottom();
    };
    
    hintTag->SetCallback(Callback::OnPressStart, [=](Event) {
        handleSubmit(Event{ .key = 257 });
        });

    int i = 0;
    for (const auto& widget : widgets) {
        auto container = std::make_shared<Border>();
        container->SetSizing(i == 1 ? SizeMode::Fill : SizeMode::Auto, SizeMode::Fill);
        container->SetAlignment(AlignMode::Start, AlignMode::Center);
        container->SetStyle(State::Default, { .BackgroundColor = colTransparent });
        container->SetHitTestable(i == 2 ? true : false);
        container->SetChild(widget);
        inputRow->AddChild(container);

        i++;
    }

    inputBar->SetChild(inputRow);

    // --- Logika komend (typing + Enter + help + unknown-command w czerwonym) ---

    inputBar->SetCallback(Callback::OnKeyDown, handleSubmit);

    contentStack->AddChild(inputBar);

  
    content->SetChild(contentStack);

    // ================= SKŁADANIE =================
    mainRow->AddChild(sidebar);
    mainRow->AddChild(content);
    root->SetChild(mainRow);

    return root;
}

// Apple Music - AI DEMO ( from source code )
std::shared_ptr<UI::Border> CreateAppleMusicUI()
{
    using namespace UI;

    // ================= PALETA =================
    const glm::vec4 accent = Color::Accent(0.5f);
    const glm::vec4 accentDim = Color::Accent(0.14f);
    const glm::vec4 accentGlow = Color::Accent(0.35f);
    const glm::vec4 accentBorder = Color::Accent(0.45f);

    const glm::vec4 bgRoot = Color::Background(1.0f, 0.85f);
    const glm::vec4 bgSidebar = Color::Surface(1.0f, 0.75f);
    const glm::vec4 bgContent = Color::Background(1.0f, 0.85f);
    const glm::vec4 bgTopBar = Color::Surface(1.0f, 0.7f);
    const glm::vec4 bgHover = Color::SurfaceAlt(0.6f, 0.85f);
    const glm::vec4 bgPressed = Color::SurfaceAlt(0.85f, 0.7f);
    const glm::vec4 border = Color::Border;
    const glm::vec4 textMain = Color::TextPrimary;
    const glm::vec4 textDim = Color::TextSecondary;
    const glm::vec4 textFaint = Color::TextSecondary(0.65f);

    // Paleta kolorów pod gradientowe "okładki" (zamiast płaskich szarych placeholderów) -
    // każda karta dostaje swój odcień, tak jak w prawdziwym Apple Music.
    static const glm::vec4 kTilePalette[] = {
        Color::Pink,   Color::Purple,     Color::Indigo, Color::Teal,
        Color::Orange, Color::DeepOrange, Color::Cyan,   Color::RoyalBlue,
        Color::Rose,   Color::Emerald,    Color::Amber,  Color::Violet
    };
    auto pickColor = [](size_t i) { return kTilePalette[i % (sizeof(kTilePalette) / sizeof(kTilePalette[0]))]; };

    // ================= HELPERY =================

    struct NavEntry {
        std::shared_ptr<Border> container;
        std::function<void(bool)> setActive;
    };

    // Pozycja nawigacji (klikalna) - zwraca kontener + setActive(bool) do późniejszej podmiany wyglądu.
    auto makeNavItem = [=](const std::string& icon, const std::string& label) -> NavEntry {
        auto item = std::make_shared<Border>();
        item->SetSizing(SizeMode::Fill, SizeMode::Auto);
        item->SetStyle(State::Hovered, { .BackgroundColor = bgHover });
        item->SetStyle(State::Pressed, { .BackgroundColor = bgPressed });

        auto row = std::make_shared<HStack>();
        row->SetSizing(SizeMode::Fill, SizeMode::Auto);
        row->SetSpacing(12.0f);
        row->SetAlignment(AlignMode::Start, AlignMode::Center);
        row->SetHitTestable(false);

        auto ic = std::make_shared<Image>();
        ic->SetImage(icon);
        ic->SetSizing(SizeMode::Fixed, SizeMode::Fixed);
        ic->SetFixedSize(20.0f, 20.0f);

        auto lbl = makeText(label, 24, textMain, SizeMode::Fill);

        row->AddChild(ic);
        row->AddChild(lbl);
        item->SetChild(row);

        auto setActive = [=](bool active) {
            item->SetStyle(State::Default, {
                .BackgroundColor = active ? accentDim : Color::Transparent,
                .Border = BorderData(active ? 1.0f : 0.0f, active ? accentBorder : Color::Transparent, 8.0f),
                .Padding = Edges(14.0f, 10.0f),
                // Poświata od lewej krawędzi na aktywnym elemencie - efekt "glow"
                .Gradient = active
                    ? GradientData(accentGlow, glm::vec2(0.0f, 0.5f), 0.05f, 200)
                    : GradientData(glm::vec4(0.0f), glm::vec2(0.0f, 0.5f), 0.05f, 0.0f)
                });
            ic->SetStyle(State::Default, { .BackgroundColor = active ? accent : textDim });
            lbl->SetStyle(State::Default, { .Color = active ? accent : textMain });
            };
        setActive(false);
        return { item, setActive };
        };

    auto makeSectionLabel = [=](const std::string& text) {
        auto wrap = std::make_shared<Border>();
        wrap->SetSizing(SizeMode::Fill, SizeMode::Auto);
        wrap->SetStyle(State::Default, { .Padding = Edges(10.f) });
        wrap->SetChild(makeText(text, 24, textFaint));
        return wrap;
        };

    auto makePlaylistRow = [=](const std::string& name) {
        auto row = std::make_shared<Border>();
        row->SetSizing(SizeMode::Fill, SizeMode::Auto);
        row->SetStyle(State::Default, { .Padding = Edges(14.0f, 10.0f) });
        row->SetStyle(State::Hovered, { .BackgroundColor = bgHover });
        row->SetStyle(State::Pressed, { .BackgroundColor = bgPressed });
        row->SetChild(makeText(name, 24, textDim, SizeMode::Fill));
        return row;
        };

    // Kafelek "okładki" z kolorowym gradientem + ikoną na wierzchu (zamiast płaskiego placeholdera)
    auto makeCoverTile = [=](const glm::vec4& color, const std::string& icon, float size, bool circle) {
        auto tile = std::make_shared<Border>();
        tile->SetSizing(SizeMode::Fixed, SizeMode::Fixed);
        tile->SetFixedSize(size, size);
        tile->SetAlignment(AlignMode::Center, AlignMode::Center);

        glm::vec4 baseDark = glm::vec4(glm::vec3(color) * 0.45f, 1.0f);

        tile->SetStyle(State::Default, {
            .BackgroundColor = baseDark,
            .Border = BorderData(0.0f, Color::Transparent, circle ? size * 0.5f : 14.0f),
            .Padding = Edges(0.0f),
            .Gradient = GradientData(color, glm::vec2(0.28f, 0.2f), 0.1f, 405)
            });

        auto ic = std::make_shared<Image>();
        ic->SetImage(icon);
        ic->SetSizing(SizeMode::Fixed, SizeMode::Fixed);
        ic->SetFixedSize(size * 0.36f, size * 0.36f);
        ic->SetStyle(State::Default, { .BackgroundColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.55f) });
        ic->SetHitTestable(false);

        tile->SetChild(ic);
        return tile;
        };

    // Karta albumu/playlisty - gradientowa okładka + tytuł + podtytuł, z hover/press
    auto makeAlbumCard = [=](const std::string& title, const std::string& subtitle, float size, const glm::vec4& tileColor) {
        auto card = std::make_shared<Border>();
        card->SetSizing(SizeMode::Fixed, SizeMode::Auto);
        card->SetFixedSize(size, 0.0f);
        card->SetStyle(State::Default, {
            .BackgroundColor = Color::Transparent,
            .Border = BorderData(0.0f, Color::Transparent, 12.0f),
            .Padding = Edges(12.0f)
            });
        card->SetStyle(State::Hovered, {
            .BackgroundColor = Color::SurfaceAlt(0.35f, 0.9f),
            .Border = BorderData(1.0f, accentBorder, 12.0f),
            .Padding = Edges(12.0f)
            });
        card->SetStyle(State::Pressed, {
            .BackgroundColor = Color::SurfaceAlt(0.5f, 0.7f),
            .Border = BorderData(1.5f, accent, 12.0f),
            .Padding = Edges(12.0f)
            });

        auto col = std::make_shared<VStack>();
        col->SetSizing(SizeMode::Auto, SizeMode::Auto);
        col->SetSpacing(12.0f);
        col->SetHitTestable(false);

        col->AddChild(makeCoverTile(tileColor, "music-2", size - 24.0f, false));
        col->AddChild(makeText(title, 24, textMain, SizeMode::Auto, SizeMode::Auto, WrapMode::Elipsis));
        col->AddChild(makeText(subtitle, 24, textDim, SizeMode::Auto, SizeMode::Auto, WrapMode::Elipsis));

        card->SetChild(col);
        return card;
        };

    // Okrągła karta artysty - avatar z gradientem + imię
    auto makeArtistCard = [=](const std::string& name, float size, const glm::vec4& tileColor) {
        auto card = std::make_shared<Border>();
        card->SetSizing(SizeMode::Fixed, SizeMode::Auto);
        card->SetFixedSize(size, 0.0f);
        card->SetAlignment(AlignMode::Center, AlignMode::Center);
        card->SetStyle(State::Default, { .BackgroundColor = Color::Transparent, .Padding = Edges(6.0f) });
        card->SetStyle(State::Hovered, { .BackgroundColor = Color::SurfaceAlt(0.3f, 0.9f), .Border = BorderData(0.0f, Color::Transparent, size * 0.5f) });

        auto col = std::make_shared<VStack>();
        col->SetSizing(SizeMode::Fill, SizeMode::Auto);
        col->SetSpacing(10.0f);
        col->SetAlignment(AlignMode::Center, AlignMode::Start);
        col->SetHitTestable(false);

        col->AddChild(makeCoverTile(tileColor, "mic", size, true));
        col->AddChild(makeText(name, 24, textMain, SizeMode::Fill));

        card->SetChild(col);
        return card;
        };

    auto makeContentSectionHeader = [=](const std::string& title) {
        auto row = std::make_shared<HStack>();
        row->SetSizing(SizeMode::Fill, SizeMode::Auto);
        row->SetSpacing(8.0f);
        row->SetAlignment(AlignMode::Start, AlignMode::Center);
        row->AddChild(makeText(title, 30, textMain));
        row->AddChild(makeText(">", 26, accent));
        return row;
        };

    auto makeCardRow = [](std::vector<std::shared_ptr<Widget>> cards, float spacing) {
        auto row = std::make_shared<HStack>();
        row->SetSizing(SizeMode::Fill, SizeMode::Auto);
        row->SetSpacing(spacing);
        for (auto& c : cards) row->AddChild(c);
        return row;
        };

    // Wiersz stacji radiowej - okrągła ikonka z gradientem + tytuł + opis
    auto makeStationRow = [=](const std::string& title, const std::string& subtitle, const glm::vec4& tileColor) {
        auto row = std::make_shared<Border>();
        row->SetSizing(SizeMode::Fill, SizeMode::Auto);
        row->SetStyle(State::Default, {
            .BackgroundColor = Color::SurfaceAlt(0.35f, 0.85f),
            .Border = BorderData(1.0f, border, 10.0f),
            .Padding = Edges(16.0f, 14.0f)
            });
        row->SetStyle(State::Hovered, { .Border = BorderData(1.0f, accentBorder, 10.0f) });
        row->SetStyle(State::Pressed, { .BackgroundColor = Color::SurfaceAlt(0.5f, 0.7f), .Border = BorderData(1.5f, accent, 10.0f)});

        auto inner = std::make_shared<HStack>();
        inner->SetSizing(SizeMode::Fill, SizeMode::Auto);
        inner->SetSpacing(14.0f);
        inner->SetAlignment(AlignMode::Start, AlignMode::Center);
        inner->SetHitTestable(false);

        inner->AddChild(makeCoverTile(tileColor, "radio", 48.0f, true));

        auto col = std::make_shared<VStack>();
        col->SetSizing(SizeMode::Fill, SizeMode::Auto);
        col->SetSpacing(4.0f);
        col->AddChild(makeText(title, 24, textMain, SizeMode::Fill));
        col->AddChild(makeText(subtitle, 24, textDim, SizeMode::Fill));

        inner->AddChild(col);
        row->SetChild(inner);
        return row;
        };

    // Wiersz utworu (widok Songs) - numer / tytuł+artysta / czas trwania
    auto makeSongRow = [=](int index, const std::string& title, const std::string& artist, const std::string& duration) {
        auto row = std::make_shared<Border>();
        row->SetSizing(SizeMode::Fill, SizeMode::Auto);
        row->SetStyle(State::Default, { .Padding = Edges(16.0f, 12.0f) });
        row->SetStyle(State::Hovered, { .BackgroundColor = bgHover, .Border = BorderData(0.0f, Color::Transparent, 8.0f) });
        row->SetStyle(State::Pressed, { .BackgroundColor = bgPressed, .Border = BorderData(0.0f, Color::Transparent, 8.0f) });

        auto inner = std::make_shared<HStack>();
        inner->SetSizing(SizeMode::Fill, SizeMode::Auto);
        inner->SetSpacing(16.0f);
        inner->SetAlignment(AlignMode::Start, AlignMode::Center);
        inner->SetHitTestable(false);

        auto num = std::make_shared<Border>();
        num->SetSizing(SizeMode::Fixed, SizeMode::Auto);
        num->SetFixedSize(28.0f, 0.0f);
        num->SetChild(makeText(std::to_string(index), 24, textFaint));

        auto col = std::make_shared<VStack>();
        col->SetSizing(SizeMode::Fill, SizeMode::Auto);
        col->SetSpacing(4.0f);
        col->AddChild(makeText(title, 24, textMain, SizeMode::Fill));
        col->AddChild(makeText(artist, 24, textDim, SizeMode::Fill));

        auto time = std::make_shared<Border>();
        time->SetSizing(SizeMode::Auto, SizeMode::Auto);
        time->SetChild(makeText(duration, 24, textFaint));

        inner->AddChild(num);
        inner->AddChild(col);
        inner->AddChild(time);
        row->SetChild(inner);
        return row;
        };

    auto makeScrollBody = [](std::shared_ptr<VStack> body) -> std::shared_ptr<Widget> {
        auto scroll = std::make_shared<ScrollBox>();
        scroll->SetChild(body);
        scroll->SetSizing(SizeMode::Fill, SizeMode::Fill);
        scroll->SetStyle(State::Default, { .BackgroundColor = Color::Transparent, .Padding = Edges(0.0f) });
        return scroll;
        };

    const float kCardSize = 160.0f;
    const float kArtistSize = 140.0f;
    const float kCardSpacing = 12.0f;

    // ================= WIDOKI =================

    auto buildHomeView = [=]() -> std::shared_ptr<Widget> {
        auto body = std::make_shared<VStack>();
        body->SetSizing(SizeMode::Fill, SizeMode::Auto);
        body->SetSpacing(40.0f);
        body->SetStyle(State::Default, { .Padding = Edges(0.0f) });

        auto updatedSection = std::make_shared<VStack>();
        updatedSection->SetSizing(SizeMode::Fill, SizeMode::Auto);
        updatedSection->SetSpacing(20.0f);
        updatedSection->AddChild(makeContentSectionHeader("Updated Playlists"));
        updatedSection->AddChild(makeCardRow({
            makeAlbumCard("Breaking Rock", "Apple Music Rock", kCardSize, pickColor(0)),
            makeAlbumCard("Pure Party", "Apple Music Party", kCardSize, pickColor(1)),
            makeAlbumCard("Easy Hits", "Apple Music Pop", kCardSize, pickColor(2)),
            makeAlbumCard("Musicals!", "Film, TV & Stage", kCardSize, pickColor(3)),
            makeAlbumCard("The New Rock", "Apple Music Rock", kCardSize, pickColor(4))
            }, kCardSpacing));
        updatedSection->AddChild(makeCardRow({
            makeAlbumCard("Electronic Pop", "Apple Music Electronic", kCardSize, pickColor(5)),
            makeAlbumCard("Indie Anthems", "Apple Music Indie", kCardSize, pickColor(6)),
            makeAlbumCard("Solid Gold Hits", "Apple Music Pop", kCardSize, pickColor(7)),
            makeAlbumCard("Peak Indie", "Apple Music Indie", kCardSize, pickColor(8)),
            makeAlbumCard("Pure Throwback", "Apple Music Pop", kCardSize, pickColor(9))
            }, kCardSpacing));
        body->AddChild(updatedSection);

        auto workoutSection = std::make_shared<VStack>();
        workoutSection->SetSizing(SizeMode::Fill, SizeMode::Auto);
        workoutSection->SetSpacing(20.0f);
        workoutSection->AddChild(makeContentSectionHeader("Workout"));
        workoutSection->AddChild(makeCardRow({
            makeAlbumCard("Run Motivation", "Apple Music Workout", kCardSize, pickColor(10)),
            makeAlbumCard("Power Walk", "Apple Music Workout", kCardSize, pickColor(11)),
            makeAlbumCard("Rihanna", "Artist Radio", kCardSize, pickColor(0)),
            makeAlbumCard("Miley Cyrus", "Artist Radio", kCardSize, pickColor(1)),
            makeAlbumCard("The Rolling Stones", "Artist Radio", kCardSize, pickColor(2))
            }, kCardSpacing));
        body->AddChild(workoutSection);

        return makeScrollBody(body);
        };

    auto buildBrowseView = [=]() -> std::shared_ptr<Widget> {
        auto body = std::make_shared<VStack>();
        body->SetSizing(SizeMode::Fill, SizeMode::Auto);
        body->SetSpacing(40.0f);
        body->SetStyle(State::Default, { .Padding = Edges(32.0f) });

        auto newSection = std::make_shared<VStack>();
        newSection->SetSizing(SizeMode::Fill, SizeMode::Auto);
        newSection->SetSpacing(20.0f);
        newSection->AddChild(makeContentSectionHeader("New Releases"));
        newSection->AddChild(makeCardRow({
            makeAlbumCard("Fresh Finds", "New This Week", kCardSize, pickColor(3)),
            makeAlbumCard("Top Charts", "Most Played", kCardSize, pickColor(4)),
            makeAlbumCard("Rising Artists", "Discover", kCardSize, pickColor(5)),
            makeAlbumCard("Hip-Hop Now", "Apple Music Hip-Hop", kCardSize, pickColor(6)),
            makeAlbumCard("Jazz Reimagined", "Apple Music Jazz", kCardSize, pickColor(7))
            }, kCardSpacing));
        body->AddChild(newSection);

        auto genreSection = std::make_shared<VStack>();
        genreSection->SetSizing(SizeMode::Fill, SizeMode::Auto);
        genreSection->SetSpacing(20.0f);
        genreSection->AddChild(makeContentSectionHeader("Genres"));
        genreSection->AddChild(makeCardRow({
            makeAlbumCard("Pop", "Genre", kCardSize, pickColor(8)),
            makeAlbumCard("Rock", "Genre", kCardSize, pickColor(9)),
            makeAlbumCard("Electronic", "Genre", kCardSize, pickColor(10)),
            makeAlbumCard("Classical", "Genre", kCardSize, pickColor(11)),
            makeAlbumCard("Latin", "Genre", kCardSize, pickColor(0))
            }, kCardSpacing));
        body->AddChild(genreSection);

        return makeScrollBody(body);
        };

    auto buildRadioView = [=]() -> std::shared_ptr<Widget> {
        auto body = std::make_shared<VStack>();
        body->SetSizing(SizeMode::Fill, SizeMode::Auto);
        body->SetSpacing(28.0f);
        body->SetStyle(State::Default, { .Padding = Edges(32.0f) });

        body->AddChild(makeContentSectionHeader("Stations"));

        auto stationList = std::make_shared<VStack>();
        stationList->SetSizing(SizeMode::Fill, SizeMode::Auto);
        stationList->SetSpacing(12.0f);
        stationList->AddChild(makeStationRow("Today's Hits Radio", "The biggest songs right now", pickColor(0)));
        stationList->AddChild(makeStationRow("Chill Mix", "Relax and unwind", pickColor(1)));
        stationList->AddChild(makeStationRow("Hip-Hop Beats", "Non-stop hip-hop", pickColor(2)));
        stationList->AddChild(makeStationRow("Classical Radio", "Timeless orchestral pieces", pickColor(3)));
        stationList->AddChild(makeStationRow("Indie Discoveries", "Fresh indie tracks daily", pickColor(4)));
        body->AddChild(stationList);

        return makeScrollBody(body);
        };

    auto buildRecentlyAddedView = [=]() -> std::shared_ptr<Widget> {
        auto body = std::make_shared<VStack>();
        body->SetSizing(SizeMode::Fill, SizeMode::Auto);
        body->SetSpacing(40.0f);
        body->SetStyle(State::Default, { .Padding = Edges(32.0f) });

        body->AddChild(makeContentSectionHeader("Recently Added"));
        body->AddChild(makeCardRow({
            makeAlbumCard("Late Night Drive", "Added yesterday", kCardSize, pickColor(6)),
            makeAlbumCard("Coffee Shop Jazz", "Added yesterday", kCardSize, pickColor(7)),
            makeAlbumCard("Focus Flow", "Added 3 days ago", kCardSize, pickColor(8)),
            makeAlbumCard("Sunset Acoustic", "Added 3 days ago", kCardSize, pickColor(9)),
            makeAlbumCard("Weekend Vibes", "Added a week ago", kCardSize, pickColor(10))
            }, kCardSpacing));
        body->AddChild(makeCardRow({
            makeAlbumCard("Deep Focus", "Added a week ago", kCardSize, pickColor(11)),
            makeAlbumCard("Morning Motivation", "Added 2 weeks ago", kCardSize, pickColor(0)),
            makeAlbumCard("Study Session", "Added 2 weeks ago", kCardSize, pickColor(1)),
            makeAlbumCard("Road Trip", "Added 3 weeks ago", kCardSize, pickColor(2)),
            makeAlbumCard("Late Summer", "Added 3 weeks ago", kCardSize, pickColor(3))
            }, kCardSpacing));

        return makeScrollBody(body);
        };

    auto buildArtistsView = [=]() -> std::shared_ptr<Widget> {
        auto body = std::make_shared<VStack>();
        body->SetSizing(SizeMode::Fill, SizeMode::Auto);
        body->SetSpacing(40.0f);
        body->SetStyle(State::Default, { .Padding = Edges(32.0f) });

        body->AddChild(makeContentSectionHeader("Your Artists"));
        body->AddChild(makeCardRow({
            makeArtistCard("Rihanna", kArtistSize, pickColor(0)),
            makeArtistCard("Miley Cyrus", kArtistSize, pickColor(1)),
            makeArtistCard("The Rolling Stones", kArtistSize, pickColor(2)),
            makeArtistCard("Dua Lipa", kArtistSize, pickColor(3)),
            makeArtistCard("Tame Impala", kArtistSize, pickColor(4))
            }, kCardSpacing));
        body->AddChild(makeCardRow({
            makeArtistCard("Arctic Monkeys", kArtistSize, pickColor(5)),
            makeArtistCard("Billie Eilish", kArtistSize, pickColor(6)),
            makeArtistCard("Daft Punk", kArtistSize, pickColor(7)),
            makeArtistCard("Frank Ocean", kArtistSize, pickColor(8)),
            makeArtistCard("HAIM", kArtistSize, pickColor(9))
            }, kCardSpacing));

        return makeScrollBody(body);
        };

    auto buildAlbumsView = [=]() -> std::shared_ptr<Widget> {
        auto body = std::make_shared<VStack>();
        body->SetSizing(SizeMode::Fill, SizeMode::Auto);
        body->SetSpacing(40.0f);
        body->SetStyle(State::Default, { .Padding = Edges(32.0f) });

        body->AddChild(makeContentSectionHeader("Your Albums"));
        body->AddChild(makeCardRow({
            makeAlbumCard("Currents", "Tame Impala", kCardSize, pickColor(2)),
            makeAlbumCard("Future Nostalgia", "Dua Lipa", kCardSize, pickColor(3)),
            makeAlbumCard("Random Access Memories", "Daft Punk", kCardSize, pickColor(4)),
            makeAlbumCard("Blonde", "Frank Ocean", kCardSize, pickColor(5)),
            makeAlbumCard("AM", "Arctic Monkeys", kCardSize, pickColor(6))
            }, kCardSpacing));
        body->AddChild(makeCardRow({
            makeAlbumCard("Happier Than Ever", "Billie Eilish", kCardSize, pickColor(7)),
            makeAlbumCard("Women in Music Pt. III", "HAIM", kCardSize, pickColor(8)),
            makeAlbumCard("Sticky Fingers", "The Rolling Stones", kCardSize, pickColor(9)),
            makeAlbumCard("Plastic Hearts", "Miley Cyrus", kCardSize, pickColor(10)),
            makeAlbumCard("Anti", "Rihanna", kCardSize, pickColor(11))
            }, kCardSpacing));

        return makeScrollBody(body);
        };

    auto buildSongsView = [=]() -> std::shared_ptr<Widget> {
        auto body = std::make_shared<VStack>();
        body->SetSizing(SizeMode::Fill, SizeMode::Auto);
        body->SetSpacing(20.0f);
        body->SetStyle(State::Default, { .Padding = Edges(32.0f) });

        body->AddChild(makeContentSectionHeader("Songs"));

        auto list = std::make_shared<VStack>();
        list->SetSizing(SizeMode::Fill, SizeMode::Auto);
        list->SetSpacing(2.0f);
        list->AddChild(makeSongRow(1, "Levitating", "Dua Lipa", "3:23"));
        list->AddChild(makeSongRow(2, "Blinding Lights", "The Weeknd", "3:20"));
        list->AddChild(makeSongRow(3, "Instant Crush", "Daft Punk", "5:37"));
        list->AddChild(makeSongRow(4, "Nights", "Frank Ocean", "5:07"));
        list->AddChild(makeSongRow(5, "Do I Wanna Know?", "Arctic Monkeys", "4:32"));
        list->AddChild(makeSongRow(6, "Bad Guy", "Billie Eilish", "3:14"));
        list->AddChild(makeSongRow(7, "The Less I Know The Better", "Tame Impala", "3:36"));
        list->AddChild(makeSongRow(8, "Umbrella", "Rihanna", "4:36"));
        body->AddChild(list);

        return makeScrollBody(body);
        };

    // ================= ROOT =================
    auto root = std::make_shared<Border>();
    root->SetSizing(SizeMode::Fill, SizeMode::Fill);
    root->SetStyle(State::Default, {
        .BackgroundColor = bgRoot,
        .Padding = Edges(0.0f),
        // Delikatna poświata w tle całej apki, żeby nie było płasko
        .Gradient = GradientData(Color::Accent(0.05f), glm::vec2(0.5f, 0.0f), 0.05f, 1.4f)
        });

    auto mainRow = std::make_shared<HStack>();
    mainRow->SetSizing(SizeMode::Fill, SizeMode::Fill);
    mainRow->SetSpacing(0.0f);

    // ================= SIDEBAR =================
    auto sidebar = std::make_shared<Border>();
    sidebar->SetSizing(SizeMode::Fixed, SizeMode::Fill);
    sidebar->SetFixedSize(280.0f, 0.0f);
    sidebar->SetStyle(State::Default, { .BackgroundColor = bgSidebar, .Border = BorderData(1.0f, border, 0.0f) });

    auto sidebarStack = std::make_shared<VStack>();
    sidebarStack->SetSizing(SizeMode::Fill, SizeMode::Fill);
    sidebarStack->SetSpacing(10.0f);
    sidebarStack->SetStyle(State::Default, { .Padding = Edges(22.0f) });

    // --- Logo (styl ikony aplikacji Apple Music - czerwono-różowy gradient) ---
    auto logoRow = std::make_shared<HStack>();
    logoRow->SetSizing(SizeMode::Fill, SizeMode::Auto);
    logoRow->SetSpacing(12.0f);
    logoRow->SetAlignment(AlignMode::Start, AlignMode::Center);
    logoRow->SetStyle(State::Default, { .Padding = Edges(10.f) });

    auto logoBox = std::make_shared<Border>();
    logoBox->SetSizing(SizeMode::Fixed, SizeMode::Fixed);
    logoBox->SetFixedSize(36.0f, 36.0f);
    logoBox->SetAlignment(AlignMode::Center, AlignMode::Center);
    logoBox->SetStyle(State::Default, {
        .BackgroundColor = Color::Red(1,0.5f),
        .Border = BorderData(0.0f, Color::Transparent, 10.0f),
        .Padding = Edges(0.0f),
        .Gradient = GradientData(Color::Pink, glm::vec2(0.25f, 0.2f), 0.1f, 0.9f)
        });

    auto logoIcon = std::make_shared<Image>();
    logoIcon->SetImage("note");
    logoIcon->SetSizing(SizeMode::Fixed, SizeMode::Fixed);
    logoIcon->SetFixedSize(18.0f, 18.0f);
    logoIcon->SetStyle(State::Default, { .BackgroundColor = Color::White });
    logoIcon->SetHitTestable(false);
    logoBox->SetChild(logoIcon);

    logoRow->AddChild(logoBox);
    logoRow->AddChild(makeText("Apple Music", 26, textMain, SizeMode::Fill));
    sidebarStack->AddChild(logoRow);

    // --- Search ---
    auto searchBox = std::make_shared<Border>();
    searchBox->SetSizing(SizeMode::Fill, SizeMode::Auto);
    searchBox->SetStyle(State::Default, {
        .BackgroundColor = Color::SurfaceAlt(0.7f, 0.8f),
        .Border = BorderData(1.0f, border, 10.0f),
        .Padding = Edges(16.0f, 12.0f)
        });
    searchBox->SetStyle(State::Hovered, { .Border = BorderData(1.0f, accentBorder, 10.0f) });

    auto searchRow = std::make_shared<HStack>();
    searchRow->SetSizing(SizeMode::Fill, SizeMode::Auto);
    searchRow->SetSpacing(10.0f);
    searchRow->SetAlignment(AlignMode::Start, AlignMode::Center);
    searchRow->SetHitTestable(false);

    auto searchIcon = std::make_shared<Image>();
    searchIcon->SetImage("search");
    searchIcon->SetSizing(SizeMode::Fixed, SizeMode::Fixed);
    searchIcon->SetFixedSize(18.0f, 18.0f);
    searchIcon->SetStyle(State::Default, { .BackgroundColor = textFaint });

    searchRow->AddChild(searchIcon);
    searchRow->AddChild(makeText("Search", 24, textFaint, SizeMode::Fill));
    searchBox->SetChild(searchRow);
    sidebarStack->AddChild(searchBox);

    // --- Kontener widoku, przełączany przez całą nawigację (Apple Music + Library) ---
    auto viewContainer = std::make_shared<Border>();
    viewContainer->SetSizing(SizeMode::Fill, SizeMode::Fill);
    viewContainer->SetStyle(State::Default, { .BackgroundColor = Color::Transparent, .Padding = Edges(0.0f) });

    // --- Apple Music nav ---
    sidebarStack->AddChild(makeSectionLabel("APPLE MUSIC"));
    auto homeNav = makeNavItem("home", "Home");
    auto browseNav = makeNavItem("browse", "Browse");
    auto radioNav = makeNavItem("radio", "Radio");
    sidebarStack->AddChild(homeNav.container);
    sidebarStack->AddChild(browseNav.container);
    sidebarStack->AddChild(radioNav.container);

    // --- Library (teraz też klikalne widoki) ---
    sidebarStack->AddChild(makeSectionLabel("LIBRARY"));
    auto recentNav = makeNavItem("clock", "Recently Added");
    auto artistsNav = makeNavItem("mic", "Artists");
    auto albumsNav = makeNavItem("disc", "Albums");
    auto songsNav = makeNavItem("note", "Songs");
    sidebarStack->AddChild(recentNav.container);
    sidebarStack->AddChild(artistsNav.container);
    sidebarStack->AddChild(albumsNav.container);
    sidebarStack->AddChild(songsNav.container);

    // Jedna wspólna lista nawigacji + odpowiadające jej buildery widoków (ten sam indeks)
    std::vector<NavEntry> navEntries = {
        homeNav, browseNav, radioNav, recentNav, artistsNav, albumsNav, songsNav
    };
    std::vector<std::function<std::shared_ptr<Widget>()>> viewBuilders = {
        buildHomeView, buildBrowseView, buildRadioView,
        buildRecentlyAddedView, buildArtistsView, buildAlbumsView, buildSongsView
    };

    for (size_t i = 0; i < navEntries.size(); ++i) {
        navEntries[i].container->SetCallback(Callback::OnPressStart, [=](Event) {
            for (size_t j = 0; j < navEntries.size(); ++j) {
                navEntries[j].setActive(j == i);
            }
            viewContainer->SetChild(viewBuilders[i]());
            });
    }

    homeNav.setActive(true);
    viewContainer->SetChild(buildHomeView());

    // --- Playlists (scrollowalna lista) ---
    sidebarStack->AddChild(makeSectionLabel("PLAYLISTS"));

    auto playlistList = std::make_shared<VStack>();
    playlistList->SetSizing(SizeMode::Fill, SizeMode::Auto);
    playlistList->SetSpacing(2.0f);

    static const std::vector<std::string> playlistNames = {
        "All Playlists", "Favourite Songs", "90s Hits",
        "90s Indie Rock", "Radio Recollection",
        "Assembly Music", "Walk On Music", "Signing Register",
        "Walk Off Music"
    };
    for (auto& name : playlistNames) playlistList->AddChild(makePlaylistRow(name));

    auto playlistScroll = std::make_shared<ScrollBox>();
    playlistScroll->SetChild(playlistList);
    playlistScroll->SetSizing(SizeMode::Fill, SizeMode::Fill);
    playlistScroll->SetStyle(State::Default, { .BackgroundColor = Color::Transparent, .Padding = Edges(0.0f) });
    sidebarStack->AddChild(playlistScroll);

    sidebar->SetChild(sidebarStack);

    // ================= CONTENT =================
    auto content = std::make_shared<Border>();
    content->SetSizing(SizeMode::Fill, SizeMode::Fill);
    content->SetStyle(State::Default, { .BackgroundColor = bgContent, .Padding = Edges(0.0f) });

    auto contentStack = std::make_shared<VStack>();
    contentStack->SetSizing(SizeMode::Fill, SizeMode::Fill);
    contentStack->SetSpacing(0.0f);

    // --- Górny pasek (playback controls) - z subtelnym gradientem w prawym rogu ---
    auto topBar = std::make_shared<Border>();
    topBar->SetSizing(SizeMode::Fill, SizeMode::Auto);
    topBar->SetStyle(State::Default, {
        .BackgroundColor = bgTopBar,
        .Padding = Edges(28.0f, 18.0f),
        .Gradient = GradientData(Color::Accent(0.08f), glm::vec2(0.9f, 0.0f), 0.05f, 1.3f)
        });

    auto topBarRow = std::make_shared<HStack>();
    topBarRow->SetSizing(SizeMode::Fill, SizeMode::Auto);
    topBarRow->SetSpacing(24.0f);
    topBarRow->SetAlignment(AlignMode::Start, AlignMode::Center);

    auto playbackIcons = std::make_shared<HStack>();
    playbackIcons->SetSizing(SizeMode::Auto, SizeMode::Auto);
    playbackIcons->SetSpacing(18.0f);
    playbackIcons->SetAlignment(AlignMode::Start, AlignMode::Center);
    for (const std::string& ic : { "shuffle", "rewind", "play", "forward", "repeat" }) {
        auto btn = std::make_shared<Border>();
        btn->SetSizing(SizeMode::Auto, SizeMode::Auto);
        btn->SetStyle(State::Default, { .BackgroundColor = Color::Transparent, .Padding = Edges(4.0f) });
        btn->SetStyle(State::Hovered, { .BackgroundColor = bgHover, .Border = BorderData(0.0f, Color::Transparent, 6.0f) });
        btn->SetStyle(State::Pressed, { .BackgroundColor = bgPressed, .Border = BorderData(0.0f, Color::Transparent, 6.0f) });

        auto icon = std::make_shared<Image>();
        icon->SetImage(ic);
        icon->SetSizing(SizeMode::Fixed, SizeMode::Fixed);
        icon->SetFixedSize(18.0f, 18.0f);
        icon->SetStyle(State::Default, { .BackgroundColor = textDim });
        icon->SetHitTestable(false);

        btn->SetChild(icon);
        playbackIcons->AddChild(btn);
    }

    auto nowPlaying = std::make_shared<Border>();
    nowPlaying->SetSizing(SizeMode::Fill, SizeMode::Auto);
    nowPlaying->SetAlignment(AlignMode::Center, AlignMode::Center);
    nowPlaying->SetStyle(State::Default, {
        .BackgroundColor = Color::SurfaceAlt(0.5f, 0.7f),
        .Border = BorderData(0.0f, Color::Transparent, 8.0f),
        .Padding = Edges(14.0f, 10.0f)
        });
    nowPlaying->SetChild(makeText("Not Playing", 24, textFaint));

    topBarRow->AddChild(playbackIcons);
    topBarRow->AddChild(nowPlaying);
    topBar->SetChild(topBarRow);
    contentStack->AddChild(topBar);

    // --- Separator ---
    auto divider = std::make_shared<Border>();
    divider->SetSizing(SizeMode::Fill, SizeMode::Fixed);
    divider->SetFixedSize(0.0f, 1.0f);
    divider->SetStyle(State::Default, { .BackgroundColor = border });
    contentStack->AddChild(divider);

    contentStack->AddChild(viewContainer);
    content->SetChild(contentStack);

    // ================= SKŁADANIE =================
    mainRow->AddChild(sidebar);
    mainRow->AddChild(content);
    root->SetChild(mainRow);

    return root;
}

// Unreal Engine - AI DEMO ( from source code )
std::shared_ptr<UI::Border> CreateAssetDetailUI(const std::string& name, const std::string& type,
    const glm::vec4& accentColor, const std::string& icon, const std::string& description)
{
    using namespace UI;

    const glm::vec4 bgRoot = glm::vec4(0.086f, 0.086f, 0.086f, 1.0f);
    const glm::vec4 bgPanelAlt = glm::vec4(0.114f, 0.114f, 0.114f, 1.0f);
    const glm::vec4 border = glm::vec4(0.035f, 0.035f, 0.035f, 1.0f);
    const glm::vec4 textMain = glm::vec4(0.860f, 0.860f, 0.860f, 1.0f);
    const glm::vec4 textDim = glm::vec4(0.550f, 0.550f, 0.550f, 1.0f);

    auto root = std::make_shared<Border>();
    root->SetSizing(SizeMode::Fill, SizeMode::Fill);
    root->SetStyle(State::Default, { .BackgroundColor = bgRoot, .Padding = Edges(0.0f) });

    auto stack = std::make_shared<VStack>();
    stack->SetSizing(SizeMode::Fill, SizeMode::Fill);
    stack->SetSpacing(0.0f);

    // Duży podgląd na górze - ten sam gradient co miniatura kafelka w Content Browserze, żeby okno
    // wyglądało jak "powiększenie" tego, co się kliknęło.
    auto preview = std::make_shared<Border>();
    preview->SetSizing(SizeMode::Fill, SizeMode::Fixed);
    preview->SetFixedSize(0.0f, 190.0f);
    preview->SetAlignment(AlignMode::Center, AlignMode::Center);
    glm::vec4 previewBase = glm::vec4(glm::vec3(accentColor) * 0.35f, 1.0f);
    preview->SetStyle(State::Default, {
        .BackgroundColor = previewBase,
        .Padding = Edges(0.0f),
        .Gradient = GradientData(accentColor, glm::vec2(0.3f, 0.2f), 0.1f, 0.9f)
        });

    auto previewIcon = std::make_shared<Image>();
    previewIcon->SetImage(icon);
    previewIcon->SetSizing(SizeMode::Fixed, SizeMode::Fixed);
    previewIcon->SetFixedSize(64.0f, 64.0f);
    previewIcon->SetStyle(State::Default, { .BackgroundColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.7f) });
    preview->SetChild(previewIcon);
    stack->AddChild(preview);

    // Treść: nazwa, typ (kolorem akcentu assetu), separator, opis.
    auto body = std::make_shared<VStack>();
    body->SetSizing(SizeMode::Fill, SizeMode::Fill);
    body->SetSpacing(14.0f);
    body->SetStyle(State::Default, { .Padding = Edges(22.0f) });

    body->AddChild(makeText(name, 28, textMain));
    body->AddChild(makeText(type, 21, accentColor));

    auto divider = std::make_shared<Border>();
    divider->SetSizing(SizeMode::Fill, SizeMode::Fixed);
    divider->SetFixedSize(0.0f, 1.0f);
    divider->SetStyle(State::Default, { .BackgroundColor = border });
    body->AddChild(divider);

    body->AddChild(makeText(description, 21, textDim, SizeMode::Fill));

    // Kilka pól metadanych - placeholdery, docelowo realne dane assetu (rozmiar, ścieżka, referencje...)
    auto metaRow = [&](const std::string& label, const std::string& value) {
        auto row = std::make_shared<HStack>();
        row->SetSizing(SizeMode::Fill, SizeMode::Auto);
        row->SetSpacing(12.0f);

        auto lbl = std::make_shared<Border>();
        lbl->SetSizing(SizeMode::Fixed, SizeMode::Auto);
        lbl->SetFixedSize(90.0f, 0.0f);
        lbl->SetChild(makeText(label, 20, textDim));

        auto val = std::make_shared<Border>();
        val->SetSizing(SizeMode::Fill, SizeMode::Auto);
        val->SetStyle(State::Default, { .BackgroundColor = bgPanelAlt, .Border = BorderData(1.0f, border, 5.0f), .Padding = Edges(9.0f, 6.0f) });
        val->SetChild(makeText(value, 20, textMain, SizeMode::Fill));

        row->AddChild(lbl);
        row->AddChild(val);
        body->AddChild(row);
        };
    metaRow("Path", "/Game/Environment/" + name);
    metaRow("Type", type);

    stack->AddChild(body);
    root->SetChild(stack);
    return root;
}

std::shared_ptr<UI::Border> CreateUnrealEditorUI()
{
    using namespace UI;

    // ================= PALETA =================
    // UWAGA: nie mam Twojego prawdziwego pliku z Color:: tokenami (dotarł tylko screenshot, nie kod).
    // Dotychczas zgadywałem nazwy i semantykę parametrów (opacity/brightness), stąd rozjazd jasności
    // między wersjami. Tutaj celowo surowe glm::vec4 - zero zależności od Twoich tokenów, żeby mieć
    // pełną, przewidywalną kontrolę nad tym że jest NAPRAWDĘ ciemno (realne wartości z UE5, nie UE4 -
    // UE5 jest bardziej neutralnie-szare, mniej niebieskawe niż UE4's Slate).
    // Wklej mi realny Color.h / paletę, a przepiszę to 1:1 na Twoje tokeny.
    const glm::vec4 accent = glm::vec4(0.114f, 0.400f, 0.635f, 1.0f);       // ~#1D66A2 - przygaszony selection-blue
    const glm::vec4 accentDim = glm::vec4(0.114f, 0.400f, 0.635f, 0.14f);
    const glm::vec4 accentGlow = glm::vec4(0.114f, 0.400f, 0.635f, 0.30f);
    const glm::vec4 accentBorder = glm::vec4(0.180f, 0.480f, 0.720f, 0.85f);

    const glm::vec4 playGreen = glm::vec4(0.290f, 0.640f, 0.290f, 1.0f);
    const glm::vec4 warnAmber = glm::vec4(0.850f, 0.600f, 0.150f, 1.0f);
    const glm::vec4 stopRed = glm::vec4(0.750f, 0.220f, 0.220f, 1.0f);

    const glm::vec4 bgRoot = glm::vec4(0.043f, 0.043f, 0.043f, 1.0f);       // ~#0B0B0B
    const glm::vec4 bgPanel = glm::vec4(0.086f, 0.086f, 0.086f, 1.0f);      // ~#161616
    const glm::vec4 bgPanelAlt = glm::vec4(0.114f, 0.114f, 0.114f, 1.0f);   // ~#1D1D1D
    const glm::vec4 bgToolbar = glm::vec4(0.098f, 0.098f, 0.098f, 1.0f);    // ~#191919
    const glm::vec4 bgViewport = glm::vec4(0.016f, 0.016f, 0.018f, 1.0f);   // niemal czysta czerń
    const glm::vec4 bgHover = glm::vec4(0.160f, 0.160f, 0.160f, 1.0f);
    const glm::vec4 bgPressed = glm::vec4(0.200f, 0.200f, 0.200f, 1.0f);
    const glm::vec4 bgSelected = glm::vec4(0.114f, 0.400f, 0.635f, 0.18f);
    const glm::vec4 border = glm::vec4(0.035f, 0.035f, 0.035f, 1.0f);       // ledwo widoczny separator, jak w realnym UE5
    const glm::vec4 textMain = glm::vec4(0.860f, 0.860f, 0.860f, 1.0f);
    const glm::vec4 textDim = glm::vec4(0.550f, 0.550f, 0.550f, 1.0f);
    const glm::vec4 textFaint = glm::vec4(0.550f, 0.550f, 0.550f, 0.65f);

    // ================= HELPERY =================

    struct TreeRowHandle {
        std::shared_ptr<Border> container;
        std::function<void(bool)> setSelected;
    };

    // Wiersz w World Outliner - ikona typu aktora + nazwa + wcięcie zależne od głębokości.
    auto makeOutlinerRow = [=](const std::string& icon, const std::string& name, int depth) -> TreeRowHandle {
        auto row = std::make_shared<Border>();
        row->SetSizing(SizeMode::Fill, SizeMode::Auto);
        row->SetStyle(State::Hovered, { .BackgroundColor = bgHover });
        row->SetStyle(State::Pressed, { .BackgroundColor = bgPressed });

        auto inner = std::make_shared<HStack>();
        inner->SetSizing(SizeMode::Fill, SizeMode::Auto);
        inner->SetSpacing(10.0f);
        inner->SetAlignment(AlignMode::Start, AlignMode::Center);
        inner->SetHitTestable(false);

        auto indent = std::make_shared<Border>();
        indent->SetSizing(SizeMode::Fixed, SizeMode::Fixed);
        indent->SetFixedSize(depth * 16.0f, 1.0f);

        auto ic = std::make_shared<Image>();
        ic->SetImage(icon);
        ic->SetSizing(SizeMode::Fixed, SizeMode::Fixed);
        ic->SetFixedSize(16.0f, 16.0f);

        auto lbl = makeText(name, 22, textMain, SizeMode::Fill);

        inner->AddChild(indent);
        inner->AddChild(ic);
        inner->AddChild(lbl);
        row->SetChild(inner);

        auto setSelected = [=](bool selected) {
            row->SetStyle(State::Default, {
                .BackgroundColor = selected ? bgSelected : Color::Transparent,
                .Border = BorderData(selected ? 1.0f : 0.0f, selected ? accentBorder : Color::Transparent, 4.0f),
                .Padding = Edges(8.0f, 6.0f)
                });
            ic->SetStyle(State::Default, { .BackgroundColor = selected ? accent : textDim });
            lbl->SetStyle(State::Default, { .Color = selected ? accent : textMain });
            };
        setSelected(false);
        return { row, setSelected };
        };

    auto makeSectionLabel = [=](const std::string& text) {
        auto wrap = std::make_shared<Border>();
        wrap->SetSizing(SizeMode::Fill, SizeMode::Auto);
        wrap->SetStyle(State::Default, { .Padding = Edges(10.0f, 8.0f) });
        wrap->SetChild(makeText(text, 20, textFaint));
        return wrap;
        };

    // Pojedynczy przycisk w toolbarze (Play / Pause / Stop / Compile / Save...)
    auto makeToolbarButton = [=](const std::string& icon, const std::string& label, const glm::vec4& tint) {
        auto btn = std::make_shared<Border>();
        btn->SetSizing(SizeMode::Auto, SizeMode::Auto);
        btn->SetStyle(State::Default, { .BackgroundColor = Color::Transparent,  .Border = BorderData(0.0f, Color::Transparent, 6.0f), .Padding = Edges(12.0f, 8.0f), });
        btn->SetStyle(State::Hovered, { .BackgroundColor = bgHover, .Border = BorderData(0.0f, Color::Transparent, 6.0f) });
        btn->SetStyle(State::Pressed, { .BackgroundColor = bgPressed, .Border = BorderData(0.0f, Color::Transparent, 6.0f) });

        auto row = std::make_shared<HStack>();
        row->SetSizing(SizeMode::Auto, SizeMode::Auto);
        row->SetSpacing(8.0f);
        row->SetAlignment(AlignMode::Start, AlignMode::Center);
        row->SetHitTestable(false);

        auto ic = std::make_shared<Image>();
        ic->SetImage(icon);
        ic->SetSizing(SizeMode::Fixed, SizeMode::Fixed);
        ic->SetFixedSize(18.0f, 18.0f);
        ic->SetStyle(State::Default, { .BackgroundColor = tint });

        row->AddChild(ic);
        if (!label.empty())
            row->AddChild(makeText(label, 22, textMain));

        btn->SetChild(row);
        return btn;
        };

    // Wiersz właściwości (Details panel) - etykieta po lewej + edytowalna wartość po prawej.
    auto makePropertyRow = [=](const std::string& label, const std::string& value) {
        auto row = std::make_shared<HStack>();
        row->SetSizing(SizeMode::Fill, SizeMode::Auto);
        row->SetSpacing(12.0f);
        row->SetAlignment(AlignMode::Start, AlignMode::Center);

        auto lbl = std::make_shared<Border>();
        lbl->SetSizing(SizeMode::Fixed, SizeMode::Auto);
        lbl->SetFixedSize(110.0f, 0.0f);
        lbl->SetChild(makeText(label, 21, textDim));

        auto valueBox = std::make_shared<Border>();
        valueBox->SetSizing(SizeMode::Fill, SizeMode::Auto);
        valueBox->SetStyle(State::Default, {
            .BackgroundColor = bgPanelAlt,
            .Border = BorderData(1.0f, border, 6.0f),
            .Padding = Edges(10.0f, 7.0f)
            });
        valueBox->SetStyle(State::Hovered, { .Border = BorderData(1.0f, accentBorder, 6.0f) });
        valueBox->SetChild(makeText(value, 21, textMain, SizeMode::Fill));

        row->AddChild(lbl);
        row->AddChild(valueBox);
        return row;
        };

    // Nagłówek kategorii we właściwościach (np. "Transform", "Static Mesh"), z linią pod spodem.
    auto makeCategoryHeader = [=](const std::string& title) {
        auto col = std::make_shared<VStack>();
        col->SetSizing(SizeMode::Fill, SizeMode::Auto);
        col->SetSpacing(6.0f);

        col->AddChild(makeText(title, 23, textMain));

        auto rule = std::make_shared<Border>();
        rule->SetSizing(SizeMode::Fill, SizeMode::Fixed);
        rule->SetFixedSize(0.0f, 1.0f);
        rule->SetStyle(State::Default, { .BackgroundColor = border });
        col->AddChild(rule);

        return col;
        };

    // Kafelek assetu w Content Browserze - miniatura z gradientem wg typu + nazwa + typ.
    // Zmniejszony względem pierwszej wersji: mniejsza miniatura i mniejsza ikona w środku.
    auto makeAssetTile = [=](const std::string& name, const std::string& type, const glm::vec4& tileColor, const std::string& icon) {
        const float kThumbSize = 78.0f;
        const float kIconSize = 26.0f;

        auto tile = std::make_shared<Border>();
        tile->SetSizing(SizeMode::Fixed, SizeMode::Auto);
        tile->SetFixedSize(96.0f, 0.0f);
        tile->SetStyle(State::Default, { .BackgroundColor = Color::Transparent,.Border = BorderData(0.0f, Color::Transparent, 8.0f),.Padding = Edges(6.0f), });
        tile->SetStyle(State::Hovered, { .BackgroundColor = bgHover, .Border = BorderData(1.0f, accentBorder, 8.0f), .Padding = Edges(6.0f) });
        tile->SetStyle(State::Pressed, { .BackgroundColor = bgPressed, .Border = BorderData(1.5f, accent, 8.0f), .Padding = Edges(6.0f) });

        auto col = std::make_shared<VStack>();
        col->SetSizing(SizeMode::Fill, SizeMode::Auto);
        col->SetSpacing(8.0f);
        col->SetHitTestable(false);

        auto thumb = std::make_shared<Border>();
        thumb->SetSizing(SizeMode::Fixed, SizeMode::Fixed);
        thumb->SetFixedSize(kThumbSize, kThumbSize);
        thumb->SetAlignment(AlignMode::Center, AlignMode::Center);
        glm::vec4 baseDark = glm::vec4(glm::vec3(tileColor) * 0.4f, 1.0f);
        thumb->SetStyle(State::Default, {
            .BackgroundColor = baseDark,
            .Border = BorderData(0.0f, Color::Transparent, 8.0f),
            .Gradient = GradientData(tileColor, glm::vec2(0.3f, 0.2f), 0.1f, 0.9f)
            });

        auto ic = std::make_shared<Image>();
        ic->SetImage(icon);
        ic->SetSizing(SizeMode::Fixed, SizeMode::Fixed);
        ic->SetFixedSize(kIconSize, kIconSize);
        ic->SetStyle(State::Default, { .BackgroundColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.6f) });
        thumb->SetChild(ic);

        col->AddChild(thumb);
        col->AddChild(makeText(name, 20, textMain, SizeMode::Fill, SizeMode::Auto, WrapMode::Elipsis));
        col->AddChild(makeText(type, 20, textFaint, SizeMode::Fill, SizeMode::Auto, WrapMode::Elipsis));

        tile->SetChild(col);

        // Klik w kafelek -> nowe okno ze szczegółami tego assetu.
        // Capture [=] (nie [&]) bo callback jest przechowywany i odpalany długo po wyjściu z tej lambdy -
        // złapanie przez referencję dałoby dangling refs do name/type/tileColor/icon (ten sam błąd co
        // wcześniej w makeText/handleSubmit).
        tile->SetCallback(Callback::OnPressStart, [=](Event) {
            std::string description = "Zasob typu " + type + " uzywany w tym projekcie. Kliknij i przeciagnij, zeby umiescic go w scenie.";
            auto win = WidgetManager::Get().CreateWindow(name + " - Asset Details", glm::vec2(400, 600), false);


            WidgetManager::Get().SetRootWidget(win, CreateAssetDetailUI(name, type, tileColor, icon, description));
            });

        return tile;
        };

    auto makeScrollBody = [](std::shared_ptr<VStack> body) -> std::shared_ptr<Widget> {
        auto scroll = std::make_shared<ScrollBox>();
        scroll->SetChild(body);
        scroll->SetSizing(SizeMode::Fill, SizeMode::Fill);
        scroll->SetStyle(State::Default, { .BackgroundColor = Color::Transparent, .Padding = Edges(0.0f) });
        return scroll;
        };

    // ================= ROOT =================
    auto root = std::make_shared<Border>();
    root->SetSizing(SizeMode::Fill, SizeMode::Fill);
    root->SetStyle(State::Default, { .BackgroundColor = bgRoot, .Padding = Edges(0.0f) });

    auto rootStack = std::make_shared<VStack>();
    rootStack->SetSizing(SizeMode::Fill, SizeMode::Fill);
    rootStack->SetSpacing(0.0f);

    // ================= MENU BAR =================
    auto menuBar = std::make_shared<Border>();
    menuBar->SetSizing(SizeMode::Fill, SizeMode::Auto);
    menuBar->SetStyle(State::Default, { .BackgroundColor = bgToolbar, .Padding = Edges(16.0f, 8.0f) });

    auto menuRow = std::make_shared<HStack>();
    menuRow->SetSizing(SizeMode::Fill, SizeMode::Auto);
    menuRow->SetSpacing(4.0f);
    menuRow->SetAlignment(AlignMode::Start, AlignMode::Center);

    for (const std::string& m : { "File", "Edit", "Window", "Tools", "Build", "Select", "Actor", "Help" }) {
        auto item = std::make_shared<Border>();
        item->SetSizing(SizeMode::Auto, SizeMode::Auto);
        item->SetStyle(State::Default, { .Border = BorderData(0.0f, Color::Transparent, 5.0f), .Padding = Edges(10.0f, 6.0f), });
        item->SetStyle(State::Hovered, { .BackgroundColor = bgHover,  .Border = BorderData(0.0f, Color::Transparent, 5.0f), .Padding = Edges(10.0f, 6.0f), });
        item->SetChild(makeText(m, 21, textDim));
        menuRow->AddChild(item);
    }
    menuBar->SetChild(menuRow);
    rootStack->AddChild(menuBar);

    // ================= TOOLBAR =================
    auto toolbar = std::make_shared<Border>();
    toolbar->SetSizing(SizeMode::Fill, SizeMode::Auto);
    toolbar->SetStyle(State::Default, {
        .BackgroundColor = bgToolbar,
        .Border = BorderData(0.0f, Color::Transparent, 0.0f),
        .Padding = Edges(20.0f, 10.0f),
        });

    auto toolbarRow = std::make_shared<HStack>();
    toolbarRow->SetSizing(SizeMode::Fill, SizeMode::Auto);
    toolbarRow->SetSpacing(6.0f);
    toolbarRow->SetAlignment(AlignMode::Start, AlignMode::Center);

    toolbarRow->AddChild(makeToolbarButton("save", "Save", textDim));
    toolbarRow->AddChild(makeToolbarButton("compile", "Compile", textDim));

    auto sep1 = std::make_shared<Border>();
    sep1->SetSizing(SizeMode::Fixed, SizeMode::Fixed);
    sep1->SetFixedSize(1.0f, 24.0f);
    sep1->SetStyle(State::Default, { .BackgroundColor = border });
    toolbarRow->AddChild(sep1);

    toolbarRow->AddChild(makeToolbarButton("play", "Play", playGreen));
    toolbarRow->AddChild(makeToolbarButton("pause", "", warnAmber));
    toolbarRow->AddChild(makeToolbarButton("stop", "", stopRed));

    auto sep2 = std::make_shared<Border>();
    sep2->SetSizing(SizeMode::Fixed, SizeMode::Fixed);
    sep2->SetFixedSize(1.0f, 24.0f);
    sep2->SetStyle(State::Default, { .BackgroundColor = border });
    toolbarRow->AddChild(sep2);

    toolbarRow->AddChild(makeToolbarButton("settings", "Settings", textDim));

    // Spacer wypychający statystyki FPS/Ping na prawą stronę toolbaru
    auto toolbarSpacer = std::make_shared<Border>();
    toolbarSpacer->SetSizing(SizeMode::Fill, SizeMode::Fixed);
    toolbarSpacer->SetFixedSize(0.0f, 1.0f);
    toolbarRow->AddChild(toolbarSpacer);

    auto statsPill = std::make_shared<Border>();
    statsPill->SetSizing(SizeMode::Auto, SizeMode::Auto);
    statsPill->SetStyle(State::Default, {
        .BackgroundColor = bgPanelAlt,
        .Border = BorderData(1.0f, border, 20.0f),
        .Padding = Edges(14.0f, 8.0f)
        });
    statsPill->SetChild(makeText("60 FPS  |  12 ms", 20, textDim));
    toolbarRow->AddChild(statsPill);

    toolbar->SetChild(toolbarRow);
    rootStack->AddChild(toolbar);

    auto topDivider = std::make_shared<Border>();
    topDivider->SetSizing(SizeMode::Fill, SizeMode::Fixed);
    topDivider->SetFixedSize(0.0f, 1.0f);
    topDivider->SetStyle(State::Default, { .BackgroundColor = border });
    rootStack->AddChild(topDivider);

    // ================= ŚRODEK: Outliner | Viewport | Details =================
    auto middleRow = std::make_shared<HStack>();
    middleRow->SetSizing(SizeMode::Fill, SizeMode::Fill);
    middleRow->SetSpacing(0.0f);

    // --- World Outliner (lewy panel) ---
    auto outliner = std::make_shared<Border>();
    outliner->SetSizing(SizeMode::Fixed, SizeMode::Fill);
    outliner->SetFixedSize(260.0f, 0.0f);
    outliner->SetStyle(State::Default, { .BackgroundColor = bgPanel, .Border = BorderData(1.0f, border, 0.0f) });

    auto outlinerStack = std::make_shared<VStack>();
    outlinerStack->SetSizing(SizeMode::Fill, SizeMode::Fill);
    outlinerStack->SetSpacing(2.0f);
    outlinerStack->SetStyle(State::Default, { .Padding = Edges(14.0f) });

    auto outlinerHeaderRow = std::make_shared<HStack>();
    outlinerHeaderRow->SetSizing(SizeMode::Fill, SizeMode::Auto);
    outlinerHeaderRow->SetAlignment(AlignMode::Start, AlignMode::Center);
    outlinerHeaderRow->AddChild(makeText("World Outliner", 24, textMain, SizeMode::Fill));
    outlinerStack->AddChild(outlinerHeaderRow);

    auto outlinerSearch = std::make_shared<Border>();
    outlinerSearch->SetSizing(SizeMode::Fill, SizeMode::Auto);
    outlinerSearch->SetStyle(State::Default, {
        .BackgroundColor = bgPanelAlt,
        .Border = BorderData(1.0f, border, 6.0f),
        .Padding = Edges(12.0f, 8.0f)
        });
    outlinerSearch->SetChild(makeText("Search actors...", 21, textFaint));
    outlinerStack->AddChild(outlinerSearch);

    outlinerStack->AddChild(makeSectionLabel("PERSISTENT LEVEL"));

    std::vector<TreeRowHandle> outlinerRows;
    outlinerRows.push_back(makeOutlinerRow("light", "DirectionalLight", 0));
    outlinerRows.push_back(makeOutlinerRow("landscape", "Landscape", 0));
    outlinerRows.push_back(makeOutlinerRow("folder", "Environment", 0));
    outlinerRows.push_back(makeOutlinerRow("mesh", "SM_Rock_01", 1));
    outlinerRows.push_back(makeOutlinerRow("mesh", "SM_Tree_03", 1));
    outlinerRows.push_back(makeOutlinerRow("folder", "Gameplay", 0));
    outlinerRows.push_back(makeOutlinerRow("player", "PlayerStart", 1));
    outlinerRows.push_back(makeOutlinerRow("cube", "BP_EnemySpawner", 1));
    outlinerRows.push_back(makeOutlinerRow("camera", "CineCameraActor", 0));
    outlinerRows.push_back(makeOutlinerRow("sky", "SkyAtmosphere", 0));

    for (auto& r : outlinerRows) outlinerStack->AddChild(r.container);

    // Klik w wiersz Outlinera zaznacza go i odznacza resztę (single-select, jak w edytorze)
    for (size_t i = 0; i < outlinerRows.size(); ++i) {
        outlinerRows[i].container->SetCallback(Callback::OnPressStart, [=](Event) {
            for (size_t j = 0; j < outlinerRows.size(); ++j)
                outlinerRows[j].setSelected(j == i);
            });
    }
    outlinerRows[3].setSelected(true); // SM_Rock_01 domyślnie zaznaczony, żeby Details miał co pokazać

    outliner->SetChild(makeScrollBody(outlinerStack));
    middleRow->AddChild(outliner);

    // --- Viewport (środek) ---
    auto viewportWrap = std::make_shared<Border>();
    viewportWrap->SetSizing(SizeMode::Fill, SizeMode::Fill);
    viewportWrap->SetStyle(State::Default, { .BackgroundColor = bgViewport, .Padding = Edges(0.0f) });

    auto viewportStack = std::make_shared<VStack>();
    viewportStack->SetSizing(SizeMode::Fill, SizeMode::Fill);
    viewportStack->SetSpacing(0.0f);

    // Pasek nad viewportem: tryb widoku (Lit) + gizmo mode + FOV
    auto viewportTopBar = std::make_shared<HStack>();
    viewportTopBar->SetSizing(SizeMode::Fill, SizeMode::Auto);
    viewportTopBar->SetSpacing(10.0f);
    viewportTopBar->SetAlignment(AlignMode::Start, AlignMode::Center);
    viewportTopBar->SetStyle(State::Default, { .Padding = Edges(14.0f, 10.0f) });

    for (const std::string& label : { "Perspective", "Lit", "Show" }) {
        auto pill = std::make_shared<Border>();
        pill->SetSizing(SizeMode::Auto, SizeMode::Auto);
        pill->SetStyle(State::Default, {
            .BackgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.35f),
            .Border = BorderData(1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 0.1f), 6.0f),
            .Padding = Edges(12.0f, 7.0f)
            });
        pill->SetStyle(State::Hovered, { .Border = BorderData(1.0f, accentBorder, 6.0f) });
        pill->SetChild(makeText(label, 20, glm::vec4(1.0f, 1.0f, 1.0f, 0.75f)));
        viewportTopBar->AddChild(pill);
    }
    viewportStack->AddChild(viewportTopBar);

    // Sam viewport - w docelowej integracji z silnikiem to Image renderujący FBO sceny
    auto viewportSurface = std::make_shared<Border>();
    viewportSurface->SetSizing(SizeMode::Fill, SizeMode::Fill);
    viewportSurface->SetAlignment(AlignMode::Center, AlignMode::Center);
    viewportSurface->SetStyle(State::Default, {
        .BackgroundColor = bgViewport,
        .Padding = Edges(0.0f),
        .Gradient = GradientData(glm::vec4(0.045f, 0.045f, 0.050f, 1.0f), glm::vec2(0.5f, 0.35f), 0.05f, 1.6f)
        });
    // viewportSurface->SetHitTestable(false); // wyłączyć gdy podłączony osobny InputManager silnika
    viewportSurface->SetChild(makeText("Viewport (Scene FBO)", 22, textFaint));
    viewportStack->AddChild(viewportSurface);

    // Dolny pasek transform gizmo (Move / Rotate / Scale) + snapping
    auto viewportBottomBar = std::make_shared<Border>();
    viewportBottomBar->SetSizing(SizeMode::Fill, SizeMode::Auto);
    viewportBottomBar->SetStyle(State::Default, { .BackgroundColor = bgToolbar, .Padding = Edges(16.0f, 8.0f) });

    auto gizmoRow = std::make_shared<HStack>();
    gizmoRow->SetSizing(SizeMode::Fill, SizeMode::Auto);
    gizmoRow->SetSpacing(6.0f);
    gizmoRow->SetAlignment(AlignMode::Start, AlignMode::Center);
    gizmoRow->AddChild(makeToolbarButton("move", "Move", accent));
    gizmoRow->AddChild(makeToolbarButton("rotate", "Rotate", textDim));
    gizmoRow->AddChild(makeToolbarButton("scale", "Scale", textDim));
    viewportBottomBar->SetChild(gizmoRow);
    viewportStack->AddChild(viewportBottomBar);

    viewportWrap->SetChild(viewportStack);
    middleRow->AddChild(viewportWrap);

    // --- Details panel (prawy panel) ---
    auto details = std::make_shared<Border>();
    details->SetSizing(SizeMode::Fixed, SizeMode::Fill);
    details->SetFixedSize(320.0f, 0.0f);
    details->SetStyle(State::Default, { .BackgroundColor = bgPanel, .Border = BorderData(1.0f, border, 0.0f) });

    auto detailsStack = std::make_shared<VStack>();
    detailsStack->SetSizing(SizeMode::Fill, SizeMode::Auto);
    detailsStack->SetSpacing(20.0f);
    detailsStack->SetStyle(State::Default, { .Padding = Edges(16.0f) });

    detailsStack->AddChild(makeText("SM_Rock_01", 26, textMain));
    detailsStack->AddChild(makeText("StaticMeshActor", 21, textFaint));

    auto transformSection = std::make_shared<VStack>();
    transformSection->SetSizing(SizeMode::Fill, SizeMode::Auto);
    transformSection->SetSpacing(10.0f);
    transformSection->AddChild(makeCategoryHeader("Transform"));
    transformSection->AddChild(makePropertyRow("Location", "120.0, 0.0, 45.0"));
    transformSection->AddChild(makePropertyRow("Rotation", "0.0, 0.0, 0.0"));
    transformSection->AddChild(makePropertyRow("Scale", "1.0, 1.0, 1.0"));
    detailsStack->AddChild(transformSection);

    auto meshSection = std::make_shared<VStack>();
    meshSection->SetSizing(SizeMode::Fill, SizeMode::Auto);
    meshSection->SetSpacing(10.0f);
    meshSection->AddChild(makeCategoryHeader("Static Mesh"));
    meshSection->AddChild(makePropertyRow("Static Mesh", "SM_Rock_01"));
    meshSection->AddChild(makePropertyRow("Material", "M_Rock_Master"));
    meshSection->AddChild(makePropertyRow("Cast Shadow", "true"));
    detailsStack->AddChild(meshSection);

    auto collisionSection = std::make_shared<VStack>();
    collisionSection->SetSizing(SizeMode::Fill, SizeMode::Auto);
    collisionSection->SetSpacing(10.0f);
    collisionSection->AddChild(makeCategoryHeader("Collision"));
    collisionSection->AddChild(makePropertyRow("Collision Preset", "BlockAll"));
    collisionSection->AddChild(makePropertyRow("Generate Overlap", "false"));
    detailsStack->AddChild(collisionSection);

    details->SetChild(makeScrollBody(detailsStack));
    middleRow->AddChild(details);

    rootStack->AddChild(middleRow);

    auto bottomDivider = std::make_shared<Border>();
    bottomDivider->SetSizing(SizeMode::Fill, SizeMode::Fixed);
    bottomDivider->SetFixedSize(0.0f, 1.0f);
    bottomDivider->SetStyle(State::Default, { .BackgroundColor = border });
    rootStack->AddChild(bottomDivider);

    // ================= DÓŁ: Content Browser =================
    // Lekko wyższy (220 -> 250) żeby zmniejszone kafelki miały oddech, nie żeby czuć się ciasno.
    auto contentBrowser = std::make_shared<Border>();
    contentBrowser->SetSizing(SizeMode::Fill, SizeMode::Fixed);
    contentBrowser->SetFixedSize(0.0f, 250.0f);
    contentBrowser->SetStyle(State::Default, { .BackgroundColor = bgPanel, .Padding = Edges(0.0f) });

    auto contentStack = std::make_shared<VStack>();
    contentStack->SetSizing(SizeMode::Fill, SizeMode::Fill);
    contentStack->SetSpacing(12.0f);
    contentStack->SetStyle(State::Default, { .Padding = Edges(18.0f, 14.0f) });

    auto contentHeaderRow = std::make_shared<HStack>();
    contentHeaderRow->SetSizing(SizeMode::Fill, SizeMode::Auto);
    contentHeaderRow->SetSpacing(8.0f);
    contentHeaderRow->SetAlignment(AlignMode::Start, AlignMode::Center);
    contentHeaderRow->AddChild(makeText("Content Browser", 24, textMain));
    contentHeaderRow->AddChild(makeText("/Game/Environment/Rocks", 20, textFaint, SizeMode::Fill));
    contentStack->AddChild(contentHeaderRow);

    auto assetRow = std::make_shared<HStack>();
    assetRow->SetSizing(SizeMode::Fill, SizeMode::Auto);
    assetRow->SetSpacing(20.0f);

    // Rzeczywiste tokeny (Sapphire/Sage/...) nie są zweryfikowane, więc surowe wartości - stonowane,
    // ale rozróżnialne barwy per typ assetu, jak w prawdziwym UE5 Content Browserze.
    static const glm::vec4 kAssetPalette[] = {
        glm::vec4(0.180f, 0.420f, 0.650f, 1.0f), // mesh - stonowany niebieski
        glm::vec4(0.360f, 0.560f, 0.280f, 1.0f), // material - stonowana zieleń
        glm::vec4(0.560f, 0.320f, 0.620f, 1.0f), // texture - fiolet
        glm::vec4(0.220f, 0.460f, 0.720f, 1.0f), // blueprint - niebieski (jak w UE)
        glm::vec4(0.260f, 0.520f, 0.400f, 1.0f), // animation - morska zieleń
        glm::vec4(0.660f, 0.420f, 0.180f, 1.0f), // audio - rdzawy pomarańcz
        glm::vec4(0.200f, 0.470f, 0.470f, 1.0f), // world - cyjan
        glm::vec4(0.620f, 0.500f, 0.180f, 1.0f)  // fx - ochra
    };

    assetRow->AddChild(makeAssetTile("SM_Rock_01", "Static Mesh", kAssetPalette[0], "mesh"));
    assetRow->AddChild(makeAssetTile("M_Rock_Master", "Material", kAssetPalette[1], "material"));
    assetRow->AddChild(makeAssetTile("T_Rock_Diffuse", "Texture2D", kAssetPalette[2], "image"));
    assetRow->AddChild(makeAssetTile("BP_EnemySpawner", "Blueprint", kAssetPalette[3], "blueprint"));
    assetRow->AddChild(makeAssetTile("Anim_Idle", "Anim Sequence", kAssetPalette[4], "animation"));
    assetRow->AddChild(makeAssetTile("SFX_Footstep", "Sound Cue", kAssetPalette[5], "audio"));
    assetRow->AddChild(makeAssetTile("Level_Forest", "World", kAssetPalette[6], "landscape"));
    assetRow->AddChild(makeAssetTile("PP_Volume", "Post Process", kAssetPalette[7], "fx"));

    auto assetScroll = std::make_shared<ScrollBox>();
    assetScroll->SetChild(assetRow); // ScrollBox pozioma - jeśli u Ciebie domyślnie jest wertykalna, patrz notka niżej
    assetScroll->SetSizing(SizeMode::Fill, SizeMode::Fill);
    assetScroll->SetStyle(State::Default, { .BackgroundColor = Color::Transparent, .Padding = Edges(0.0f) });
    contentStack->AddChild(assetScroll);

    contentBrowser->SetChild(contentStack);
    rootStack->AddChild(contentBrowser);

    root->SetChild(rootStack);
    return root;
}

int main() {
    using namespace UI;

    auto win2 = WidgetManager::Get().CreateWindow("Unreal Engine");
    WidgetManager::Get().SetRootWidget(win2, CreateUnrealEditorUI());

    // Work in progress - test
    
    //std::vector<std::filesystem::path> files;
//std::vector<std::filesystem::path> sdfFiles;
//std::filesystem::path readDir = UI::AssetManager::Get().GetImagesDir() / "test/IconsPNG";
//std::filesystem::path writePath = UI::AssetManager::Get().GetImagesDir() / "test/IconsSDF";

//std::filesystem::path readFile = UI::AssetManager::Get().GetImagesDir() / "test/ct.png";
//std::filesystem::path writeFile = UI::AssetManager::Get().GetImagesDir() / "test/DC.png";
//
//ImageUtils::Image img = ImageUtils::ReadImage(readFile);

//for (auto i{ 0 }; i < 2 ; ++i) {
//    ImageUtils::Downscale(img);
//}

//ImageUtils::Image sdf = ImageUtils::GenerateSDF(img, 12);


//ImageUtils::WriteImage(writeFile, sdf);
//
//return 0;

//for (const auto& entry : std::filesystem::directory_iterator(readDir)) {
//    files.emplace_back(entry.path());
//}
//

//std::vector<ImageUtils::AtlasEntry> atlasEntries;
//uint16_t f = 0;
//for (const auto& file : files) {
//    ImageUtils::Image img = ImageUtils::ReadImage(file);
//    ImageUtils::ApplyPadding(img, 16);

//    ImageUtils::Image sdf = ImageUtils::GenerateSDF(img, 12);
//    ImageUtils::Downscale(sdf);
//    ImageUtils::Downscale(sdf);

//    ImageUtils::AtlasEntry entry;
//    entry.ID = f;
//    entry.image = std::move(sdf);

//    atlasEntries.push_back(entry);
//    //ImageUtils::WriteImage(writePath / file.filename(), sdf);

//    sdfFiles.push_back(file);
//    ++f;
//}


//ImageUtils::Atlas atlas = ImageUtils::GenerateAtlas(atlasEntries);
//ImageUtils::WriteImage(UI::AssetManager::Get().GetImagesDir() / "test/Atlas.png", atlas.Image);

//for (auto [ID, RECT] : atlas.Rects) {
//    Logger::Get().PrintToConsole("ID: {}, x: {}, y: {}", LogInfo, ID, RECT.x, RECT.y);
//}

//for (auto i{ 0 }; i < atlas.Rects.size(); ++i) {
//    const ImageUtils::Rect& imgRect = atlas.Rects[i];
//    Logger::Get().PrintToConsole("Rect {}, x: {}, y: {}", LogDebug, i, imgRect.x, imgRect.y);
//}

//auto ToPascalCase = [](const std::string& input) {
//    std::string result;
//    bool capitalizeNext = true; // Pierwsza litera zawsze wielka

//    for (char c : input) {
//        if (c == '-') {
//            capitalizeNext = true; // Następna litera ma być wielka
//        }
//        else {
//            if (capitalizeNext) {
//                result += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
//                capitalizeNext = false;
//            }
//            else {
//                result += c;
//            }
//        }
//    }
//    return result;
//    };

//std::string iconsEnum = "enum class Icons {\n";
//for(const auto& file : sdfFiles) {
//    std::string pascalName = ToPascalCase(file.stem().string());
//    iconsEnum += pascalName + ",\n";
//}
//iconsEnum += "};";

//std::filesystem::path opath = UI::AssetManager::Get().GetImagesDir() / "test/enum.cpp";
//std::ofstream file(opath);
//file << (iconsEnum.c_str());

//auto end = std::chrono::high_resolution_clock::now();
//std::cout << "[LOG] Icon Processing: "
//    << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";

 	// Main loop
    while (WidgetManager::Get().IsRunning()) WidgetManager::Get().Render();

    // Cleanup
	glfwTerminate();
    return 0;
}