#pragma once
#include "helpers/math.h"

// Window class using SDL windows
class Window {
public:
	// Construct but don't create
	Window() : sdlWindow(nullptr), sdlGlContext(nullptr) {}

	// Construct and create
	Window(const char* windowTitle, const Vec2I& windowSize) {
		Init(windowTitle, windowSize);
	}

	~Window();

public:
	void Init(const char* windowTitle, const Vec2I& windowSize);

public:
	void BeginRender(bool doClear = true);

	void EndRender();

public:
	// Returns the error state where 0 = no errors in creation
	int GetError() const {
		return (int)(sdlWindow != nullptr);
	}

public:
	// Tries to create a GL context. If successful, returns the context. If the context already exists, nullptr is returned. Use GetGlContext in this case
	void* CreateGlContext();

	void* GetGlContext();

private:
	struct SDL_Window* sdlWindow;

	void* sdlGlContext;
};