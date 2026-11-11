#pragma once

#include "../Widget.h"
#include "../CompoundWidget.h"
#include <string>
#include <memory>

// Source
#include "../../Engine/Window.h"
#include "../Layout/HStack.h"
#include "../Layout/Border.h"
#include "../Content/Text.h"
#include "../Content/Image.h"
#include "../Common/Color.h"


namespace UI {
	class WindowTitlebar : public CompoundWidget {
	public:

		WindowTitlebar(Window* window);
 
		void SetTitle(const std::string& title);
		void SetIconVisible(bool value);
		void SetAccentColor(ColorToken& color);
		std::string GetTitle() {
			return m_Title->GetText();
		}

	private:
		// Window
		Window* m_Window;

		// Colors
		ColorToken m_AccentColor = Color::DeepPurple;

		// Widgets
		std::shared_ptr<HStack> m_Wrapper;
		std::shared_ptr<Text> m_Title;
		std::shared_ptr<Image> m_Icon;
		std::shared_ptr<Border> m_Spacer;

		// Buttons
		std::shared_ptr<Image> m_MinButton;
		std::shared_ptr<Image> m_MaxButton;
		std::shared_ptr<Image> m_CloseButton;
	};
}