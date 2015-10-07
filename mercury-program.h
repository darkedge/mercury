#pragma once

GLuint m_program;
GLuint m_vertex;
GLuint m_fragment;

char *ReadFile(FILE *file) {
	fseek(file, 0, SEEK_END);
	int32_t size = ftell(file);
	fseek(file, 0, SEEK_SET);
	char *str = new char[size + 1];
	if (size != fread(str, sizeof(char), size, file))
	{
		delete [] str;
		printf("Error while reading file\n");
		return nullptr;
	}
	str[size] = '\0';
	return str;
}

void LoadProgram() {
	const char *path = "../program.vert";
#pragma warning(suppress:4996)
	FILE *file = fopen(path, "rb");
	if (!file)
	{
		printf("Could not open file: %s\n", path);
		return;
	}
	char *str = ReadFile(file);
	
	fclose(file);

	GLuint shader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(shader, 1, (const char **) &str, nullptr);
	delete [] str;

	glCompileShader(shader);

	// Check compilation log
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	bool success = (status == GL_TRUE);
	if (success) {
		glAttachShader(m_program, shader);
	} else {
		GLint length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		if (length > 1)
		{
			char *c_log = new char[length];
			glGetShaderInfoLog(shader, length, nullptr, c_log);
			printf("%s\n", c_log);
			delete [] c_log;
		}
	}
}