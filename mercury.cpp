#include <codeanalysis/warnings.h>
#pragma warning( push )
  #pragma warning( disable : ALL_CODE_ANALYSIS_WARNINGS )
  
  #include "glad/glad.h"
  #include "glad.c"
  
  #include "GLFW/glfw3.h"
  
  #define STB_IMAGE_IMPLEMENTATION
  #include "stb_image.h"
  
  #include <stdint.h>
#pragma warning( pop )

#include "mercury.h"
#include "mercury-math.h"
#include "mercury-program.h"

// Tell Optimus to use the high-performance NVIDIA processor
// option if it's on auto-select (we cannot override it)
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

GLFWwindow *s_window;
int s_width = 1280;
int s_height = 720;
const char *name = "Hello World!";

void main() {
	// glfwSetErrorCallback( GlfwErrorCallback );

	if ( glfwInit() ) {
		// Window hints
		glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
		glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 5 );
		glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

		s_window = glfwCreateWindow( s_width, s_height, name, nullptr, nullptr );
		glfwMakeContextCurrent( s_window );

		// TODO: Callbacks
		// glfwSetKeyCallback( s_window, GlfwKeyCallBack );
		// glfwSetMouseButtonCallback( s_window, GlfwMouseButtonCallback );
		// glfwSetScrollCallback( s_window, GlfwScrollCallback );
		// glfwSetCharCallback( s_window, GlfwCharCallback );
		// glfwSetCursorPosCallback( s_window, GlfwCursorPosCallback );
		// glfwSetWindowSizeCallback( s_window, GlfwWindowSizeCallBack );

		// Sync to monitor refresh rate
		glfwSwapInterval( 1 );

	/************************************************************************/
	/* OpenGL                                                               */
	/************************************************************************/
		if ( gladLoadGL() ) {
			glEnable( GL_BLEND );
			glEnable( GL_MULTISAMPLE );
			glEnable( GL_CULL_FACE );
			glEnable( GL_DEPTH_TEST );
			glEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS );
			glClearColor( 0.192156862745098f, 0.3019607843137255f, 0.4745098039215686f, 1.0f );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glDepthFunc( GL_LEQUAL );

			// https://www.opengl.org/wiki/Debug_Output
			// glDebugMessageCallback( (GLDEBUGPROC) debugCallbackARB, nullptr );
			glEnable( GL_DEBUG_OUTPUT );
			glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );

			
			LoadProgram();
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

void EnterWindowLoop() {
	double lastTime = glfwGetTime();
	while ( !glfwWindowShouldClose( s_window ) )
	{
		double now = glfwGetTime();
		//SetDeltaTime( (float) ( now - lastTime ) );
		lastTime = now;

		glfwPollEvents();
		//mj::Input::Tick();

		Tick();
		//mj::Input::PostTick();
		glfwSwapBuffers( s_window );
	}

	glfwDestroyWindow( s_window );
	glfwTerminate();

	//std::exit( EXIT_SUCCESS );
}

void Tick() {
	glClear(GL_COLOR_BUFFER_BIT);
}