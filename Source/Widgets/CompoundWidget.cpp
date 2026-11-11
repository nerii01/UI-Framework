#include "CompoundWidget.h"

namespace UI {

	glm::vec2 CompoundWidget::Measure(float availableWidth, float availableHeight) {
		m_DesiredSize = m_Root ? m_Root->Measure(availableWidth, availableHeight) : glm::vec2(0,0);
		return m_DesiredSize;
	}

	void CompoundWidget::Layout(float x, float y) {
		if (m_Root != nullptr) m_Root->Layout(x, y);
	}

	void CompoundWidget::Update(float deltaTime) {
		if (m_Root != nullptr) m_Root->Update(deltaTime);
	}

	void CompoundWidget::Render() {
		if (m_Visibility != Visibility::Visible) return;
		if (m_Root != nullptr) m_Root->Render();
	};

	Widget* CompoundWidget::HitTest(glm::vec2 position) {
		if (!m_HitTestable) return nullptr;

		if (m_Root != nullptr) {
			Widget* hit = m_Root->HitTest(position);
			if (hit != nullptr) return hit;
		}

		if (Contains(position)) return this;
		return nullptr;
	}
}
