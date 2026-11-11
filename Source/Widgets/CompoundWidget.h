#pragma once
#include "Widget.h"
#include <memory>
#include <vector>

namespace UI {
	class CompoundWidget : public Widget {
	public:
		CompoundWidget() = default;

		// Base functions
		glm::vec2 Measure(float availableWidth, float availableHeight) override;
		void Layout(float x, float y) override;
		void Update(float deltaTime) override;
		void Render() override;
		Widget* HitTest(glm::vec2 position) override;

	protected:
		Widget* m_Root = nullptr;

		template<typename T, typename ...Args>
		std::shared_ptr<T> CreateComponent(Args&&... args) {
			auto component = std::make_shared<T>(std::forward<Args>(args)...);
			return component;
		}
	};
}
