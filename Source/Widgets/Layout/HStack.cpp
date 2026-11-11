#include "HStack.h"

// Source
#include "../Common/Style.h"
#include "../Widget.h"

#include <algorithm>

namespace UI {

    glm::vec2 HStack::Measure(float availableWidth, float availableHeight) {

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
        if (m_Children.size() > 1) innerWidth += (m_Children.size() - 1) * m_Spacing;

        // Static children
        for (const auto& child : m_Children) {
            Sizing childSizing = child->GetSizing();

            // Skip axis filled children
            if (childSizing.width == SizeMode::Fill) {
                ++fillWidgets;
                continue;
            }

            // Measure static children and add to inner size
            glm::vec2 childSize = child->Measure(m_DesiredSize.x - padding.Horizontal(), m_DesiredSize.y - padding.Vertical());
            innerHeight = std::max(innerHeight, childSize.y);
            innerWidth += childSize.x;
        }

        // Fill axis children
        float fillWidgetSize = (m_DesiredSize.x - innerWidth - padding.Horizontal()) / std::max(1, fillWidgets);
        for (const auto& child : m_Children) {
            Sizing childSizing = child->GetSizing();

            // Skip if not fill sized
            bool fillW = childSizing.width == SizeMode::Fill;
            if (!fillW) continue;

            // Measure flex child and add to inner size
            glm::vec2 childSize = child->Measure((fillW ? fillWidgetSize : m_DesiredSize.x - padding.Horizontal()), m_DesiredSize.y - padding.Vertical());
            innerHeight = std::max(innerHeight, childSize.y);
            innerWidth += childSize.x;
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

    void HStack::Layout(float x, float y) {
        m_Position = { x, y };

        const Edges& currentPadding = m_CurrentStyle.Padding.value();
        float xOffset{ 0 };

        switch (m_Alignment.xAxis) {
        case AlignMode::End:
        case AlignMode::Start:
            for (const auto& child : m_Children) {
                glm::vec2 childSize = child->GetSize();

                float childX = x + CalculateAlignmentOffset(m_Alignment.xAxis, childSize.x, m_DesiredSize.x - currentPadding.Horizontal()) + currentPadding.Left + xOffset;
                float childY = y + CalculateAlignmentOffset(m_Alignment.yAxis, childSize.y, m_DesiredSize.y - currentPadding.Vertical()) + currentPadding.Top;

                child->Layout(childX, childY);

                xOffset += m_Alignment.xAxis == AlignMode::End ? -(childSize.x + m_Spacing) : childSize.x + m_Spacing;
            }
            break;
        case AlignMode::Center:
            // Calculate children size
            float childrenWidth{ 0 };
            for (const auto& child : m_Children) { childrenWidth += child->GetSize().x + m_Spacing; }

            // Layout children
            float beginX = x + CalculateAlignmentOffset(m_Alignment.xAxis, childrenWidth, m_DesiredSize.x - currentPadding.Horizontal()) + currentPadding.Left + xOffset;
            for (const auto& child : m_Children) {
                glm::vec2 childSize = child->GetSize();
                float childX = x + beginX + xOffset;
                float childY = y + CalculateAlignmentOffset(m_Alignment.yAxis, childSize.y, m_DesiredSize.y - currentPadding.Vertical()) + currentPadding.Top;
                child->Layout(childX, childY);
                xOffset += childSize.x + m_Spacing;
            }
            break;
        }
    }

    void HStack::Update(float deltaTime) {
        Widget::Update(deltaTime);
        for (const auto& child : m_Children) {
            child->Update(deltaTime);
        }
    }

    void HStack::Render() {
        Widget::Render();
        for (const auto& child : m_Children) {
            child->Render();
        }
    };

    void HStack::AddChild(std::shared_ptr<Widget> child) {
        child->SetParent(this);
        m_Children.push_back(child);
    };

    Widget* HStack::HitTest(glm::vec2 position) {
        if (!m_HitTestable) return nullptr;

        for (const auto& child : m_Children) {
            Widget* hit = child->HitTest(position);
            if (hit != nullptr) return hit;
        }

        if (Contains(position)) return this;
        return nullptr;
    }
}