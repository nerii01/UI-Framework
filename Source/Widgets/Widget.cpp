
#include "Widget.h"

// Source
#include "../Engine/Logger.h"
#include <GLFW/glfw3.h>
#include "Common/Style.h"

namespace UI {

	// Base functions
	Widget& Widget::SetDebug(bool value) { m_Debug = value; return *this; }
	glm::vec2 Widget::Measure(float availableWidth, float availableHeight) {
		if (m_Visibility == Visibility::Collapsed) return { 0,0 };
		m_DesiredSize.x = (m_Sizing.width == SizeMode::Fill) ? availableWidth : std::max(0.0f, m_FixedSize.x);
		m_DesiredSize.y = (m_Sizing.height == SizeMode::Fill) ? availableHeight : std::max(0.0f, m_FixedSize.y);
		return m_DesiredSize;
	}
	void Widget::Layout(float x, float y) {
		m_Position = { x  ,y };
	}
	void Widget::Update(float deltaTime) {
		UpdateCursor();
		RefreshState();
		InterpolateStyles(deltaTime);
	}
	void Widget::Render() {
		if (m_Visibility != Visibility::Visible) return;
		Renderer::Get().DrawRect(m_Position, m_DesiredSize, m_CurrentStyle, m_Debug);
	};
	
	// Display
	Style Widget::GetCombinedStyle() {
		Style combinedStyle = m_Styles[State::Default];

		for (State state : m_ActiveStates) {
			auto it = m_Styles.find(state);
			if (it == m_Styles.end()) continue;
			combinedStyle.Merge(it->second);
		}

		return combinedStyle;
	}
	void Widget::InterpolateStyles(float deltaTime) {
		const Style target = GetCombinedStyle();

		if (target == m_CurrentStyle) {
			//Logger::Get().PrintToConsole("SAME STYLES", LogDebug);
			return;
		}
		float clampedDelta = std::min(deltaTime, 0.05f);

		m_CurrentStyle.Interpolate(target, m_StyleTransition * clampedDelta);
	}

	// Aligment
	float Widget::CalculateAlignmentOffset(AlignMode alignMode, float childSizeOnAxis, float availableSize ) {
		float offset{0};

		switch (alignMode) {
		case AlignMode::Start: offset = 0; break;
		case AlignMode::Center: offset = (availableSize / 2) - (childSizeOnAxis / 2);  break;
		case AlignMode::End: offset = availableSize - childSizeOnAxis; break;
		}

		return offset;
	}

	Widget& Widget::SetStyle(State state, Style style) {
		if (m_Styles.find(state) != m_Styles.end()) {
			m_Styles[state].Merge(style);
			return *this;
		}

		m_Styles[state] = style;
		return *this;
	}

	// Hit detection
	Widget& Widget::SetHitTestable(bool value) { m_HitTestable = value; return*this; }
	Widget* Widget::HitTest(glm::vec2 position) {
		if (!m_HitTestable) return nullptr;
		if (Contains(position)) return this;
		return nullptr;
	}
	bool Widget::Contains(glm::vec2 position) {
		return (position.x >= m_Position.x && position.x <= m_Position.x + m_DesiredSize.x &&
			position.y >= m_Position.y && position.y <= m_Position.y + m_DesiredSize.y);
	}

	// Interactions
	void Widget::UpdateCursor()
	{
		// Not quite sure of the implemnentation - TODO
		// if (m_IsHovered) {
		//	SetCursor(LoadCursor(NULL, IDC_HAND));
		// }
	}
	
	// Callbacks
	Widget& Widget::SetCallback(Callback callback, std::function<void(Event)> callbackFunction) { m_Callbacks[callback] = std::move(callbackFunction);return *this; }
	void Widget::RunCallback(Callback callback, Event event) {
		Event callbackEvent = std::move(event);
		event.EventType = callback;
		//event.WidgetState = m_CurrentState;

		// Callback
		auto it = m_Callbacks.find(callback);
		if (it != m_Callbacks.end()) it->second(callbackEvent);
	}
	
	// Non bubbling events
	void Widget::OnHover() {
		m_IsHovered = true;
		RunCallback(Callback::OnHoverBegin, {});
	}
	void Widget::OnHoverLeave() {
		m_IsHovered = false;
		RunCallback(Callback::OnHoverEnd, {});
	}
	void Widget::OnFocus() {
		m_IsFocused = true;
		RunCallback(Callback::OnFocusBegin, {});
	}
	void Widget::OnFocusLost() {
		m_IsFocused = false;
		RunCallback(Callback::OnFocusEnd, {});
	}
	
	// Bubbling Events
	bool Widget::OnPress(int button) {
		if (!m_Interactable) return false;
		
		bool hasCallback = m_Callbacks.contains(Callback::OnPressStart) ;
		RunCallback(Callback::OnPressStart, { .key = button });
		
		bool handled =  HandlePress(button) || hasCallback;
		if(handled && button == 0) m_IsPressed = true;

		return handled;
	}

	bool Widget::OnKeyPress(int key) {
		if (!m_Interactable) return false;

		bool hasCallback = m_Callbacks.contains(Callback::OnKeyDown);
		RunCallback(Callback::OnKeyDown, { .key = key });

		return HandleKeyPress(key) || hasCallback;
	}

	bool Widget::OnScrollInput(glm::vec2 scroll) {
		if (!m_Interactable) return false;

		bool hasCallback = m_Callbacks.contains(Callback::OnScroll);
		RunCallback(Callback::OnScroll, { .scroll = scroll });

		return HandleScrollInput(scroll) || hasCallback;
	};
	
	// Release 
	void Widget::OnRelease(int button) {
		m_IsPressed = false;
		RunCallback(Callback::OnPressEnd, { .key = button });
	}

	void Widget::OnKeyRelease(int key) {
		RunCallback(Callback::OnKeyUp, { .key = key });
	}
	
	// States
	void Widget::RefreshState()
	{
		std::vector<State> newStates{};
		newStates.push_back(State::Default);
		if (IsHovered()) { newStates.push_back(State::Hovered); }
		if (IsFocused()) { newStates.push_back(State::Focused); }
		if (IsPressed()) { newStates.push_back(State::Pressed); }

		if (newStates == m_ActiveStates) return;
		m_ActiveStates = newStates;
	}
}

