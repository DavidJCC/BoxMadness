// Initial OpenGL Framework
// DO not modify this project. Use a copy of it.
#include "Game.h"
#include "key_codes.h"
////////////////////////////////////////////////////////////////////
#include <Common/Base/keycode.cxx>	// MUST be in main file!!!
// we're not using any product apart from Havok Physics.
#ifdef HK_FEATURE_PRODUCT_AI
	#undef HK_FEATURE_PRODUCT_AI
#endif
#ifdef HK_FEATURE_PRODUCT_ANIMATION
	#undef HK_FEATURE_PRODUCT_ANIMATION
#endif
#ifdef HK_FEATURE_PRODUCT_CLOTH
	#undef HK_FEATURE_PRODUCT_CLOTH
#endif
#ifdef HK_FEATURE_PRODUCT_DESTRUCTION
	#undef HK_FEATURE_PRODUCT_DESTRUCTION
#endif
#ifdef HK_FEATURE_PRODUCT_BEHAVIOR
	#undef HK_FEATURE_PRODUCT_BEHAVIOR
#endif

// Also we're not using any serialization/versioning so we don't need any of these.
#define HK_EXCLUDE_FEATURE_SerializeDeprecatedPre700
#define HK_EXCLUDE_FEATURE_RegisterVersionPatches
#define HK_EXCLUDE_FEATURE_MemoryTracker

// Visual debugging tool needs the reflected classes
#define HK_EXCLUDE_FEATURE_MemoryTracker
#define HK_EXCLUDE_FEATURE_hkpAccurateInertiaTensorComputer
#define HK_EXCLUDE_FEATURE_CompoundShape
#define HK_EXCLUDE_FEATURE_hkpAabbTreeWorldManager
#define HK_EXCLUDE_FEATURE_hkpContinuousSimulation
#define HK_EXCLUDE_FEATURE_hkpKdTreeWorldManager
#include <Common/Base/Config/hkProductFeatures.cxx>	
///////////////////////////////////////////////////////////////////


// tell the compiler to always link into a Winmain, even if we are using a console app
#pragma comment (linker, "/ENTRY:WinMainCRTStartup") 

/*
The following code tells the linker whether to create a console or not. If we are debugging logic,
we can easily switch to console output and print debug messages to the console.
*/

// comment this out for OGL usage.
//#define _CONSOLEONLY_

#ifdef _CONSOLEONLY_
	#pragma comment (linker, "/SUBSYSTEM:CONSOLE") // Show console output in debug mode 
#else 
	#pragma comment (linker, "/SUBSYSTEM:WINDOWS") 
#endif 

Game g_Game;	//Access to Game Object


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_KEYDOWN:
			switch(wParam)	
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;
				case VK_SPACE:
					g_Game.PauseGame();
					break;
				case VK_1:
					g_Game.boxSelected = 0;
					break;
				case VK_2:
					g_Game.boxSelected = 1;
					break;
				case VK_3:
					g_Game.boxSelected = 2;
					break;
				case VK_4:
					g_Game.boxSelected = 3;
					break;
				case VK_L:
					g_Game.CameraLock();
					break;
				case VK_ADD:
					g_Game.audioPl->volUp();
					break;
				case VK_SUBTRACT:
					g_Game.audioPl->volDown();
					break;
				case VK_R:
					//if(g_Game.gameState == gameOver)
						g_Game.Reset();
					break;
				default:
					break;
			}
			break;

		case WM_KEYUP:
			switch(wParam)
			{
			case VK_W:
				g_Game.StopMotion();
				break;
			case VK_S:
				g_Game.StopMotion();
				break;
			default:
				break;
			}
		break;
		case WM_DESTROY:
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		case WM_SIZE:
			g_Game.Resize(LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_MOUSEMOVE:
			// Track the mouse position
			if(g_Game.lockCam == false)
			{
				g_Game.mouseX = LOWORD(lParam);
				g_Game.mouseY = HIWORD(lParam);
			}
			break;
		case WM_MOUSEWHEEL:
			if(g_Game.lockCam == false)
			{
				if(GET_WHEEL_DELTA_WPARAM(wParam) < 0)  //chk dir of scroll wheel
						g_Game.camRad *= 1.1f;
				else
					g_Game.camRad /= 1.1f;
				g_Game.CameraPos();
			}
			break;
		case WM_LBUTTONDOWN:
			break;
		case WM_RBUTTONDOWN:
			break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/**
This is the starting point for the application
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	MSG msg;
	bool done = false;

	// don't create a screen if we are in "console output" mode
#ifndef _CONSOLEONLY_
	g_Game.CreateScreen("Applied Physics Assignment", 1024, 720, 32, false);
	g_Game.InitOpenGL();
#endif

	DebugOut("Initialising Game");
	g_Game.Initialise();

	// ===========================================================================
	//            THE MAIN GAME LOOP
	// ===========================================================================
	while(!done){
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			if(msg.message == WM_QUIT){
				done = true;
			}else{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}else{
			// the loop that updates the game..
			g_Game.Update();
			// don't perform graphical updates if we are in "console" mode
#ifndef _CONSOLEONLY_
			g_Game.Render();
			g_Game.Swap();
#endif
		}
	}

// don't need to release any screens if we are using a console.
#ifndef _CONSOLEONLY_
	g_Game.ReleaseScreen();
#endif
	
	g_Game.Shutdown();

	return msg.wParam;
}
