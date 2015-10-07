#include "mercury-input.h"
#include "mercury.h"

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

bool Input::GetKey( int key ) {
	return keys[key];
}
bool Input::GetKeyDown( int key ) {
	return down[key];
}
bool Input::GetKeyUp( int key ) {
	return up[key];
}

bool Input::GetMouse( int button ) {
	return mouseButtons[button];
}

bool Input::GetMouseDown( int button ) {
	return mouseDown[button];
}

bool Input::GetMouseUp( int button ) {
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

void Input::SetMouseButton( int button, bool pressed ) {
	mouseButtons[button] = pressed;
}

void Input::SetKey( int key, bool pressed ) {
	keys[key] = pressed;
}

void Input::SetMousePosition( const float2 pos ) {
	mousePos = pos;
	mouseDelta += pos - lastMousePos;
}

void Input::Init() {
	// Keyboard
	memset( prev, int( false ), MC_NUM_KEYBOARD_KEYS * sizeof( bool ) );
	memset( keys, int( false ), MC_NUM_KEYBOARD_KEYS * sizeof( bool ) );
	memset( down, int( false ), MC_NUM_KEYBOARD_KEYS * sizeof( bool ) );
	memset( up, int( false ), MC_NUM_KEYBOARD_KEYS * sizeof( bool ) );

	// Mouse
	memset( mousePrev, int( false ), MC_NUM_MOUSE_BUTTONS * sizeof( bool ) );
	memset( mouseButtons, int( false ), MC_NUM_MOUSE_BUTTONS * sizeof( bool ) );
	memset( mouseDown, int( false ), MC_NUM_MOUSE_BUTTONS * sizeof( bool ) );
	memset( mouseUp, int( false ), MC_NUM_MOUSE_BUTTONS * sizeof( bool ) );

	isMouseGrabbed = false;
	mouseDelta = {0, 0};
}

void Input::Tick() {
	// Keyboard
	{
		bool changes[MC_NUM_KEYBOARD_KEYS];
		for ( unsigned int i = 0; i < MC_NUM_KEYBOARD_KEYS; i++ ) {
			changes[i]	= keys[i]		^ prev[i];
			down[i]		= changes[i]	& keys[i];
			up[i]		= changes[i]	& !keys[i];
		}
	}

	// Mouse
	{
		bool changes[MC_NUM_MOUSE_BUTTONS];
		for ( unsigned int i = 0; i < MC_NUM_MOUSE_BUTTONS; i++ ) {
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
