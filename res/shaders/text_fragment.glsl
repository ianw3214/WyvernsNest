#version 330 core

in vec2 texCoord;

out vec4 colour;

uniform sampler2D textTexture;
uniform vec3 textColour;

void main() {
	colour = vec4(textColour, 1.0) * vec4(1.0, 1.0, 1.0, texture(textTexture, texCoord).r);
}