#include "mercury_input.h"
#include "GLFW/glfw3.h"
#include "mercury.h"
#include <cstring>

#define MC_NUM_KEYBOARD_KEYS GLFW_KEY_LAST + 1
#define MC_NUM_MOUSE_BUTTONS GLFW_MOUSE_BUTTON_LAST + 1

// Keyboard
static bool prev[MC_NUM_KEYBOARD_KEYS];
static bool keys[MC_NUM_KEYBOARD_KEYS];
static bool down[MC_NUM_KEYBOARD_KEYS];
static bool up[MC_NUM_KEYBOARD_KEYS];

// Mouse
static bool mousePrev[MC_NUM_MOUSE_BUTTONS];
static bool mouseButtons[MC_NUM_MOUSE_BUTTONS];
static bool mouseDown[MC_NUM_MOUSE_BUTTONS];
static bool mouseUp[MC_NUM_MOUSE_BUTTONS];

static bool isMouseGrabbed;
static float2 lastMousePos;
static float2 mouseDelta;
static float2 mousePos;

bool Input::GetKey( int32_t key ) {
	return keys[key];
}
bool Input::GetKeyDown( int32_t key ) {
	return down[key];
}
bool Input::GetKeyUp( int32_t key ) {
	return up[key];
}

bool Input::GetMouse( int32_t button ) {
	return mouseButtons[button];
}

bool Input::GetMouseDown( int32_t button ) {
	return mouseDown[button];
}

bool Input::GetMouseUp( int32_t button ) {
	return mouseUp[button];
}

bool Input::IsMouseGrabbed() {
	return isMouseGrabbed;
}

// In pixels. Origin is top-left
float2 Input::GetMouseDelta() {
	return mouseDelta;
}

// In pixels. Origin is top-left
float2 Input::GetMousePosition() {
	return mousePos;
}

void Input::SetMouseButton( int32_t button, bool pressed ) {
	mouseButtons[button] = pressed;
}

void Input::SetKey( int32_t key, bool pressed ) {
	keys[key] = pressed;
}

void Input::SetMousePosition( const float2 pos ) {
	mousePos = pos;
	mouseDelta += pos - lastMousePos;
}

void Input::Init() {
	// Keyboard
	memset( prev, int32_t( false ), MC_NUM_KEYBOARD_KEYS * sizeof( bool ) );
	memset( keys, int32_t( false ), MC_NUM_KEYBOARD_KEYS * sizeof( bool ) );
	memset( down, int32_t( false ), MC_NUM_KEYBOARD_KEYS * sizeof( bool ) );
	memset( up, int32_t( false ), MC_NUM_KEYBOARD_KEYS * sizeof( bool ) );

	// Mouse
	memset( mousePrev, int32_t( false ), MC_NUM_MOUSE_BUTTONS * sizeof( bool ) );
	memset( mouseButtons, int32_t( false ), MC_NUM_MOUSE_BUTTONS * sizeof( bool ) );
	memset( mouseDown, int32_t( false ), MC_NUM_MOUSE_BUTTONS * sizeof( bool ) );
	memset( mouseUp, int32_t( false ), MC_NUM_MOUSE_BUTTONS * sizeof( bool ) );

	isMouseGrabbed = false;
	mouseDelta = {0, 0};
}

void Input::Tick() {
	// Keyboard
	{
		bool changes[MC_NUM_KEYBOARD_KEYS];
		for ( int32_t i = 0; i < MC_NUM_KEYBOARD_KEYS; i++ ) {
			changes[i]	= keys[i]		^ prev[i];
			down[i]		= changes[i]	& keys[i];
			up[i]		= changes[i]	& !keys[i];
		}
	}

	// Mouse
	{
		bool changes[MC_NUM_MOUSE_BUTTONS];
		for ( int32_t i = 0; i < MC_NUM_MOUSE_BUTTONS; i++ ) {
			changes[i] = mouseButtons[i] ^ mousePrev[i];
			mouseDown[i] = changes[i] & mouseButtons[i];
			mouseUp[i] = changes[i] & !mouseButtons[i];
		}
	}
}

void Input::PostTick() {
	memcpy( prev, keys, MC_NUM_KEYBOARD_KEYS * sizeof( bool ) );
	memcpy( mousePrev, mouseButtons, MC_NUM_MOUSE_BUTTONS * sizeof( bool ) );
	mouseDelta = {0, 0};
	lastMousePos = mousePos;
}

void Input::SetMouseGrabbed( bool grabMouse ) {
	isMouseGrabbed = grabMouse;
	if ( isMouseGrabbed ) {
		glfwSetInputMode( GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED );
	} else {
		glfwSetInputMode( GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL );
	}
}
