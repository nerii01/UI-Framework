#pragma once

// Source
#include "../Widget.h"

namespace UI {

	class HStack : public Widget {
	public:

		glm::vec2 Measure(float availableWidth, float availableHeight) override;
		void Layout(float x, float y) override;
		void Update(float deltaTime) override;
		void Render() override;
		void AddChild(std::shared_ptr<Widget> child);

		Widget& SetSpacing(float value) { m_Spacing = value; return *this; }
		Widget* HitTest(glm::vec2 position) override;

	protected:
		float m_Spacing = 8.0f;
		std::vector<std::shared_ptr<Widget>> m_Children{};
	};

};