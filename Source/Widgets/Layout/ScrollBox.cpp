#include "ScrollBox.h"
#include "../../Engine/Time.h"
#include "../../Engine/Window.h"
#include "../../Managers/WidgetManager.h"

namespace UI {

	glm::vec2 ScrollBox::Measure(float availableWidth, float availableHeight) {
		const Edges currentPadding = m_CurrentStyle.Padding.value_or(Edges(0.0f));

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
		glm::vec2 childSize = glm::vec2(0.0f);
		if (m_Child != nullptr) childSize = m_Child->Measure(std::max(0.0f, m_DesiredSize.x - currentPadding.Horizontal()), std::max(0.0f, m_DesiredSize.y - currentPadding.Vertical()));
		if (m_Sizing.width == SizeMode::Auto) m_DesiredSize.x = childSize.x + currentPadding.Horizontal();
		if (m_Sizing.height == SizeMode::Auto) m_DesiredSize.y = childSize.y + currentPadding.Vertical();

		m_ChildSize = m_Child != nullptr ? childSize.y + currentPadding.Vertical() : 0;
		
		HandleScrollInput(glm::vec2(0.0f));

		return m_DesiredSize;
	}

	void ScrollBox::Layout(float x, float y) {
		Widget::Layout(x, y);

		if (m_Child == nullptr) return;
		const Edges& currentPadding = m_CurrentStyle.Padding.value();
		glm::vec2 childSize = m_Child->GetSize();
		float childX = x + currentPadding.Left;
		float childY = y - m_ScrollCurrent + currentPadding.Top;

		m_Child->Layout(childX, childY);
	}

	void ScrollBox::Update(float deltaTime) {
		Widget::Update(deltaTime);

		if (m_Child != nullptr) m_Child->Update(deltaTime);

		float lastScroll = m_ScrollCurrent;

		m_ScrollCurrent = glm::mix(m_ScrollCurrent, m_ScrollTarget, Time::GetDeltaTime() * 24.0f);
		m_ScrollDelta = std::abs(m_ScrollCurrent - lastScroll) / 10;
		m_BarScale = std::min(1.0f, m_DesiredSize.y / m_ChildSize);
		
		if (m_ShouldScrollToBottom) {
			m_ShouldScrollToBottom = false;
			SetScrollPercentage(1.0f);
		}

	}

	void ScrollBox::Render() {
		Widget::Render();

		Edges padding = m_CurrentStyle.Padding.value_or(Edges(0.0f));
		glEnable(GL_SCISSOR_TEST);

		auto windowSize = WidgetManager::Get().GetWindow(this)->GetWindowSize();
		
		int scissorY = static_cast<int>(windowSize.y - (m_Position.y + m_DesiredSize.y - padding.Top));

		glScissor(
			static_cast<int>(m_Position.x + padding.Left),
			scissorY,
			static_cast<int>(m_DesiredSize.x - padding.Horizontal()),
			static_cast<int>(m_DesiredSize.y - padding.Vertical())
		);

		m_Child->Render();

		glDisable(GL_SCISSOR_TEST);


		if (IsHovered() && m_BarScale < 0.99f) {
			Style testStyle = m_CurrentStyle;
			float scrollPercent = m_ScrollCurrent / (std::max(0.0001f, m_ChildSize - m_DesiredSize.y)) ;
		
			glm::vec4 color = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);

			float barWidth = 10.0f;
			//float barHeight = std::lerp(65.f, 100.f, std::clamp(m_ScrollDelta, 0.0f, 1.0f));
			float barHeight = std::max(10.0f, (m_DesiredSize.y - 10) * m_BarScale);
			glm::vec2 barPos{ m_Position.x + m_DesiredSize.x - barWidth - 5, std::lerp(m_Position.y + 5, (m_Position.y) + (m_DesiredSize.y - 5) - barHeight, scrollPercent) };
			glm::vec2 barSize{ barWidth,  barHeight };
			Style barStyle{ .BackgroundColor = color, .Border = BorderData(1.0f, glm::vec4(1,1,1,0.6f), 12.0f)};
			testStyle = m_CurrentStyle;
			Renderer::Get().DrawRect(barPos, barSize, testStyle.Merge(barStyle));

		}
	}

	ScrollBox& ScrollBox::SetChild(std::shared_ptr<Widget> child) {
		m_Child = child;
		m_Child->SetParent(this);
		return *this;
	}
	void ScrollBox::ScrollToBottom()
	{
		m_ShouldScrollToBottom = true;
	}
	void ScrollBox::SetScrollPercentage(float percentage)
	{

		float maxScrollY = std::max(0.0f, m_ChildSize - m_DesiredSize.y) ;
		float minScrollY = 0.0f;

		m_ScrollTarget = std::lerp(minScrollY,maxScrollY, std::clamp(percentage,0.0f,1.0f));
	}
	;

	Widget* ScrollBox::HitTest(glm::vec2 position) {
		if (!m_HitTestable) return nullptr;

		if (m_Child != nullptr && Contains(position)) {
			Widget* hit = m_Child->HitTest(position);
			if (hit != nullptr) return hit;
		}

		if (Contains(position)) return this;

		return nullptr;
	}

	bool ScrollBox::HandleScrollInput(glm::vec2 scroll) {
		
		m_ScrollTarget += static_cast<float>(scroll.y) * -40;

		// Scroll positive -> down
		float maxScrollY = std::max(0.0f, m_ChildSize - m_DesiredSize.y) ;
		float minScrollY = 0.0f;


		m_ScrollTarget = std::clamp(m_ScrollTarget, minScrollY, maxScrollY);

		return true;
	}

}
