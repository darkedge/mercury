#pragma once
GLFWwindow * GetWindow();
float GetDeltaTime();
int32_t GetWindowWidth();
int32_t GetWindowHeight();

#define GL_ERROR_CASE( err )\
	case err:\
	str = #err;\
	break;

const char *mjGlErrorString(GLenum err)
{
	const char *str = "";
	switch (err)
	{
		GL_ERROR_CASE(GL_NO_ERROR);
		GL_ERROR_CASE(GL_INVALID_ENUM);
		GL_ERROR_CASE(GL_INVALID_VALUE);
		GL_ERROR_CASE(GL_INVALID_OPERATION);
		GL_ERROR_CASE(GL_INVALID_FRAMEBUFFER_OPERATION);
		GL_ERROR_CASE(GL_OUT_OF_MEMORY);
		GL_ERROR_CASE(GL_STACK_UNDERFLOW);
		GL_ERROR_CASE(GL_STACK_OVERFLOW);
	}
	return str;
}

#define GL_TRY( expr )\
{\
	GLenum __err;\
	do\
	{\
		__err = glGetError();\
		if ( __err != GL_NO_ERROR )\
		{\
			const char *__str = mjGlErrorString( __err );\
			printf("%s generated before executing %s:\n\tAt: %s:%d\n", __str, #expr, __FILE__, __LINE__ );\
		}\
	} while( __err != GL_NO_ERROR );\
	expr;\
	do\
	{\
		__err = glGetError();\
		if ( __err != GL_NO_ERROR )\
		{\
			const char *__str = mjGlErrorString( __err );\
			printf( "OpenGL error: %s\n\tBy: %s\n\tAt: %s:%d\n", __str, #expr, __FILE__, __LINE__ );\
		}\
	} while ( __err != GL_NO_ERROR );\
}
