//#include "Main.h"

#define NOMINMAX
#include "DXUT.h"
#include "CraterRacerApp.h"

//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------
INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	RacerApp* pApp = new RacerApp(); //The application managing the game and gui

    // Set the callback functions
	DXUTSetCallbackD3D9DeviceCreated( RacerApp::OnCreateDevice );
	DXUTSetCallbackD3D9DeviceDestroyed( RacerApp::OnDestroyDevice );
	DXUTSetCallbackMsgProc( RacerApp::MsgProc );
    DXUTSetCallbackD3D9DeviceReset( RacerApp::OnResetDevice );
    DXUTSetCallbackD3D9DeviceLost( RacerApp::OnLostDevice );
	DXUTSetCallbackD3D9FrameRender( RacerApp::OnRender );
	DXUTSetCallbackFrameMove( RacerApp::OnUpdateGame );
	DXUTSetCallbackKeyboard( RacerApp::OnKeyboard );
	DXUTSetCallbackDeviceChanging( RacerApp::ModifyDeviceSettings );
	DXUTSetCallbackD3D9DeviceAcceptable( RacerApp::IsD3D9DeviceAcceptable );

    // Initialize DXUT and create the desired Win32 window and Direct3D device for the application
    DXUTInit( true, true );						
    DXUTSetHotkeyHandling( true, true, true );  // handle the default hotkeys 
												// ALT+ENTER -> full screen toggle
												// ESC -> exit application
												// PAUSE or BREAK -> pause time
    DXUTSetCursorSettings( true, true );		// Show the cursor and clip it when in full screen
    DXUTCreateWindow( L"Crater Racer" );
    DXUTCreateDevice( true, 1200, 900 ); //false = full screen mode

    // Start the render loop
    DXUTMainLoop();
	
	delete pApp;
	CleanupXACT();

    return DXUTGetExitCode();	
}