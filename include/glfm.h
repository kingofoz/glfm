/*
 GLFM
 Copyright (c) 2014 David Brackeen
 
 This software is provided 'as-is', without any express or implied warranty.
 In no event will the authors be held liable for any damages arising from the
 use of this software. Permission is granted to anyone to use this software
 for any purpose, including commercial applications, and to alter it and
 redistribute it freely, subject to the following restrictions:
 
 1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software in a
    product, an acknowledgment in the product documentation would be appreciated
    but is not required.
 2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.
 3. This notice may not be removed or altered from any source distribution.
 */
#ifndef _GLFM_H_
#define _GLFM_H_

#ifdef __cplusplus
extern "C" {
#endif
    
#define GLFM_VERSION_MAJOR    0
#define GLFM_VERSION_MINOR    7
#define GLFM_VERSION_REVISION 0

//
// Platform, which defines one of:
// GLFM_PLATFORM_IOS
// GLFM_PLATFORM_ANDROID
// GLFM_PLATFORM_EMSCRIPTEN
//
    
#if defined(ANDROID)
  #define GLFM_PLATFORM_ANDROID
#elif defined(__EMSCRIPTEN__)
  #define GLFM_PLATFORM_EMSCRIPTEN
#elif defined(__APPLE__)
  #include <TargetConditionals.h>
  #if TARGET_OS_IPHONE
    #define GLFM_PLATFORM_IOS
  #else
    #error Unknown Apple platform 
  #endif
#else
  #error Unknown platform 
#endif

//
// OpenGL ES 2.0 includes
//

#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

//
// Enums
//

typedef enum {
    GLFMColorFormatRGBA8888 = 0,
    GLFMColorFormatRGB565,
} GLFMColorFormat;

typedef enum {
    GLFMDepthFormatNone = 0,
    GLFMDepthFormat16,
    GLFMDepthFormat24,
} GLFMDepthFormat;

typedef enum {
    GLFMStencilFormatNone = 0,
    GLFMStencilFormat8,
} GLFMStencilFormat;

typedef enum {
    GLFMUserInterfaceOrientationAny = 0,
    GLFMUserInterfaceOrientationPortrait,
    GLFMUserInterfaceOrientationLandscape,
} GLFMUserInterfaceOrientation;

typedef enum {
    GLFMUserInterfaceIdiomPhone = 0,
    GLFMUserInterfaceIdiomTablet,
    GLFMUserInterfaceIdiomWeb,
} GLFMUserInterfaceIdiom;
    
typedef enum {
    GLFMTouchPhaseBegan = 0,
    GLFMTouchPhaseMoved,
    GLFMTouchPhaseEnded,
    GLFMTouchPhaseCancelled,
} GLFMTouchPhase;
    
typedef enum {
    GLFMKeyBackspace = 0x08,
    GLFMKeyTab       = 0x09,
    GLFMKeyEnter     = 0x0d,
    GLFMKeyEscape    = 0x1b,
    GLFMKeySpace     = 0x20,
    GLFMKeyLeft      = 0x25,
    GLFMKeyUp        = 0x26,
    GLFMKeyRight     = 0x27,
    GLFMKeyDown      = 0x28,
} GLFMKey;
    
typedef enum {
    GLFMKeyModifierShift = (1 << 0),
    GLFMKeyModifierCtrl  = (1 << 1),
    GLFMKeyModifierAlt   = (1 << 2),
    GLFMKeyModifierMeta  = (1 << 3),
} GLFMKeyModifier;

typedef enum {
    GLFMKeyActionPressed = 0,
    GLFMKeyActionRepeated,
    GLFMKeyActionReleased,
} GLFMKeyAction;

//
// Structs and function pointers
//

typedef struct GLFMDisplay GLFMDisplay;
    
/// Main loop callback function. The frame time is in seconds, and is not related to wall time.
typedef void (*GLFMMainLoopFunc)(GLFMDisplay*, const double frameTime);

/// Callback function for mouse or touch events. The (x,y) values are in pixels.
/// The function should return GL_TRUE if the event was handled, and false otherwise.
typedef GLboolean (*GLFMTouchFunc)(GLFMDisplay*, const int touch, const GLFMTouchPhase phase, const int x, const int y);

/// Callback function for key events.
/// The function should return GL_TRUE if the event was handled, and false otherwise.
typedef GLboolean (*GLFMKeyFunc)(GLFMDisplay*, const GLFMKey keyCode, const GLFMKeyAction action, const int modifiers);

/// Callback when the surface could not be created.
typedef void (*GLFMSurfaceErrorFunc)(GLFMDisplay*, const char *message);

/// Callback function when the OpenGL surface is created
typedef void (*GLFMSurfaceCreatedFunc)(GLFMDisplay*, const int width, const int height);

/// Callback function when the OpenGL surface is resized (or rotated).
typedef void (*GLFMSurfaceResizedFunc)(GLFMDisplay*, const int width, const int height);

/// Callback function when the OpenGL surface is destroyed. 
typedef void (*GLFMSurfaceDestroyedFunc)(GLFMDisplay*);
  
/// Callback function when the system recieves a low memory warning.
typedef void (*GLFMMemoryWarningFunc)(GLFMDisplay*);

typedef void (*GLFMAppPausingFunc)(GLFMDisplay*);
    
typedef void (*GLFMAppResumingFunc)(GLFMDisplay*);
    
//
// Functions
//

/// Main entry point for the app, where the display can be initialized and the GLFMMainLoopFunc can be set.
extern void glfm_main(GLFMDisplay *display);

/// Init the display condifuration. Should only be called in glfm_main.
void glfmSetDisplayConfig(GLFMDisplay *display,
                          const GLFMColorFormat colorFormat,
                          const GLFMDepthFormat depthFormat,
                          const GLFMStencilFormat stencilFormat,
                          const GLFMUserInterfaceOrientation allowedOrientations,
                          const GLboolean hideStatusBar);

void glfmSetUserData(GLFMDisplay *display, void *userData);

void *glfmGetUserData(GLFMDisplay *display);
    
/// Sets whether multitouch input is enabled. By default, multitouch is disabled.
void glfmSetMultitouchEnabled(GLFMDisplay *display, const GLboolean multitouchEnabled);
    
/// Gets whether multitouch input is enabled. By default, multitouch is disabled.
GLboolean glfmGetMultitouchEnabled(GLFMDisplay *display);
    
/// Gets the display width, in pixels. The result will only be valid after the surface is created,
/// or in GLFMMainLoopFunc
int glfmGetDisplayWidth(GLFMDisplay *display);

/// Gets the display height, in pixels. The result will only be valid after the surface is created,
/// or in GLFMMainLoopFunc
int glfmGetDisplayHeight(GLFMDisplay *display);

/// Gets the display scale. On Apple devices, the value will be 1.0 for non-retina displays and 2.0 for retina.
float glfmGetDisplayScale(GLFMDisplay *display);

/// Gets the user interface idiom (phone, tablet, or web).
GLFMUserInterfaceIdiom glfmGetUserInterfaceIdiom(GLFMDisplay *display);

/// Checks if a named OpenGL extension is supported
GLboolean glfmExtensionSupported(const char *extension);
   
/// Sets the function to call before each frame is displayed.
void glfmSetMainLoopFunc(GLFMDisplay *display, GLFMMainLoopFunc mainLoopFunc);
    
/// Sets the function to call when a mouse or touch event occurs.
void glfmSetTouchFunc(GLFMDisplay *display, GLFMTouchFunc touchFunc);
    
/// Sets the function to call when a key event occurs.
/// Note, on iOS, only pressed events are sent (no repeated or released events) and with no modifiers.
void glfmSetKeyFunc(GLFMDisplay *display, GLFMKeyFunc keyFunc);
    
/// Sets the function to call when the surface could not be created.
/// For example, the browser does not support WebGL.
void glfmSetSurfaceErrorFunc(GLFMDisplay *display, GLFMSurfaceErrorFunc surfaceErrorFunc);

void glfmSetSurfaceCreatedFunc(GLFMDisplay *display, GLFMSurfaceCreatedFunc surfaceCreatedFunc);

/// Sets the function to call when the surface was resized (or rotated).
void glfmSetSurfaceResizedFunc(GLFMDisplay *display, GLFMSurfaceResizedFunc surfaceResizedFunc);
    
/// Sets the function to call when the surface was destroyed. For example, OpenGL context loss.
/// All OpenGL resources should be deleted in this call.
void glfmSetSurfaceDestroyedFunc(GLFMDisplay *display, GLFMSurfaceDestroyedFunc surfaceDestroyedFunc);

void glfmSetMemoryWarningFunc(GLFMDisplay *display, GLFMMemoryWarningFunc lowMemoryFunc);
    
void glfmSetAppPausingFunc(GLFMDisplay *display, GLFMAppPausingFunc pausingFunc);

void glfmSetAppResumingFunc(GLFMDisplay *display, GLFMAppResumingFunc resumingFunc);

#ifdef __cplusplus
}
#endif

#endif