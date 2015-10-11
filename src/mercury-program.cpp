#include "mercury-program.h"

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

void _Finalize(GLuint program) {
	glLinkProgram(program);
	_FindProgramErrors(program, GL_LINK_STATUS);
	glValidateProgram(program);
	_FindProgramErrors(program, GL_VALIDATE_STATUS);
}

/************************************************************************/
/* Common stuff                                                         */
/************************************************************************/

static struct {
	GLuint program = 0;
	GLuint vertex = 0;
	GLuint fragment = 0;
	GLint u_mvp = -1;
	GLint u_m = -1;
} s_geometryProgram;

static struct {
	GLuint program = 0;
	GLuint vertex = 0;
	GLuint fragment = 0;
	GLint u_invView = -1;
	GLint u_invProj = -1;
} s_iblProgram;

void LoadPrograms() {
	{
		s_geometryProgram.program = glCreateProgram();
		s_geometryProgram.vertex = _AttachShaderFromFile(s_geometryProgram.program, "../../assets/program.vert", GL_VERTEX_SHADER);
		s_geometryProgram.fragment = _AttachShaderFromFile(s_geometryProgram.program, "../../assets/program.frag", GL_FRAGMENT_SHADER);
		_Finalize(s_geometryProgram.program);

		// get uniforms
		s_geometryProgram.u_mvp = glGetUniformLocation(s_geometryProgram.program, "mvp_matrix");
		s_geometryProgram.u_m = glGetUniformLocation(s_geometryProgram.program, "m_matrix");
	}

	{
		s_iblProgram.program = glCreateProgram();
		s_iblProgram.vertex = _AttachShaderFromFile(s_iblProgram.program, "../../assets/ibl.vert", GL_VERTEX_SHADER);
		s_iblProgram.fragment = _AttachShaderFromFile(s_iblProgram.program, "../../assets/ibl.frag", GL_FRAGMENT_SHADER);
		_Finalize(s_iblProgram.program);

		// get uniforms
		s_iblProgram.u_invView = glGetUniformLocation(s_iblProgram.program, "invView");
		s_iblProgram.u_invProj = glGetUniformLocation(s_iblProgram.program, "invProj");
	}
}

/************************************************************************/
/* Geometry program                                                     */
/************************************************************************/

void BindGeometryProgram() {
	glUseProgram(s_geometryProgram.program);
}

void SetGeometryProgramConstants(const mat4 &mvp, const mat4 &m) {
	GL_TRY(glUniformMatrix4fv(s_geometryProgram.u_mvp, 1, GL_FALSE, &mvp.e[0]));
	GL_TRY(glUniformMatrix4fv(s_geometryProgram.u_m, 1, GL_FALSE, &m.e[0]));
}

/************************************************************************/
/* Image-based lighting program                                         */
/************************************************************************/

void BindIBLProgram() {
	glUseProgram(s_iblProgram.program);
}

void SetIBLProgramConstants(const mat4 &invView, const mat4 &invProj) {
	GL_TRY(glUniformMatrix4fv(s_iblProgram.u_invView, 1, GL_FALSE, &invView.e[0]));
	GL_TRY(glUniformMatrix4fv(s_iblProgram.u_invProj, 1, GL_FALSE, &invProj.e[0]));
}
