//#include "main.h"

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


	RacerApp* pApp = new RacerApp();

    // Set the callback functions
	DXUTSetCallbackD3D9DeviceCreated( RacerApp::OnCreateDevice );
	DXUTSetCallbackD3D9DeviceDestroyed( RacerApp::OnDestroyDevice );
	DXUTSetCallbackMsgProc( RacerApp::MsgProc );
    DXUTSetCallbackD3D9DeviceReset( RacerApp::OnResetDevice );
    DXUTSetCallbackD3D9DeviceLost( RacerApp::OnLostDevice );
	DXUTSetCallbackD3D9FrameRender( RacerApp::OnRender );
	DXUTSetCallbackFrameMove( RacerApp::OnUpdateGame );
	//DXUTSetCallbackKeyboard( KeyboardProc );

    // TODO: Perform any application-level initialization here


    // Initialize DXUT and create the desired Win32 window and Direct3D device for the application
    DXUTInit( true, true );						// Parse the command line and show msgboxes
    DXUTSetHotkeyHandling( true, true, true );  // handle the default hotkeys
    DXUTSetCursorSettings( true, true );		// Show the cursor and clip it when in full screen
    DXUTCreateWindow( L"Crater Racer" );
    DXUTCreateDevice( true, 640, 480 );
	DXUTToggleFullScreen();

    // Start the render loop
	//gameLoop();
    DXUTMainLoop();

	delete pApp;

    return 0;	
}