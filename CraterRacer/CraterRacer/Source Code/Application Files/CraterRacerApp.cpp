//--------------------------------------------------------------------------------------
// File: CraterRacerApp.cpp
//
//--------------------------------------------------------------------------------------
#include "CraterRacerApp.h"

GameObj* g_pGame = NULL;	//global game used by the RacerApp class 

//--------------------------------------------------------------------------------------
// Function:  OnUpdateGame
// This is the callback called by DXUT in the render loop.  It is called right before
// the render function is called.  We are using this function to update our game (do
// simulations and collision detection)
// INPUTS:  double fTime = time in seconds that the application has been running so far
//			float fElapsedTime = time since the last render loop ended
//			*pUserContext = we aren't using this
//--------------------------------------------------------------------------------------
void CALLBACK RacerApp::OnUpdateGame( double fTime, float fElapsedTime, void* pUserContext  )
{
	g_pGame->processInput( fElapsedTime );
	g_pGame->simulate( fElapsedTime );
}



//--------------------------------------------------------------------------------------
// Function: MsgProc
// This callback function is called by windows when any message occurs.  Right now we
// are not using it.
//--------------------------------------------------------------------------------------
LRESULT CALLBACK RacerApp::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext )
{
	return 0;
}


//--------------------------------------------------------------------------------------
// Function: OnKeyboard
// When a key on the keyboard is pressed or released, this function is called.
//--------------------------------------------------------------------------------------
void CALLBACK RacerApp::OnKeyboard ( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
	if( g_pGame )
	{
		// online virtual key codes: http://msdn.microsoft.com/en-us/library/ms927178.aspx
		switch( nChar ) 
		{
			case  VK_SPACE:
				//pause game here
				break;

			default:
				g_pGame->addInput( bKeyDown, nChar );
				break;
		}
	}
}


//--------------------------------------------------------------------------------------
// Function:  OnGUIEvent
// We do not use this function yet, but will use it for managing the pre-game screen
// and pause screen.
//--------------------------------------------------------------------------------------
void CALLBACK RacerApp::OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{

}


//--------------------------------------------------------------------------------------
// Function: OnResetDevice
// This function is called when the window size changes.  In here we want to reset
// all objects that rely on the back buffer size.
//--------------------------------------------------------------------------------------
HRESULT CALLBACK RacerApp::OnResetDevice( IDirect3DDevice9* device, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	if( g_pGame )
		g_pGame->processCallback( ON_RESET, device, pBackBufferSurfaceDesc );
	
	device->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Function:  OnLostDevice
// This function is called right before the device is destroyed.  We want to prepare 
// anything attached to the device for the upcoming destruction.
//--------------------------------------------------------------------------------------
void CALLBACK RacerApp::OnLostDevice(void* pUserContext )
{
	if( g_pGame )
		g_pGame->processCallback( ON_LOST );
}



//--------------------------------------------------------------------------------------
// Function: OnRender
// This callback function is called every frame to render the scene.  
//--------------------------------------------------------------------------------------
void CALLBACK RacerApp::OnRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext  )
{
	HRESULT hr;

    // Clear the render target and the zbuffer 
    V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 150,0, 20, 30 ), 1.0f, 0 ) );

    // Render the scene
    if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {
		//get the game to render all of its components
		g_pGame->render( pd3dDevice );

        V( pd3dDevice->EndScene() );
    }
}


//--------------------------------------------------------------------------------------
// Function: OnCreateDevice
// This function is called when a device is created.  We are using this function
// as the place to initialize the game (by initializing its components with the device)
//--------------------------------------------------------------------------------------
HRESULT CALLBACK RacerApp::OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	g_pGame->initGame( pd3dDevice, pBackBufferSurfaceDesc );

	return S_OK;
}


//-------------------------------------------------------------------------------------
// Function:  OnDestroyDevice
// This function is called when a device is destroyed.  We will use this function to 
// release any memory attached to the device that we need to clean up.
//--------------------------------------------------------------------------------------
void CALLBACK RacerApp::OnDestroyDevice( void* pUserContext )
{
	if( g_pGame )
		g_pGame->processCallback( ON_DESTROY );
}


//--------------------------------------------------------------------------------------
// Function:  IsD3D9DeviceAcceptable
// This function rejects any D3D9 devices that aren't acceptable to the app by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK RacerApp::IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat,
                                      D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
    // Skip backbuffer formats that don't support alpha blending
    IDirect3D9* pD3D = DXUTGetD3D9Object();
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                                         AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
                                         D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;

    // No fallback defined by this app, so reject any device that 
    // doesn't support at least ps2.0
    if( pCaps->PixelShaderVersion < D3DPS_VERSION( 2, 0 ) )
        return false;

    return true;
}


//--------------------------------------------------------------------------------------
// Function:  ModifyDeviceSettings
// This function is called right before creating a D3D9 or D3D10 device, allowing the app to modify the 
// device settings as needed.
// We do not use this function yet.
//--------------------------------------------------------------------------------------
bool CALLBACK RacerApp::ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
    return true;
}


//--------------------------------------------------------------------------------------
RacerApp::RacerApp()
{
	g_pGame = new GameObj();
}


//--------------------------------------------------------------------------------------
RacerApp::~RacerApp()
{
	if( g_pGame )
	{
		g_pGame->processCallback( ON_DESTROY ); //release the memory/objects used by the game
		delete g_pGame;
		g_pGame = NULL;
	}
}