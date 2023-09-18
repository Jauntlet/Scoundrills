#version 450

in vec2 fragmentUV;
in vec4 fragmentColor;

out vec4 color;

uniform sampler2D imageTexture;
//uniform vec4 fontColor;

void main() {	
    vec4 sampledColor = texture(imageTexture, fragmentUV);
    color = vec4(fragmentColor.rgb, sampledColor.r * fragmentColor.a);
}
