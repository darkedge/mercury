#pragma once
#include "mercury-math.h"
#include <cstdint>

namespace Input {
	bool GetKey( int32_t key );
	bool GetKeyDown( int32_t key );
	bool GetKeyUp( int32_t key );
	bool GetMouse( int32_t button );
	bool GetMouseDown( int32_t button );
	bool GetMouseUp( int32_t button );
	bool IsMouseGrabbed();
	// In pixels. Origin is top-left
	float2 GetMouseDelta();
	// In pixels. Origin is top-left
	float2 GetMousePosition();
	void SetMouseButton( int32_t button, bool pressed );
	void SetKey( int32_t key, bool pressed );
	void SetMousePosition( const float2 pos );
	void Init();
	void Tick();
	void PostTick();
	void SetMouseGrabbed( bool grabMouse );
} // namespace Input
