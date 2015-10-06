#pragma once

GLuint m_program;
GLuint m_vertex;
GLuint m_fragment;

int8_t *ReadFile(FILE *file) {
	// TODO
	return nullptr;
}

void LoadProgram() {
	GLuint bla;
	GLuint *shader = &bla;
	const char *file = "../program.vert";
	int32_t size = 0;
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



	*shader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(*shader, 1, (const char **) &str, nullptr);
	delete [] str;

	glCompileShader(*shader);

	// Check compilation log
	GLint status;
	glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
	bool success = (status == GL_TRUE);
	if (success) {
		glAttachShader(m_program, *shader);
	} else {
		GLint length = 0;
		glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &length);
		if (length > 1)
		{
			char *c_log = new char[length];
			glGetShaderInfoLog(*shader, length, nullptr, c_log);
			printf("%s\n", c_log);
			delete [] c_log;
		}
	}
}