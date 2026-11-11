#pragma once

// Source
#include "../Widget.h"

namespace UI {
	class ScrollBox : public Widget{
	public:
		ScrollBox() = default;

		// Base functions
		glm::vec2 Measure(float availableWidth, float availableHeight) override;
		void Layout(float x, float y) override;
		void Update(float deltaTime) override;
		void Render() override;
		Widget* HitTest(glm::vec2 position) override;
		

		// Child 
		ScrollBox& SetChild(std::shared_ptr<Widget> child);


		// Scroll
		void ScrollToBottom();
		void SetScrollPercentage(float percentage);

	protected:
		float m_ScrollTarget{ 0 };
		float m_ScrollCurrent{ 0 };
		float m_ScrollDelta{ 0 };
		float m_BarScale{ 0 };

		float m_ChildSize{ 0 };

		bool m_ShouldScrollToBottom{ false };

		std::shared_ptr<Widget> m_Child;
		bool HandleScrollInput(glm::vec2 scroll) override;
	};
}
