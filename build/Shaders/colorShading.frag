#version 450

in vec2 fragmentUV;
in vec4 fragmentColor;

out vec4 color;

uniform sampler2D imageTexture;

void main() {	
    color = texture(imageTexture, fragmentUV) * fragmentColor;
}
