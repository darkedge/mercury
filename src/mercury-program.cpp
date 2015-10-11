#include "mercury-program.h"

static struct GeometryProgram {
	GLuint program = 0;
	GLuint vertex = 0;
	GLuint fragment = 0;
	GLint u_mvp = -1;
	GLint u_m = -1;
} s_geometryProgram;

GLuint _AttachShaderFromFile(GLuint program, const char *file, GLenum shaderType) {
	size_t size = 0;
#pragma warning(suppress:4996)
	FILE *f = fopen(file, "rb");
	if (!f)
	{
		printf("Could not open file: %s\n", file);
		return 0;
	}

	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);
	char *str = new char[size + 1];
	if (size != fread(str, sizeof(char), size, f))
	{
		delete [] str;
		printf("Error while reading file: %s\n", file);
		return 0;
	}
	fclose(f);
	str[size] = '\0';

	GLuint shader = glCreateShader(shaderType);

	glShaderSource(shader, 1, (const char **) &str, nullptr);
	delete [] str;

	GL_TRY(glCompileShader(shader));

	// Check compilation log
	GLint status;
	GL_TRY(glGetShaderiv(shader, GL_COMPILE_STATUS, &status));
	if (status == GL_TRUE)
	{
		GL_TRY(glAttachShader(program, shader));
	}
	else
	{
		GLint length = 0;
		GL_TRY(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));
		if (length > 1)
		{
			char *c_log = new char[length];
			GL_TRY(glGetShaderInfoLog(shader, length, nullptr, c_log));
			printf("%s\n", c_log);
			delete [] c_log;
		}
		return 0;
	}

	return shader;
}

bool _FindProgramErrors(GLuint program, GLenum type)
{
	GLint status;
	GL_TRY(glGetProgramiv(program, type, &status));
	bool success = (status == GL_TRUE);
	if (!success)
	{
		GLint length = 0;
		GL_TRY(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));

		if (length > 0)
		{
			char *c_log = new char[length];
			GL_TRY(glGetProgramInfoLog(program, length, nullptr, c_log));
			printf("%s\n", c_log);
			delete [] c_log;
		}

		return true;
	}

	return false;
}

void LoadProgram() {
	s_geometryProgram.program = glCreateProgram();
	s_geometryProgram.vertex = _AttachShaderFromFile(s_geometryProgram.program, "../../assets/program.vert", GL_VERTEX_SHADER);
	s_geometryProgram.fragment = _AttachShaderFromFile(s_geometryProgram.program, "../../assets/program.frag", GL_FRAGMENT_SHADER);
	glLinkProgram(s_geometryProgram.program);
	_FindProgramErrors(s_geometryProgram.program, GL_LINK_STATUS);
	glValidateProgram(s_geometryProgram.program);
	_FindProgramErrors(s_geometryProgram.program, GL_VALIDATE_STATUS);

	// get uniforms
	s_geometryProgram.u_mvp = glGetUniformLocation(s_geometryProgram.program, "mvp_matrix");
	s_geometryProgram.u_m = glGetUniformLocation(s_geometryProgram.program, "m_matrix");
}

void BindProgram() {
	glUseProgram(s_geometryProgram.program);
}

void UploadMVPMatrix(const mat4 &mat) {
	GL_TRY(glUniformMatrix4fv(s_geometryProgram.u_mvp, 1, GL_FALSE, &mat.e[0]));
}

void UploadMMatrix(const mat4 &mat) {
	GL_TRY(glUniformMatrix4fv(s_geometryProgram.u_m, 1, GL_FALSE, &mat.e[0]));
}
