#include "Border.h"

namespace UI {

		glm::vec2 Border::Measure(float availableWidth, float availableHeight)  {
			if (m_Visibility == Visibility::Collapsed) return { 0,0 };
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

			m_DesiredSize.x = std::max(0.0f, m_DesiredSize.x);
			m_DesiredSize.y = std::max(0.0f, m_DesiredSize.y);

			return m_DesiredSize;
		}

		void Border::Layout(float x, float y)  {
			Widget::Layout(x, y);

			if (m_Child == nullptr) return;
			const Edges& currentPadding = m_CurrentStyle.Padding.value();
			glm::vec2 childSize = m_Child->GetSize();
			float childX = x + CalculateAlignmentOffset(m_Alignment.xAxis, childSize.x, m_DesiredSize.x - currentPadding.Horizontal()) + currentPadding.Left;
			float childY = y + CalculateAlignmentOffset(m_Alignment.yAxis, childSize.y, m_DesiredSize.y - currentPadding.Vertical()) + currentPadding.Top;

			m_Child->Layout(childX, childY);
		}

		void Border::Update(float deltaTime) {
			Widget::Update(deltaTime);

			if (m_Child != nullptr) m_Child->Update(deltaTime);
		}

		void Border::Render()  {
			if (m_Visibility != Visibility::Visible) return;
			Widget::Render();

			if (m_Child != nullptr) m_Child->Render();
		};

		Border& Border::SetChild(std::shared_ptr<Widget> child) {
			m_Child = child; 
			m_Child->SetParent(this);
			return *this;
		};

		//void Border::RemoveChild

		Widget* Border::HitTest(glm::vec2 position)  {
			if (!m_HitTestable) return nullptr;

			if (m_Child != nullptr) {
				Widget* hit = m_Child->HitTest(position);
				if (hit != nullptr) return hit;
			}

			if (Contains(position)) return this;
			return nullptr;
		}
}
