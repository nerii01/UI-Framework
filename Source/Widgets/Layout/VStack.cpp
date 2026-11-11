#include "VStack.h"

// Source
#include "../Common/Style.h"
#include "../Widget.h"

#include <algorithm>

namespace UI {

    glm::vec2 VStack::Measure(float availableWidth, float availableHeight) {

        // Initial measure
        switch (m_Sizing.width) {
        case SizeMode::Fixed: m_DesiredSize.x = m_FixedSize.x; break;
        case SizeMode::Fill:  m_DesiredSize.x = availableWidth; break;
        case SizeMode::Auto:  m_DesiredSize.x = availableWidth; break;
        }
        switch (m_Sizing.height) {
        case SizeMode::Fixed: m_DesiredSize.y = m_FixedSize.y; break;
        case SizeMode::Fill:  m_DesiredSize.y = availableHeight; break;
        case SizeMode::Auto:  m_DesiredSize.y = availableHeight; break;
        }
		
		// Skip measurements for not auto containers
		//if (!(m_Sizing.height == SizeMode::Auto || m_Sizing.width == SizeMode::Auto)) return { m_DesiredSize };

        Edges padding = m_CurrentStyle.Padding.value_or(Edges(0.0f));
        float innerWidth = 0.0f;
        float innerHeight = 0.0f;
		int fillWidgets = 0;

        // Spacing
        if (m_Children.size() > 1) innerHeight += (m_Children.size() - 1) * m_Spacing;
		
		// Static children
		for (const auto& child : m_Children) {
			Sizing childSizing = child->GetSizing();
			
			// Skip axis filled children
			if (childSizing.height == SizeMode::Fill) {
				++fillWidgets;
				continue;
			}

			// Measure static children and add to inner size
			glm::vec2 childSize = child->Measure(m_DesiredSize.x - padding.Horizontal(), m_DesiredSize.y - padding.Vertical());
			innerWidth = std::max(innerWidth, childSize.x);
			innerHeight += childSize.y;
		}

		// Fill axis children
		float fillWidgetHeight = (m_DesiredSize.y - innerHeight - padding.Vertical()) / std::max(1,fillWidgets);
		for (const auto& child : m_Children) {
			Sizing childSizing = child->GetSizing();
			
			// Skip if not fill sized
			bool fillH = childSizing.height == SizeMode::Fill;
			if (!fillH) continue;

			// Measure flex child and add to inner size
			glm::vec2 childSize = child->Measure(m_DesiredSize.x - padding.Horizontal(), (fillH ? fillWidgetHeight : m_DesiredSize.y - padding.Vertical()) );
			innerWidth = std::max(innerWidth, childSize.x);
			innerHeight += childSize.y;
		}

		// Apply auto measurements
		switch (m_Sizing.width) {
		case SizeMode::Auto:  m_DesiredSize.x = innerWidth + padding.Horizontal(); break;
		}
		switch (m_Sizing.height) {
		case SizeMode::Auto:  m_DesiredSize.y = innerHeight + padding.Vertical(); break;
		}
		
        return m_DesiredSize;
    }


    // Add allignment
	void VStack::Layout(float x, float y) {
		m_Position = { x,y };

		const Edges& currentPadding = m_CurrentStyle.Padding.value();

		float childY = y + currentPadding.Top;
		float childX = x + currentPadding.Left;

		for (const auto& child : m_Children) {
			glm::vec2 childSize = child->GetSize();
			child->Layout(childX, childY);
			childY += childSize.y + m_Spacing;
		}
	}

	void VStack::Update(float deltaTime) {
		Widget::Update(deltaTime);
		for (const auto& child : m_Children) {
			child->Update(deltaTime);
		}
	}

	void VStack::Render() {
		Widget::Render();
		for (const auto& child : m_Children) {
			child->Render();
		}
	};

	void VStack::AddChild(std::shared_ptr<Widget> child) { 
		child->SetParent(this);
		m_Children.push_back(child); 
	};

	Widget* VStack::HitTest(glm::vec2 position) {
		if (!m_HitTestable) return nullptr;

		for (const auto& child : m_Children) {
			Widget* hit = child->HitTest(position);
			if (hit != nullptr) return hit;
		}

		if (Contains(position)) return this;
		return nullptr;
	}
}