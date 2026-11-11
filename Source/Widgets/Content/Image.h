#pragma once

// Source
#include "../Widget.h"

namespace UI {
	class Image : public Widget {
	public:
		Image() = default;

		// Base functions
		glm::vec2 Measure(float availableWidth, float availableHeight) override;
		void Layout(float x, float y) override;
		void Update(float deltaTime) override;
		void Render() override;
		Widget* HitTest(glm::vec2 position) override;

		// Image Setup
		void SetImage(const std::string& path);

	protected:
		std::string m_ImagePath{""};
	};
}
