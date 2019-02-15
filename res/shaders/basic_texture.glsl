#version 330 core

in vec2 vTexCoord;

out vec4 colour;

uniform sampler2D u_Texture;

void main() {
	vec4 texColour = texture(u_Texture, vTexCoord);
	colour = texColour;
}
