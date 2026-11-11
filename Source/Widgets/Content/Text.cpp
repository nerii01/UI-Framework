#include "Text.h"


namespace UI {

	glm::vec2 Text::Measure(float availableWidth, float availableHeight) {
		if (m_Visibility == Visibility::Collapsed) return { 0,0 };

		// First pass 
		switch (m_Sizing.width) {
		case SizeMode::Fixed: m_DesiredSize.x = m_FixedSize.x; break;
		case SizeMode::Auto: m_DesiredSize.x = availableWidth; break;
		case SizeMode::Fill: m_DesiredSize.x = availableWidth; break;
		}

		switch (m_Sizing.height) {
		case SizeMode::Fixed: m_DesiredSize.y = m_FixedSize.y; break;
		case SizeMode::Auto: m_DesiredSize.y = availableHeight; break;
		case SizeMode::Fill: m_DesiredSize.y = availableHeight; break;
		}

		// Second pass for Auto
		glm::vec2 textMeasuredSize = FontManager::Get().MeasureText(m_Text, m_FontName, m_FontSize, m_DesiredSize.x, m_WrapMode);
		if (m_Sizing.width == SizeMode::Auto) m_DesiredSize.x = textMeasuredSize.x;
		if (m_Sizing.height == SizeMode::Auto) m_DesiredSize.y = textMeasuredSize.y;

		return m_DesiredSize;
	}

	void Text::Layout(float x, float y) {
		Widget::Layout(x, y);
	}

	void Text::Update(float deltaTime) {
		Widget::Update(deltaTime);

		glm::vec4 color = m_CurrentStyle.Color.value();
		//Logger::Get().PrintToConsole("Text: {}, Color: {},{},{}", LogInfo, m_Text, color.r, color.g, color.b, color.a);
	}

	void Text::Render() {
		if (m_Visibility != Visibility::Visible) return
		Widget::Render();
		Renderer::Get().DrawText(m_FontName, m_Text, m_Position, m_CurrentStyle.Color.value(), m_FontSize, m_DesiredSize.x, m_WrapMode);
	}

}
