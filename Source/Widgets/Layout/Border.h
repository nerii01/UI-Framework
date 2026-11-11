#pragma once

// Source
#include "../Widget.h"

namespace UI {
	class Border : public Widget {
		public:
			Border() = default;
			Border(std::shared_ptr<Widget> child) : m_Child(std::move(child)) { };

			// Base functions
			glm::vec2 Measure(float availableWidth, float availableHeight) override;
			void Layout(float x, float y) override;
			void Update(float deltaTime) override;
			void Render() override;
			Widget* HitTest(glm::vec2 position) override;
			
			// Border Setup
			Border& SetChild(std::shared_ptr<Widget> child);


		protected:
			std::shared_ptr<Widget> m_Child;
	};
}
