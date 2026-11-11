#version 330 core

layout (location = 0) out vec4 FragColor;

in vec4 vColor;   
in vec2 vTexCoord; 

// Libs
float hash(vec2 p) {
    return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453);
}

float BorderSDF(vec2 centerPos, vec2 halfSize, float radius) {
    float r = min(radius, min(halfSize.x, halfSize.y));
    vec2 q = abs(centerPos) - halfSize + r;
    return length(max(q, 0.0)) + min(max(q.x, q.y), 0.0) - r;
}

uniform sampler2D u_FontAtlas;
uniform sampler2D u_Image;

// Data
struct GradientParams {
    vec4 Color;
    vec2 Position;
    float Hardness;
    float Radius;
};

struct NoiseParams {
    float Amount;
};

struct GlobalParams{
    // vec2 CursorPosition;  
    float Time;
};

struct WidgetParams {
    vec2 Size;
    vec2 Position; 
    float Scale;
};

struct TextParams {
    vec4 TextColor;
};

struct RectParams {
    vec4 BackgroundColor;
    float BorderWidth;
    float BorderRadius;
    vec4 BorderColor;
    vec2 BorderHighlight;
    bool HasImage;
};

// Params
uniform WidgetParams u_Widget;
uniform GlobalParams u_Global;
uniform GradientParams u_Gradient;
uniform NoiseParams u_Noise;
uniform int u_RenderMode; 
uniform TextParams u_Text;
uniform RectParams u_Rect;

void main() {

    // Rect
      if (u_RenderMode == 0) {
        GradientParams gradient = u_Gradient;
        GlobalParams global = u_Global;
        WidgetParams widget = u_Widget;
        RectParams rect = u_Rect;
        NoiseParams noise = u_Noise;

        float borderAlpha = smoothstep(0.0, 1.0, rect.BorderWidth);
        rect.BorderColor *= borderAlpha;
        rect.BorderWidth = clamp(rect.BorderWidth, 0.0,100.0);
        rect.BorderRadius = clamp(rect.BorderRadius, 2.0,100.0);

        // Used Later
        vec2 halfSize = vec2(u_Widget.Size) / 2;
        vec2 centerRelativePixelPos = (vTexCoord -0.5) * widget.Size;
        vec2 relativePixelPos= vTexCoord.xy * widget.Size;

        // Position and distance
        vec2 wordPixelPos = vec2(vTexCoord.x, vTexCoord.y) * widget.Size + widget.Position;
        vec2 relativeGradientPos = widget.Position + (gradient.Position) * widget.Size;
        float distanceToMouse = distance(wordPixelPos, relativeGradientPos) / gradient.Radius;
       
        // Gradient - ADD GRADIENTS[] , RADIAL AND LINEAR
        vec4 fillColor;
        if(gradient.Color.a != 0.0) {
            float gradientHardness = pow(gradient.Hardness + 1, (gradient.Hardness * 10));
            float gradientMask = clamp(pow(distanceToMouse, gradientHardness),0,1);
            vec4 gradientColor = vec4(vec3(1-gradientMask), 1) * gradient.Color;
            fillColor = mix(gradientColor, rect.BackgroundColor, gradientMask);
        } else {
            fillColor = rect.BackgroundColor;
        }

        // Noise
        vec2 grainCoord = gl_FragCoord.xy * 0.01337;
        float grain = clamp((hash(grainCoord) - 0.5) * (noise.Amount * 0.015),0,1);
        fillColor.rgb += grain;

        // Border radius
        float finalBorderWidth = clamp(rect.BorderWidth, 1.0, 1000.0) + 0.5;
        float dist = BorderSDF(centerRelativePixelPos, halfSize, rect.BorderRadius);
        float aa = fwidth(dist);
        float edgeAlpha = smoothstep(finalBorderWidth - aa, -finalBorderWidth, dist) - smoothstep(-aa, 0.0, dist);
        if (edgeAlpha <= 0.0) discard;

        // Border
        float borderMask = 0.0;
        borderMask = smoothstep(-finalBorderWidth - aa, -finalBorderWidth, dist) - smoothstep(-aa, 0.0, dist);
        borderMask = clamp(borderMask, 0.0, 1.0);
       
        // vec2 cursorRel =( rect.BorderHighlight - (widget.Position + halfSize))/ 200;
        // vec2 mirroredursorRel =( rect.BorderHighlight - (widget.Position + halfSize))/ 200;

        // Final    
        vec4 finalColor = mix(fillColor, rect.BorderColor, borderMask * rect.BorderColor.a);
        vec4 image;

        if(rect.HasImage) {
            image = texture(u_Image, vTexCoord);
        } else {
            image = vec4(1.0);
        }

        FragColor = vec4(finalColor.rgb, finalColor.a * edgeAlpha) * image;
        // FragColor = vec4(vec3(dist), 1);
        return; 
    } 

    // Text
    if (u_RenderMode == 1) {
        // Text
        float gamma = 1.15;
        float distance = texture(u_FontAtlas, vTexCoord, -0.5).r; 
        float smoothing = clamp(fwidth(distance), 0.03, 0.5);
        float alpha = smoothstep(0.5 - smoothing, 0.5 + smoothing, distance);
        alpha = pow(alpha, 1 / gamma);
        

        // Final
        FragColor = vec4(vec3(distance), 1);
        FragColor = vec4(u_Text.TextColor.rgb, u_Text.TextColor.a * alpha );
        return;
    } 
}