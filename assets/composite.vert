#version 450 core

layout (location = 0) in vec2 Position;

out vec2 texcoord;

void main(void) {
	// [-1 ... 1]
	gl_Position = vec4(Position.xy, 0.0, 1.0);
	// Map to [0 ... 1]
	texcoord = Position * 0.5 + 0.5;
}
