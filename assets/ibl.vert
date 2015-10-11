#version 450 core

layout (location = 0) in vec2 Position;

out vec2 Uvs;

void main(void) {
	// [0 ... 1]
	gl_Position = vec4(Position.xy, 0.0, 1.0);

	Uvs = Position;
}
