#version 450 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;

uniform mat4 mvp_matrix;
uniform mat4 m_matrix;

out vec2 texcoord;
out vec3 normal;
//out vec3 worldPos;

void main(void) {
	gl_Position = mvp_matrix * vec4(Position, 1.0);

	texcoord = TexCoord;
	normal = (m_matrix * vec4(Normal, 0.0)).xyz;
	//worldPos = (m_matrix * vec4(Position, 1.0)).xyz;
}
