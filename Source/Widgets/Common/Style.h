#pragma once

#include <optional>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>    
#include "../../Engine/Logger.h"

namespace UI {
	struct Edges {
		float Top = 0.0f;
		float Right = 0.0f;
		float Bottom = 0.0f;
		float Left = 0.0f;

		Edges() = default;
		explicit Edges(float all) : Top(all), Right(all), Left(all), Bottom(all) {}
		explicit Edges(float horizontal, float vertical) : Top(vertical), Bottom(vertical), Left(horizontal), Right(horizontal) {}
		float Horizontal() const { return Left + Right; };
		float Vertical() const { return Top + Bottom; };

		bool operator==(const Edges& other) const {
			return Top == other.Top &&
				Right == other.Right &&
				Bottom == other.Bottom &&
				Left == other.Left;
		}

		Edges& Interpolate(const Edges& target, float t) {
			Top = glm::mix(Top, target.Top, t);
			Right = glm::mix(Right, target.Right, t);
			Bottom = glm::mix(Bottom, target.Bottom, t);
			Left = glm::mix(Left, target.Left, t);
			return *this;
		}
	};

	struct GradientData {
		GradientData(glm::vec4 color, glm::vec2 position, float hardness, float radius) : Color(color), Position(position), Hardness(hardness), Radius(radius) {};
		
		glm::vec4 Color{ glm::vec4(0.0f)};
		glm::vec2 Position{ glm::vec2(0.0f) };
		float Hardness{ 0.0 };
		float Radius{ 0.0 };

		bool operator==(const GradientData& other) const {
			if (this == &other) return true;
			bool ColorEqual = glm::all(glm::epsilonEqual(Color, other.Color, 0.01f));
			bool PositionEqual = glm::all(glm::epsilonEqual(Position, other.Position, 0.01f));
			bool HardnessEqual = glm::epsilonEqual(Hardness, other.Hardness, 0.01f);
			bool RadiusEqual = glm::epsilonEqual(Radius, other.Radius, 0.01f);
			return ColorEqual && PositionEqual && HardnessEqual && RadiusEqual;
		}

		GradientData& Interpolate(const GradientData& target, float t) {
			Color = glm::mix(Color, target.Color, t);
			Position = glm::mix(Position, target.Position, t);
			Hardness = glm::mix(Hardness, target.Hardness, t);
			Radius = glm::mix(Radius, target.Radius, t);
			return *this;
		}
	};

	struct BorderData {
		BorderData(float width, glm::vec4 color, float radius) : Width(width), Color(color), Radius(radius) {};

		float Width{ 0 };
		glm::vec4 Color{ glm::vec4(0.0f) };
		float Radius{ 0.0f };

		bool operator==(const BorderData& other) const {
			if (this == &other) return true;
			bool ColorEqual = glm::all(glm::epsilonEqual(Color, other.Color, 0.01f));
			bool WidthEqual = Width == other.Width;
			bool RadiusEqual = glm::epsilonEqual(Radius, other.Radius, 0.01f);
			return WidthEqual && ColorEqual && RadiusEqual;
		}

		BorderData& Interpolate(const BorderData& target, float t) {
			Color = glm::mix(Color, target.Color, t);
			Width = glm::mix(Width, target.Width, t);
			Radius = glm::mix(Radius, target.Radius, t);
			return *this;
		}

	};

	struct Style {

		// Default style - DON'T MIND THE ORDER
		static Style Default() {
			Style style;
			style.BackgroundColor = default_BackgroundColor;
			style.Color = default_Color;
			style.Padding = default_Padding;
			style.Gradient = default_Gradient;
			style.Scale = default_Scale;
			style.Border = default_Border;
			return style;
		}

		// Comparasion
		bool operator==(const Style& other) const {
			if (this == &other) return true;

			auto vec4Equal = [](const std::optional<glm::vec4>& a, const std::optional<glm::vec4>& b) {
				if (a.has_value() != b.has_value()) return false;
				if (!a.has_value()) return true; 
				return glm::all(glm::epsilonEqual(a.value(), b.value(), 0.01f));
				};

			auto floatEqual = [](const std::optional<float>& a, const std::optional<float>& b) {
				if (a.has_value() != b.has_value()) return false;
				if (!a.has_value()) return true;
				return glm::epsilonEqual(a.value(), b.value(), 0.01f);
				};

			return vec4Equal(BackgroundColor, other.BackgroundColor) &&
				vec4Equal(Color, other.Color) &&
				floatEqual(Scale, other.Scale) &&
				(Padding == other.Padding) &&   
				(Gradient == other.Gradient) && (Border == other.Border); 
		}

		// Merge
		Style& Merge(const Style& overlay) {
			if (overlay.BackgroundColor.has_value()) BackgroundColor = overlay.BackgroundColor;
			if (overlay.Color.has_value())           Color = overlay.Color;
			if (overlay.Border.has_value())          Border = overlay.Border;
			if (overlay.Padding.has_value())         Padding = overlay.Padding;
			if (overlay.Gradient.has_value())        Gradient = overlay.Gradient;
			if (overlay.Scale.has_value())           Scale = overlay.Scale;
			return *this;
		}

		// Interpolate
		Style& Interpolate(const Style& overlay, float transition) {
			if (overlay.BackgroundColor.has_value()) {
				glm::vec4 startColor = BackgroundColor.value_or(default_BackgroundColor);
				glm::vec4 endColor = overlay.BackgroundColor.value();

				// Jeśli startowy kolor był w pełni przezroczysty (a == 0),
				// używamy RGB koloru docelowego, aby zapobiec "czernieniu" podczas przejścia.
				if (startColor.a == 0.0f) {
					startColor = glm::vec4(endColor.r, endColor.g, endColor.b, 0.0f);
				}
				// Analogicznie w drugą stronę (z koloru do przezroczystości)
				else if (endColor.a == 0.0f) {
					endColor = glm::vec4(startColor.r, startColor.g, startColor.b, 0.0f);
				}

				BackgroundColor = glm::mix(startColor, endColor, transition);
			}

			if (overlay.Color.has_value()) {
				Color = glm::mix(Color.value_or(default_Color), overlay.Color.value(), transition);
			}

			if (overlay.Padding.has_value()) {
				Edges startPadding = Padding.value_or(default_Padding);
				Padding = startPadding.Interpolate(overlay.Padding.value(), transition);
			}

			if (overlay.Scale.has_value()) {
				Scale = glm::mix(Scale.value_or(default_Scale), overlay.Scale.value(), transition);
			}

			if (overlay.Gradient.has_value()) {
				GradientData startGradient = Gradient.value_or(default_Gradient);
				Gradient = startGradient.Interpolate(overlay.Gradient.value(), transition);
			}

			if (overlay.Border.has_value()) {
				BorderData startBorder = Border.value_or(default_Border);
				Border = startBorder.Interpolate(overlay.Border.value(), transition);
			}

			return *this;
		}

		// Initialization order 
		std::optional<glm::vec4> BackgroundColor;
		std::optional<glm::vec4> Color;
		std::optional<BorderData> Border;
		std::optional<Edges> Padding;
		std::optional<GradientData> Gradient;
		std::optional<float> Scale;

		inline static const glm::vec4 default_BackgroundColor{ 0.0f, 0.0f, 0.0f, 0.0f };
		inline static const glm::vec4 default_Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		inline static const BorderData default_Border{ 0, glm::vec4(0.0f), 2.0f };
		inline static const Edges default_Padding{ 0.0f };
		inline static const GradientData default_Gradient{ glm::vec4(0.0f), glm::vec2(0.0f), 0.0f, 0.0f };
		inline static const float default_Scale{ 1.0f };
	};
}
