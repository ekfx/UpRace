#version 330 core

in vec3 Color;
in vec2 TexCoord;
out vec4 FragColor;

//uniform sampler2D texture0;  

void main() {
    FragColor = vec4(1.0);
    //FragColor = texture(texture0, vec2(TexCoord.x, TexCoord.y));
}