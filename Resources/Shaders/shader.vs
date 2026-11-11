#version 330 core

layout (location = 0) in vec2 aPos;        
layout (location = 1) in vec4 aColor;    
layout (location = 2) in vec2 aTexCoord; 

uniform mat4 u_Projection; 
uniform mat4 u_Model;    
uniform vec4 u_UVRect; 
uniform int u_RenderMode;  

out vec4 vColor;    
out vec2 vTexCoord; 

void main() {
    gl_Position = u_Projection * u_Model * vec4(aPos, 0.0, 1.0);
    vColor = aColor;

     // Rect
    if (u_RenderMode == 0) {
        vTexCoord = aTexCoord;
        return;
    }

    // Text
    if (u_RenderMode == 1) {
        vTexCoord = vec2(
            mix(u_UVRect.x, u_UVRect.z, aTexCoord.x),
            mix(u_UVRect.y, u_UVRect.w, aTexCoord.y)
        );
        return;
    } 
}