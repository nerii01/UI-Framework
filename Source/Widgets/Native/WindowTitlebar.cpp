#include "WindowTitlebar.h"

namespace UI {

	WindowTitlebar::WindowTitlebar(Window* window) {
		m_Window = window;

		m_Wrapper = CreateComponent<HStack>();
		m_Wrapper->SetSizing(SizeMode::Fill, SizeMode::Auto);
		m_Wrapper->SetAlignment(AlignMode::Start, AlignMode::Center);
		m_Wrapper->SetStyle(State::Default, { .BackgroundColor = Color::Dark, .Padding = Edges(6.0f), .Gradient = GradientData(Color::Black(0.5,1), glm::vec2(0, 3), 0.2f, 400.0f)});
		m_Wrapper->SetSpacing(10.0f);

		// Callbacks
		m_Wrapper->SetCallback(Callback::OnPressStart, [&](Event event) {
			m_Window->StartDrag();
		});

		// Title
		m_Title = CreateComponent<Text>();
		m_Title->SetFontSize(24);
		m_Title->SetText("Unreal Engine");
		m_Title->SetSizing(SizeMode::Fill, SizeMode::Auto);
		m_Title->SetWrapMode(WrapMode::Elipsis);
		m_Title->SetDebug(true);
		
		// Icon
		m_Icon = CreateComponent<Image>();
		m_Icon->SetImage("unreal");
		m_Icon->SetSizing(SizeMode::Fixed, SizeMode::Fixed);
		m_Icon->SetFixedSize(32,32);
		m_Icon->SetStyle(State::Default, { .BackgroundColor = Color::White, .Border = BorderData(0.0f, Color::White(1.f), 0)});

		// Spacer
		m_Spacer = CreateComponent<Border>();
		m_Spacer->SetSizing(SizeMode::Fill, SizeMode::Auto);
		m_Spacer->SetAlignment(AlignMode::Center, AlignMode::Center);

		m_Wrapper->AddChild(m_Icon);
		m_Wrapper->AddChild(m_Title);
		m_Wrapper->AddChild(m_Spacer);

		for (auto i{ 0u }; i <= 2; i++) {
			auto button = CreateComponent<Image>();
			button->SetSizing(SizeMode::Fixed, SizeMode::Fixed);
			button->SetFixedSize(16,16);
			button->SetAlignment(AlignMode::Center, AlignMode::Center);
			button->SetStyle(State::Default, { .BackgroundColor = Color::White });

			switch (i) {
			case 0:
				button->SetImage("window_min");
				button->SetStyle(State::Hovered, { .BackgroundColor = Color::White(0.25f)});
				button->SetCallback(Callback::OnPressStart, [&](Event event) {
					m_Window->Minimise();
					});
				m_MinButton = std::move(button);
				m_Wrapper->AddChild(m_MinButton);
				break;
			case 1:
				button->SetImage("window_max");
				button->SetStyle(State::Hovered, { .BackgroundColor = Color::White(0.25f) , });
				button->SetCallback(Callback::OnPressStart, [&](Event event) {
					m_Window->Maximise();
					});
				m_MaxButton = std::move(button);
				m_Wrapper->AddChild(m_MaxButton);
				break;
			case 2:
				button->SetImage("window_close");
				button->SetStyle(State::Hovered, { .BackgroundColor = Color::Red });
				button->SetCallback(Callback::OnPressStart, [&](Event event) {
					m_Window->Close();
					});
				m_CloseButton = std::move(button);
				m_Wrapper->AddChild(m_CloseButton);
				break;
			}
		}

		m_Root = m_Wrapper.get();
	}

	void WindowTitlebar::SetAccentColor(ColorToken& color) {
		m_AccentColor = color;
		m_Wrapper->SetStyle(State::Default, { .BackgroundColor = Color::Dark, .Padding = Edges(4.0f), .Gradient = GradientData(m_AccentColor, glm::vec2(0, 0.5), 0.2f, 400.0f) });
	}

	void WindowTitlebar::SetTitle(const std::string& title)
	{
		if(m_Window) m_Window->SetWindowTitle(title);
		m_Title->SetText(title);
	}

	void WindowTitlebar::SetIconVisible(bool value) {
		m_Icon->SetVisibility(value == true ? Visibility::Visible : Visibility::Collapsed);
	}
}

