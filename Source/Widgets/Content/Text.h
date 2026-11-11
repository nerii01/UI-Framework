#pragma once

// Source
#include "../Widget.h"
#include "../../Managers/FontManager.h"

#include <algorithm>

namespace UI {
	class Text : public Widget {
	public:
		Text() = default;
		Text(const std::string& fontName, const std::string& text, unsigned int fontSize = 26, WrapMode wrapMode = WrapMode::Word)
			: m_FontSize(fontSize), m_FontName(fontName), m_Text(text), m_WrapMode(wrapMode) {};

		// Base functions
		glm::vec2 Measure(float availableWidth, float availableHeight) override;
		void Layout(float x, float y) override;
		void Update(float deltaTime) override;
		void Render() override;

		// Getters
		std::string GetText() { return m_Text; };

		// Text setup
		Text& SetText(const std::string& text) { m_Text = text; return *this; };
		Text& SetWrapMode(WrapMode wrapmode) { m_WrapMode = wrapmode;return *this;  };
		Text& SetFontSize(unsigned int size) { m_FontSize = std::min(24u,size); return *this; };
		Text& SetFontName(const std::string& font) { m_FontName = font; return *this; };

	protected:
		// Data
		unsigned int m_FontSize = 24;
		std::string m_FontName = "InterSemibold";
		std::string m_Text{ "" };

		// Wrap
		WrapMode m_WrapMode = WrapMode::Word;
	};
}
