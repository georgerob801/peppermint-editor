#version 330 core

in vec2 uv;
out vec4 fragColour;

uniform sampler2D text;
uniform vec3 vertexColour;

uniform vec3 tintColour;
uniform bool outlineMode;
uniform vec3 outlineColour;
uniform float outlineAlpha;

void main() {
	if (outlineMode) {
		fragColour = vec4(outlineColour, outlineAlpha);
		return;
	}

	vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(text, uv).r);
	fragColour = vec4(vertexColour * tintColour, 1.0f) * sampled;
}