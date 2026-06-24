#version 330 core

in vec3 Color;
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D TexSlot;  
uniform vec4 pColor;

void main() {
    // FragColor = pColor;
    FragColor = texture(TexSlot, vec2(TexCoord.x, TexCoord.y));
}