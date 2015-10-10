#include "mercury-program.h"

static GLuint m_program = 0;
static GLuint m_vertex = 0;
static GLuint m_fragment = 0;
static GLint u_mvp = -1;
static GLint u_m = -1;

void AttachShaderFromFile(GLuint *shader, const char *file, GLenum shaderType) {
	size_t size = 0;
#pragma warning(suppress:4996)
	FILE *f = fopen(file, "rb");
	if (!f)
	{
		printf("Could not open file: %s\n", file);
		return;
	}

	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);
	char *str = new char[size + 1];
	if (size != fread(str, sizeof(char), size, f))
	{
		delete [] str;
		printf("Error while reading file: %s\n", file);
		return;
	}
	fclose(f);
	str[size] = '\0';

	*shader = glCreateShader(shaderType);

	glShaderSource(*shader, 1, (const char **) &str, nullptr);
	delete [] str;

	GL_TRY(glCompileShader(*shader));

	// Check compilation log
	GLint status;
	GL_TRY(glGetShaderiv(*shader, GL_COMPILE_STATUS, &status));
	bool success = (status == GL_TRUE);
	if (success)
	{
		GL_TRY(glAttachShader(m_program, *shader));
	}
	else
	{
		GLint length = 0;
		GL_TRY(glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &length));
		if (length > 1)
		{
			char *c_log = new char[length];
			GL_TRY(glGetShaderInfoLog(*shader, length, nullptr, c_log));
			printf("%s\n", c_log);
			delete [] c_log;
		}
	}
}

bool FindProgramErrors(GLenum type)
{
	GLint status;
	GL_TRY(glGetProgramiv(m_program, type, &status));
	bool success = (status == GL_TRUE);
	if (!success)
	{
		GLint length = 0;
		GL_TRY(glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &length));

		if (length > 0)
		{
			char *c_log = new char[length];
			GL_TRY(glGetProgramInfoLog(m_program, length, nullptr, c_log));
			printf("%s\n", c_log);
			delete [] c_log;
		}

		return true;
	}

	return false;
}

void LoadProgram() {
	m_program = glCreateProgram();
	AttachShaderFromFile(&m_vertex, "../../assets/program.vert", GL_VERTEX_SHADER);
	AttachShaderFromFile(&m_fragment, "../../assets/program.frag", GL_FRAGMENT_SHADER);
	glLinkProgram(m_program);
	FindProgramErrors(GL_LINK_STATUS);
	glValidateProgram(m_program);
	FindProgramErrors(GL_VALIDATE_STATUS);

	// get uniforms
	u_mvp = glGetUniformLocation(m_program, "mvp_matrix");
	u_m = glGetUniformLocation(m_program, "m_matrix");
}

void BindProgram() {
	glUseProgram(m_program);
}

void UploadMVPMatrix(const mat4 &mat) {
	GL_TRY(glUniformMatrix4fv(u_mvp, 1, GL_FALSE, &mat.e[0]));
}

void UploadMMatrix(const mat4 &mat) {
	GL_TRY(glUniformMatrix4fv(u_m, 1, GL_FALSE, &mat.e[0]));
}
