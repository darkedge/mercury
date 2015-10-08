#version 450 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;

uniform mat4 mvp_matrix;

out vec2 texcoord;
out vec3 normal;

void main(void) {
	gl_Position = mvp_matrix * vec4(Position, 1.0f);
	
	normal = Normal;
	texcoord = TexCoord;
}
