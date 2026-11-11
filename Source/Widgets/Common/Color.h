#pragma once
#include <glm/glm.hpp>

namespace UI {
    struct ColorToken {
        glm::vec4 Value;

        operator glm::vec4() const { return Value; }

        glm::vec4 operator()(float opacity = 1.0f, float brightness = 1.0f) const {
            return glm::vec4(Value.r * brightness,  Value.g * brightness, Value.b * brightness, Value.a * opacity);
        }
    };

    namespace Color {
		// Base
		inline constexpr ColorToken Red{ {0.957f, 0.263f, 0.212f, 1.0f} };
		inline constexpr ColorToken Pink{ {0.914f, 0.118f, 0.388f, 1.0f} };
		inline constexpr ColorToken Purple{ {0.612f, 0.153f, 0.690f, 1.0f} };
		inline constexpr ColorToken DeepPurple{ {0.404f, 0.227f, 0.718f, 1.0f} };
		inline constexpr ColorToken Indigo{ {0.247f, 0.318f, 0.710f, 1.0f} };
		inline constexpr ColorToken Blue{ {0.129f, 0.588f, 0.953f, 1.0f} };
		inline constexpr ColorToken LightBlue{ {0.012f, 0.663f, 0.957f, 1.0f} };
		inline constexpr ColorToken Cyan{ {0.000f, 0.737f, 0.831f, 1.0f} };
		inline constexpr ColorToken Teal{ {0.000f, 0.588f, 0.533f, 1.0f} };
		inline constexpr ColorToken Green{ {0.298f, 0.686f, 0.314f, 1.0f} };
		inline constexpr ColorToken LightGreen{ {0.545f, 0.765f, 0.290f, 1.0f} };
		inline constexpr ColorToken Lime{ {0.804f, 0.863f, 0.224f, 1.0f} };
		inline constexpr ColorToken Yellow{ {1.000f, 0.922f, 0.231f, 1.0f} };
		inline constexpr ColorToken Amber{ {1.000f, 0.757f, 0.027f, 1.0f} };
		inline constexpr ColorToken Orange{ {1.000f, 0.596f, 0.000f, 1.0f} };
		inline constexpr ColorToken DeepOrange{ {1.000f, 0.341f, 0.133f, 1.0f} };
		inline constexpr ColorToken Brown{ {0.475f, 0.333f, 0.282f, 1.0f} };
		inline constexpr ColorToken Grey{ {0.620f, 0.620f, 0.620f, 1.0f} };
		inline constexpr ColorToken BlueGrey{ {0.376f, 0.490f, 0.545f, 1.0f} };

		// Name
		inline constexpr ColorToken Tomato{ {1.000f, 0.388f, 0.278f, 1.0f} };
		inline constexpr ColorToken Coral{ {1.000f, 0.498f, 0.314f, 1.0f} };
		inline constexpr ColorToken Salmon{ {0.980f, 0.502f, 0.447f, 1.0f} };
		inline constexpr ColorToken LightSalmon{ {1.000f, 0.627f, 0.478f, 1.0f} };
		inline constexpr ColorToken Crimson{ {0.863f, 0.078f, 0.235f, 1.0f} };
		inline constexpr ColorToken Maroon{ {0.502f, 0.000f, 0.000f, 1.0f} };
		inline constexpr ColorToken FireBrick{ {0.698f, 0.133f, 0.133f, 1.0f} };
		inline constexpr ColorToken IndianRed{ {0.804f, 0.361f, 0.361f, 1.0f} };
		inline constexpr ColorToken HotPink{ {1.000f, 0.412f, 0.706f, 1.0f} };
		inline constexpr ColorToken DeepPink{ {1.000f, 0.078f, 0.576f, 1.0f} };
		inline constexpr ColorToken Orchid{ {0.855f, 0.439f, 0.839f, 1.0f} };
		inline constexpr ColorToken Violet{ {0.933f, 0.510f, 0.933f, 1.0f} };
		inline constexpr ColorToken Plum{ {0.867f, 0.627f, 0.867f, 1.0f} };
		inline constexpr ColorToken Magenta{ {1.000f, 0.000f, 1.000f, 1.0f} };
		inline constexpr ColorToken MediumPurple{ {0.576f, 0.439f, 0.859f, 1.0f} };
		inline constexpr ColorToken SlateBlue{ {0.416f, 0.353f, 0.804f, 1.0f} };
		inline constexpr ColorToken RoyalBlue{ {0.255f, 0.412f, 0.882f, 1.0f} };
		inline constexpr ColorToken Navy{ {0.000f, 0.000f, 0.502f, 1.0f} };
		inline constexpr ColorToken MidnightBlue{ {0.098f, 0.098f, 0.439f, 1.0f} };
		inline constexpr ColorToken DodgerBlue{ {0.118f, 0.565f, 1.000f, 1.0f} };
		inline constexpr ColorToken SkyBlue{ {0.529f, 0.808f, 0.922f, 1.0f} };
		inline constexpr ColorToken SteelBlue{ {0.275f, 0.510f, 0.706f, 1.0f} };
		inline constexpr ColorToken Turquoise{ {0.251f, 0.878f, 0.816f, 1.0f} };
		inline constexpr ColorToken Aquamarine{ {0.498f, 1.000f, 0.831f, 1.0f} };
		inline constexpr ColorToken SeaGreen{ {0.180f, 0.545f, 0.341f, 1.0f} };
		inline constexpr ColorToken ForestGreen{ {0.133f, 0.545f, 0.133f, 1.0f} };
		inline constexpr ColorToken OliveDrab{ {0.420f, 0.557f, 0.137f, 1.0f} };
		inline constexpr ColorToken Olive{ {0.502f, 0.502f, 0.000f, 1.0f} };
		inline constexpr ColorToken DarkKhaki{ {0.741f, 0.718f, 0.420f, 1.0f} };
		inline constexpr ColorToken Gold{ {1.000f, 0.843f, 0.000f, 1.0f} };
		inline constexpr ColorToken Goldenrod{ {0.855f, 0.647f, 0.125f, 1.0f} };
		inline constexpr ColorToken Chocolate{ {0.824f, 0.412f, 0.118f, 1.0f} };
		inline constexpr ColorToken Peru{ {0.804f, 0.522f, 0.247f, 1.0f} };
		inline constexpr ColorToken SaddleBrown{ {0.545f, 0.271f, 0.075f, 1.0f} };
		inline constexpr ColorToken Sienna{ {0.627f, 0.322f, 0.176f, 1.0f} };
		inline constexpr ColorToken Beige{ {0.961f, 0.961f, 0.863f, 1.0f} };
		inline constexpr ColorToken Ivory{ {1.000f, 1.000f, 0.941f, 1.0f} };
		inline constexpr ColorToken Wheat{ {0.961f, 0.871f, 0.702f, 1.0f} };
		inline constexpr ColorToken Tan{ {0.824f, 0.706f, 0.549f, 1.0f} };
		inline constexpr ColorToken Khaki{ {0.941f, 0.902f, 0.549f, 1.0f} };
		inline constexpr ColorToken Lavender{ {0.902f, 0.902f, 0.980f, 1.0f} };
		inline constexpr ColorToken Thistle{ {0.847f, 0.749f, 0.847f, 1.0f} };
		inline constexpr ColorToken MistyRose{ {1.000f, 0.894f, 0.882f, 1.0f} };
		inline constexpr ColorToken PeachPuff{ {1.000f, 0.855f, 0.725f, 1.0f} };
		inline constexpr ColorToken LightCoral{ {0.941f, 0.502f, 0.502f, 1.0f} };
		inline constexpr ColorToken RosyBrown{ {0.737f, 0.561f, 0.561f, 1.0f} };
		inline constexpr ColorToken Gainsboro{ {0.863f, 0.863f, 0.863f, 1.0f} };
		inline constexpr ColorToken WhiteSmoke{ {0.961f, 0.961f, 0.961f, 1.0f} };
		inline constexpr ColorToken Snow{ {1.000f, 0.980f, 0.980f, 1.0f} };
		inline constexpr ColorToken Linen{ {0.980f, 0.941f, 0.902f, 1.0f} };
		inline constexpr ColorToken AliceBlue{ {0.941f, 0.973f, 1.000f, 1.0f} };
		inline constexpr ColorToken GhostWhite{ {0.973f, 0.973f, 1.000f, 1.0f} };
		inline constexpr ColorToken Honeydew{ {0.941f, 1.000f, 0.941f, 1.0f} };
		inline constexpr ColorToken MintCream{ {0.961f, 1.000f, 0.980f, 1.0f} };
		inline constexpr ColorToken Azure{ {0.941f, 1.000f, 1.000f, 1.0f} };
		inline constexpr ColorToken LavenderBlush{ {1.000f, 0.941f, 0.961f, 1.0f} };
		inline constexpr ColorToken OldLace{ {0.992f, 0.961f, 0.902f, 1.0f} };
		inline constexpr ColorToken Cornsilk{ {1.000f, 0.973f, 0.863f, 1.0f} };
		inline constexpr ColorToken LemonChiffon{ {1.000f, 0.980f, 0.804f, 1.0f} };
		inline constexpr ColorToken SlateGray{ {0.439f, 0.502f, 0.565f, 1.0f} };
		inline constexpr ColorToken DarkSlateGray{ {0.184f, 0.310f, 0.310f, 1.0f} };
		inline constexpr ColorToken DimGray{ {0.412f, 0.412f, 0.412f, 1.0f} };
		inline constexpr ColorToken LightSlateGray{ {0.467f, 0.533f, 0.600f, 1.0f} };
		inline constexpr ColorToken Charcoal{ {0.212f, 0.271f, 0.310f, 1.0f} };
		inline constexpr ColorToken Jet{ {0.204f, 0.204f, 0.204f, 1.0f} };
		inline constexpr ColorToken Onyx{ {0.208f, 0.220f, 0.224f, 1.0f} };
		inline constexpr ColorToken Emerald{ {0.314f, 0.784f, 0.471f, 1.0f} };
		inline constexpr ColorToken Jade{ {0.000f, 0.659f, 0.420f, 1.0f} };
		inline constexpr ColorToken Mint{ {0.243f, 0.706f, 0.537f, 1.0f} };
		inline constexpr ColorToken Sage{ {0.612f, 0.686f, 0.533f, 1.0f} };
		inline constexpr ColorToken Moss{ {0.541f, 0.604f, 0.357f, 1.0f} };
		inline constexpr ColorToken Cobalt{ {0.000f, 0.278f, 0.671f, 1.0f} };
		inline constexpr ColorToken Sapphire{ {0.059f, 0.322f, 0.729f, 1.0f} };
		inline constexpr ColorToken Cerulean{ {0.000f, 0.482f, 0.655f, 1.0f} };
		inline constexpr ColorToken Periwinkle{ {0.800f, 0.800f, 1.000f, 1.0f} };
		inline constexpr ColorToken Lilac{ {0.784f, 0.635f, 0.784f, 1.0f} };
		inline constexpr ColorToken Mauve{ {0.878f, 0.690f, 1.000f, 1.0f} };
		inline constexpr ColorToken Fuchsia{ {1.000f, 0.000f, 1.000f, 1.0f} };
		inline constexpr ColorToken Rose{ {1.000f, 0.000f, 0.498f, 1.0f} };
		inline constexpr ColorToken Ruby{ {0.878f, 0.067f, 0.373f, 1.0f} };
		inline constexpr ColorToken Wine{ {0.447f, 0.184f, 0.216f, 1.0f} };
		inline constexpr ColorToken Rust{ {0.718f, 0.255f, 0.055f, 1.0f} };
		inline constexpr ColorToken Copper{ {0.722f, 0.451f, 0.200f, 1.0f} };
		inline constexpr ColorToken Bronze{ {0.804f, 0.498f, 0.196f, 1.0f} };
		inline constexpr ColorToken Brass{ {0.710f, 0.651f, 0.259f, 1.0f} };
		inline constexpr ColorToken Ochre{ {0.800f, 0.467f, 0.133f, 1.0f} };
		inline constexpr ColorToken Terracotta{ {0.886f, 0.447f, 0.357f, 1.0f} };
		inline constexpr ColorToken Cream{ {1.000f, 0.992f, 0.816f, 1.0f} };
		inline constexpr ColorToken Vanilla{ {0.953f, 0.898f, 0.671f, 1.0f} };
		inline constexpr ColorToken Pearl{ {0.918f, 0.878f, 0.784f, 1.0f} };
		inline constexpr ColorToken Slate{ {0.392f, 0.455f, 0.545f, 1.0f} };

		// UI
		inline constexpr ColorToken Success{ {0.180f, 0.800f, 0.443f, 1.0f} };
		inline constexpr ColorToken Warning{ {0.945f, 0.769f, 0.059f, 1.0f} };
		inline constexpr ColorToken Danger{ {0.906f, 0.298f, 0.235f, 1.0f} };
		inline constexpr ColorToken Info{ {0.204f, 0.596f, 0.859f, 1.0f} };
		inline constexpr ColorToken Primary{ {0.345f, 0.396f, 0.949f, 1.0f} };
		inline constexpr ColorToken Secondary{ {0.600f, 0.667f, 0.710f, 1.0f} };
		inline constexpr ColorToken Accent{ {1.000f, 0.420f, 0.420f, 1.0f} };
		inline constexpr ColorToken Background{ {0.118f, 0.122f, 0.133f, 1.0f} };
		inline constexpr ColorToken Surface{ {0.169f, 0.176f, 0.192f, 1.0f} };
		inline constexpr ColorToken SurfaceAlt{ {0.192f, 0.200f, 0.220f, 1.0f} };
		inline constexpr ColorToken Border{ {0.247f, 0.255f, 0.278f, 1.0f} };
		inline constexpr ColorToken TextPrimary{ {0.949f, 0.953f, 0.961f, 1.0f} };
		inline constexpr ColorToken TextSecondary{ {0.710f, 0.729f, 0.757f, 1.0f} };
		inline constexpr ColorToken Disabled{ {0.427f, 0.435f, 0.471f, 1.0f} };

		// Base
		inline constexpr ColorToken White{ {1.0f, 1.0f, 1.0f, 1.0f} };
		inline constexpr ColorToken Black{ {0.0f, 0.0f, 0.0f, 1.0f} };
		inline constexpr ColorToken Transparent{ {0.0f, 0.0f, 0.0f, 0.0f} };
		inline constexpr ColorToken Dark{ {0.12f, 0.12f, 0.14f, 1.0f} };
		inline constexpr ColorToken DarkSurface{ {0.18f, 0.18f, 0.20f, 1.0f} };
		inline constexpr ColorToken Gray{ {0.5f, 0.5f, 0.5f, 1.0f} };

    }
}