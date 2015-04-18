// GNU:
#ifndef _GNU_SOURCE
#  define _GNU_SOURCE
#endif
#include <dlfcn.h>

// C++ Standard Library:
#include <cstdint>
#include <string>
#include <cstring>

// OpenGL:
#include <GL/gl.h>
#include <GL/glx.h>

// Ed:
#include "../DriverInterface.hpp"
#include <edvonhack/util/FunctionPointerCast.hpp>

// Driver for targets, which use glX and Wine -> OpenGL Win32 applications.
// The idea is to hook glXGetProcAddressARB and return a hooked glXSwapBuffers
// so we get to call our code at the end of every frame.
// But Wine causes problems because it loads glXGetProcAddressARB itself
// via dlsym so we need to start at hooking dlsym. Hooking dlsym forces us to
// find a way to load the real dlsym so it does not break code using dlsym
// and we can't use dlsym to load dlsym because this obviously leads to
// recursion. The solution for this is using dlvsym. The only drawback is that
// we need a valid version string for dlsym now.

// The version string of dlsym. Can be retrieved via looking at
// objdump -T /usr/lib/libdl.so.$ | grep dlsym
// TODO: Retrieve this at runtime.
static std::string VERSION_STRING = "GLIBC_2.0";

// The types of all the real function we are hooking.
typedef void* (*dlsym_T)(void*, const char*);
typedef void* (*glXGetProcAddressARB_T)(char const*);
typedef void (*glXSwapBuffers_T)(Display*, GLXDrawable);

// All the real function we are hooking.
static dlsym_T dlsym_Real = nullptr;
static glXGetProcAddressARB_T glXGetProcAddressARB_Real = nullptr;
static glXSwapBuffers_T glXSwapBuffers_Real = nullptr;

static void glXSwapBuffers_Hook(Display* display, GLXDrawable drawable) {

	// Retrieve the original glXSwapBuffers if we don't have it yet.
	if(glXSwapBuffers_Real == nullptr) {
		glXSwapBuffers_Real =
			ed::toFunctionPointer<void, Display*, GLXDrawable>(
				glXGetProcAddressARB_Real("glXSwapBuffers"));
	}
	
	// Call the initialization function if not done yet.
	static bool isInit = false;
	if(!isInit) {
		ed::driverif::init();
      isInit = true;
	}
	
	// Notify driver main of the frame and call the real glXSwapBuffers.
	ed::driverif::onFrame();
	glXSwapBuffers_Real(display, drawable);
}

static void* glXGetProcAddressARB_Hook(char const* symbol) {

	// Retrieve the original glXGetProcAddressARB if we don't have it yet.
	if(glXGetProcAddressARB_Real == nullptr) {
		glXGetProcAddressARB_Real =
			ed::toFunctionPointer<void*, char const*>(
				dlsym_Real(RTLD_DEFAULT, "glXGetProcAddressARB"));
	}
	
	// Return our hooked glXSwapBuffers upon request.
	if(std::strcmp(symbol, "glXSwapBuffers") == 0) {
		return ed::toObjectPointer(&glXSwapBuffers_Hook);
	}

	// All other requests shall be handled by the real glXGetProcAddressARB.
    return glXGetProcAddressARB_Real(symbol);
}

void* dlsym(void* handle, char const* symbol) {

	// Retrieve the original dlsym via dlvsym if we don't have it yet.
	// We can't use dlsym because this would result in recursive calls.
    if(dlsym_Real == nullptr) {
        dlsym_Real = ed::toFunctionPointer<void*, void*, char const*>(
			dlvsym(RTLD_DEFAULT, "dlsym", VERSION_STRING.c_str()));
	}
 
	// Return our hooked glXGetProcAddressARB upon request.
    if(std::strcmp(symbol, "glXGetProcAddressARB") == 0) {
        return ed::toObjectPointer(&glXGetProcAddressARB_Hook);
	}

	// All other requests shall be handled by the real dlsym.
    return dlsym_Real(handle, symbol);
}