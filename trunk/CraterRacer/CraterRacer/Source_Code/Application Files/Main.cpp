#define NOMINMAX
#include "DXUT.h"
#include "CraterRacerApp.h"
#include "MessageManager.h"

#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC_NEW
#include <stdlib.h>
#include <crtdbg.h>

//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------
INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	_crtBreakAlloc = 18;
	RacerApp* pApp = new RacerApp(); //The application managing the game and gui

	InitEmitter( pApp );

	//turn off cursor
	ShowCursor( false );

    // Set the callback functions
	DXUTSetCallbackD3D9DeviceCreated( RacerApp::OnCreateDevice );
	DXUTSetCallbackD3D9DeviceDestroyed( RacerApp::OnDestroyDevice );
    DXUTSetCallbackD3D9DeviceReset( RacerApp::OnResetDevice );
    DXUTSetCallbackD3D9DeviceLost( RacerApp::OnLostDevice );
	DXUTSetCallbackD3D9FrameRender( RacerApp::OnRender );
	DXUTSetCallbackFrameMove( RacerApp::OnUpdateGame );
	DXUTSetCallbackKeyboard( RacerApp::OnKeyboard );
	DXUTSetCallbackD3D9DeviceAcceptable( RacerApp::IsD3D9DeviceAcceptable );
	DXUTSetCallbackDeviceChanging( RacerApp::ModifyDeviceSettings );

    // Initialize DXUT and create the desired Win32 window and Direct3D device for the application
    DXUTInit( true, true );						
    DXUTSetHotkeyHandling( false, false, false );  // handle the default hotkeys 
												// ALT+ENTER -> full screen toggle
												// ESC -> exit application
												// PAUSE or BREAK -> pause time
    DXUTSetCursorSettings( true, true );		// Show the cursor and clip it when in full screen
    DXUTCreateWindow( L"Crater Racer" );
    DXUTCreateDevice( true, 1200, 900 ); //false = full screen mode
	//DXUTCreateDevice( true, 1200, 900 );

    // Start the render loop
    DXUTMainLoop();
	
	delete pApp;
	
	MessageManager::deleteManager( );

	CleanupXACT();

    return DXUTGetExitCode();	
}