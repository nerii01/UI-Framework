#pragma once

// Source
#include "../Engine/Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>    
#include <unordered_map>
#include <functional>
#include "Common/Style.h"

namespace UI {
	enum class SizeMode { Fill, Fixed, Auto };
	enum class AlignMode {Start, Center, End};
	enum class State { Default = 0, Hovered = 1, Focused = 2, Pressed = 3 };
	enum class Callback { OnHoverBegin, OnHoverEnd, WhileHover, OnFocusBegin, OnFocusEnd, WhileFocus, OnPressStart, OnPressEnd, WhilePress, OnKeyDown, WhileKeyDown, OnKeyUp, OnScroll, WhileScroll};
	enum class Visibility {Visible, Hidden, Collapsed};

	struct Sizing {
		SizeMode width;
		SizeMode height;
	};

	struct Alignment {
		AlignMode xAxis;
		AlignMode yAxis;
	};

	struct Event {
		Callback EventType;

		// State
		State WidgetState;

		// On Key Press
		std::optional<int> key;

		// Scroll
		std::optional<glm::vec2> scroll;
	};

	class Widget {
		friend class InputManager;
	public:
		Widget() = default;
		virtual ~Widget() = default;

		// Core
		virtual glm::vec2 Measure(float availableWidth, float availableHeight);
		virtual void Render();
		virtual void Layout(float x, float y);
		virtual void Update(float deltaTime);
		virtual Widget* HitTest(glm::vec2 position);
		Widget& SetDebug(bool value);

		// Size and positioning
		Sizing GetSizing() const { return m_Sizing; };
		glm::vec2 GetSize() const { return m_DesiredSize; };
		glm::vec2 GetPosition() const { return m_Position; }

		// State
		bool IsHovered() const { return m_IsHovered || m_IsChildrenHovered; }

		bool IsFocused() const { return m_IsFocused || m_IsChildrenFocused; };

		bool IsPressed() const { return m_IsPressed; };

		// Display
		Widget& SetPosition(const glm::vec2& position) { m_Position = position; return *this; };
		Widget& SetVisibility(const Visibility& visibility) { m_Visibility = visibility; return *this; };
		Widget& SetFixedSize(float width, float heigh) { m_FixedSize = { width, heigh }; return *this; };
		Widget& SetSizing(SizeMode xAxis, SizeMode yAxis) { m_Sizing.width = xAxis; m_Sizing.height = yAxis; return*this; };
		Widget& SetStyle(State state, Style style);
		Widget& SetAlignment(AlignMode xAxis, AlignMode yAxis) { m_Alignment = { xAxis, yAxis };return *this; }
		
		// Parent system
		void SetParent(Widget* parent) { m_Parent = parent; };
		Widget* GetParent() const { return m_Parent; };
		Widget* GetRoot() {
			Widget* current = this;
			while (Widget* parent = current->GetParent()) { current = parent; };
			return current;
		};

		// Hit detection
		Widget& SetHitTestable(bool value);

		// Interaction
		Widget& SetCallback(Callback callback, std::function<void(Event)> callbackFunction);

	protected:

		


		// Parent
		Widget* m_Parent = nullptr;

		// Hit detection
		virtual bool Contains(glm::vec2 position);

		// Widget State
		
	
		std::vector<State> m_ActiveStates{ State::Default };
		void RefreshState();
		bool m_HitTestable{ true };

		bool m_IsPressed{ false };

		bool m_IsHovered{ false };
		bool m_IsChildrenHovered{ false };
		void SetChildrenHovered(bool value) { m_IsChildrenHovered = value; }
		
		bool m_IsFocused{ false };
		bool m_IsChildrenFocused{ false };
		void SetChildrenFocused(bool value) { m_IsChildrenFocused = value; };

		// Interaction
		bool m_Interactable{ true };
		std::map<Callback, std::function<void(Event)>> m_Callbacks;

		// Base interactions
		void UpdateCursor();
		void RunCallback(Callback callback, Event event);
			
		// Core interaction functions
		void OnHover();
		void OnHoverLeave();
		void OnFocus();
		void OnFocusLost();
		
		// Bool -> Handled?
		bool OnPress(int button);
		bool OnKeyPress(int key);
		bool OnScrollInput(glm::vec2 scroll);

		void OnRelease(int button);
		void OnKeyRelease(int key);

		// Non bubble hooks
		virtual void HandleHover() {}
		virtual void HandleHoverLeave() {}
		virtual void HandleFocus() {}
		virtual void HandleFocusLost() {}

		// Bubble hooks
		virtual bool HandlePress(int button) { return false; }
		virtual bool HandleKeyPress(int key) { return false; }
		virtual bool HandleScrollInput(glm::vec2 scroll) { return false; }

		// Debug
		bool m_Debug{ false };

		// Display
		virtual void InterpolateStyles(float deltaTime);
		std::unordered_map<State, Style> m_Styles{ {State::Default, Style::Default() } };
		Style GetCombinedStyle();
		Style m_CurrentStyle = m_Styles[State::Default];
		Sizing m_Sizing = { SizeMode::Auto, SizeMode::Auto };
		Alignment m_Alignment = { AlignMode::Start, AlignMode::Start };
		Visibility m_Visibility = Visibility::Visible;
		
		virtual float CalculateAlignmentOffset(AlignMode alignMode, float childSizeOnAxis, float availableSize);
		float m_StyleTransition = 24.0f;
		glm::vec2 m_DesiredSize{ 0.0f, 0.0f };
		glm::vec2 m_FixedSize = { 128, 128 };
		glm::vec2 m_Position{ 0.0f, 0.0f };
	};

}


