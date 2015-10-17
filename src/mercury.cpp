#include <codeanalysis/warnings.h>
#pragma warning( push, 0 )
  #pragma warning( disable : ALL_CODE_ANALYSIS_WARNINGS )
  
  #include "glad/glad.h"
  #include "glad.c"
  
  #include "GLFW/glfw3.h"
  
  #define STB_IMAGE_IMPLEMENTATION
  #include "stb_image.h"
  
  #include <stdint.h>

  // ImGui
  #include "imgui.cpp"
  #include "imgui_demo.cpp"
  #include "imgui_draw.cpp"
#pragma warning( pop )

#include "mercury.h"

#include "mercury_imgui.cpp"
#include "mercury_input.cpp"
#include "mercury_game.cpp"
#include "mercury_program.cpp"
#include "mercury_math.cpp"

// Tell Optimus to use the high-performance NVIDIA processor
// option if it's on auto-select (we cannot override it)
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

static GLFWwindow *s_window;
static float s_deltaTime = 0.0f;
static int32_t s_width = 1280;
static int32_t s_height = 720;
static const char *s_name = "Hello World!";

inline float GetDeltaTime() {
	return s_deltaTime;
}

inline GLFWwindow *GetWindow() {
	return s_window;
}

inline int32_t GetWindowWidth() {
	return s_width;
}

inline int32_t GetWindowHeight() {
	return s_height;
}

void CALLBACK debugCallbackARB( GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar *message, GLvoid *userParam ) {
	if (type == GL_DEBUG_TYPE_OTHER) return;

	printf("Type: ");
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		printf("ERROR");
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		printf("DEPRECATED_BEHAVIOR");
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		printf("UNDEFINED_BEHAVIOR");
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		printf("PORTABILITY");
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		printf("PERFORMANCE");
		break;
	case GL_DEBUG_TYPE_OTHER:
		printf("OTHER");
		break;
	}
	printf(" - id: %d", id);
	printf(" - Severity: ");
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_LOW:
		printf("LOW");
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		printf("MEDIUM");
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		printf("HIGH");
		break;
	}
	printf(" - %s\n", message);
}

void GlfwErrorCallback( int32_t, const char *description ) {
	fputs( description, stderr );
	fputs( "\n", stderr );
}

void GlfwKeyCallBack( GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods ) {
	if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS ) {
		glfwSetWindowShouldClose( window, GL_TRUE );
	}

	if ( action == GLFW_PRESS )	{
		Input::SetKey( key, true );
	}
	if ( action == GLFW_RELEASE ) {
		Input::SetKey( key, false );
	}
}

void GlfwMouseButtonCallback( GLFWwindow *window, int32_t button, int32_t action, int32_t mods ) {
	if ( action == GLFW_PRESS )	{
		Input::SetMouseButton( button, true );
	}
	if ( action == GLFW_RELEASE ) {
		Input::SetMouseButton( button, false );
	}
}

void GlfwScrollCallback( GLFWwindow *window, double xoffset, double yoffset ) {
	// TODO: GUI
}

void GlfwCharCallback( GLFWwindow *window, uint32_t codepoint ) {
	// TODO: GUI
}

void GlfwCursorPosCallback( GLFWwindow *window, double xpos, double ypos ) {
	Input::SetMousePosition( float2 { (float) xpos, (float) ypos } );
}

void GlfwWindowSizeCallBack( GLFWwindow *window, int32_t width, int32_t height ) {
	// TODO
	//Application::SetWidth( width );
	//Application::SetHeight( height );
}

void EnterWindowLoop() {
	Init(); // Slow
	double lastTime = glfwGetTime();
	while ( !glfwWindowShouldClose( s_window ) )
	{
		double now = glfwGetTime();
		s_deltaTime = (float) ( now - lastTime );
		lastTime = now;

		glfwPollEvents();
		Input::Tick();

		Tick();
		Input::PostTick();
		glfwSwapBuffers( s_window );
	}

	glfwDestroyWindow( s_window );
	glfwTerminate();

	//std::exit( EXIT_SUCCESS );
}

void main() {
	glfwSetErrorCallback( GlfwErrorCallback );

	if ( glfwInit() ) {
		// Window hints
		glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
		glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 5 );
		glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

		s_window = glfwCreateWindow( s_width, s_height, s_name, nullptr, nullptr );
		glfwMakeContextCurrent( s_window );

		// TODO: Callbacks
		glfwSetKeyCallback( s_window, GlfwKeyCallBack );
		glfwSetMouseButtonCallback( s_window, GlfwMouseButtonCallback );
		glfwSetScrollCallback( s_window, GlfwScrollCallback );
		glfwSetCharCallback( s_window, GlfwCharCallback );
		glfwSetCursorPosCallback( s_window, GlfwCursorPosCallback );
		glfwSetWindowSizeCallback( s_window, GlfwWindowSizeCallBack );

		ImGui_ImplGlfwGL3_Init(s_window, true);

		// Sync to monitor refresh rate
		glfwSwapInterval( 1 );

		/************************************************************************/
		/* OpenGL                                                               */
		/************************************************************************/
		if ( gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) ) {
			printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);
			glEnable( GL_BLEND );
			glEnable( GL_MULTISAMPLE );
			glEnable( GL_CULL_FACE );
			glEnable( GL_DEPTH_TEST );
			glClearColor( 0.192156862745098f, 0.3019607843137255f, 0.4745098039215686f, 1.0f );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glDepthFunc( GL_LEQUAL );

			// https://www.opengl.org/wiki/Debug_Output
			glDebugMessageCallback( (GLDEBUGPROC) debugCallbackARB, nullptr );
			glEnable( GL_DEBUG_OUTPUT );
			glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );

			EnterWindowLoop();

		} else {
			printf( "Failed to init glad!\n" );
			// std::exit( EXIT_FAILURE );
		}

	} else {
		printf( "Failed to init GLFW!\n" );
		// std::exit( EXIT_FAILURE );
	}
}
