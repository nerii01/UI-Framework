#include "Image.h"
#include "../../Managers/ImageManager.h"

namespace UI {

	glm::vec2 Image::Measure(float availableWidth, float availableHeight) {
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

		m_DesiredSize.x = std::max(0.0f, m_DesiredSize.x);
		m_DesiredSize.y = std::max(0.0f, m_DesiredSize.y);

		return m_DesiredSize;
	}

	void Image::Layout(float x, float y) {
		Widget::Layout(x, y);
	}

	void Image::Update(float deltaTime) {
		Widget::Update(deltaTime);
	}

	void Image::Render() {
		if (m_Visibility != Visibility::Visible) return;
		Renderer::Get().DrawRect(m_Position, m_DesiredSize, m_CurrentStyle, false, m_ImagePath);
	};


	void Image::SetImage(const std::string& path) {
		m_ImagePath = path;
	}

	Widget* Image::HitTest(glm::vec2 position) {
		if (!m_HitTestable) return nullptr;

		if (Contains(position)) return this;
		return nullptr;
	}
}
