//--------------------------------------------------------------------------------------
// File: CraterRacerApp.cpp
//
//--------------------------------------------------------------------------------------
#include "CraterRacerApp.h"

GameObj* g_pGame = NULL;	//global game used by the RacerApp class 

//instatiating all the static member variables used by RacerApp
ResourceManager	 RacerApp::m_ResourceManager;
ApplicationState RacerApp::m_AppState;
Dialog			 RacerApp::m_MenuScreen;
Dialog			 RacerApp::m_OnePlayerScreen;
XBox360Controller* RacerApp::m_MenuController;


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
	if( m_AppState != APP_STARTUP )
	{
		g_pGame->processInput( fElapsedTime );
		g_pGame->simulate( fElapsedTime );
		g_pGame->think( );
		UpdateAudio();
	}
	else
	{
		m_MenuController->Update(fElapsedTime);

		//move menu up
		//if( m_MenuController->LeftThumbstick.GetY() > 0.2 )
		if( m_MenuController->A.WasPressed() )
		{
			m_AppState = APP_RENDER_GAME;
		}
	}
}



//--------------------------------------------------------------------------------------
// Function: MsgProc
// This callback function is called by windows when any message occurs.  Right now we
// are not using it.
//--------------------------------------------------------------------------------------
LRESULT CALLBACK RacerApp::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext )
{
	//Let the resource manager handle it first
	*pbNoFurtherProcessing = m_ResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
	if( *pbNoFurtherProcessing )
		return 0;

	switch( m_AppState )
	{
		//If we are in startup, we want the menu screen to handle all messages
		case APP_STARTUP:
			*pbNoFurtherProcessing = m_MenuScreen.MsgProc( hWnd, uMsg, wParam, lParam );
			if( *pbNoFurtherProcessing )
				return 0;
			break;
	}
}


//--------------------------------------------------------------------------------------
// Function: OnKeyboard
// When a key on the keyboard is pressed or released, this function is called.
//--------------------------------------------------------------------------------------
void CALLBACK RacerApp::OnKeyboard ( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
	if( g_pGame && m_AppState != APP_STARTUP )
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
	switch( nControlID )
	{
		case GUI_BTN_SINGLE_PLAYER:
			g_audioState.pSoundBank->Play(g_audioState.iApplicationStart, 0, 0, NULL);
			m_AppState = APP_RENDER_GAME; 
			break;
	}
}


//--------------------------------------------------------------------------------------
// Function: OnResetDevice
// This function is called when the window size changes.  In here we want to reset
// all objects that rely on the back buffer size.
//--------------------------------------------------------------------------------------
HRESULT CALLBACK RacerApp::OnResetDevice( IDirect3DDevice9* device, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	HRESULT hr;

	V_RETURN( m_ResourceManager.OnD3D9ResetDevice() );

	if( g_pGame )
		g_pGame->processCallback( ON_RESET, device, pBackBufferSurfaceDesc );

	//initialize the size and position of the startup menu based on size of device surface
	m_MenuScreen.SetLocation( ( pBackBufferSurfaceDesc->Width - 300 ) / 2,
                             ( pBackBufferSurfaceDesc->Height - 200 ) / 2 );
    m_MenuScreen.SetSize	( 300, 200 );

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Function:  OnLostDevice
// This function is called right before the device is destroyed.  We want to prepare 
// anything attached to the device for the upcoming destruction.
//--------------------------------------------------------------------------------------
void CALLBACK RacerApp::OnLostDevice(void* pUserContext )
{
	m_ResourceManager.OnD3D9LostDevice();

	if( g_pGame )
		g_pGame->processCallback( ON_LOST );
}



//--------------------------------------------------------------------------------------
// Function: OnRender
// This callback function is called every frame to render the scene.  
//--------------------------------------------------------------------------------------
void CALLBACK RacerApp::OnRender( Device* device, double dTime, float fElapsedTime, void* pUserContext  )
{
	HRESULT hr;

	// Clear the render target and the zbuffer 
	V( device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 255, 0, 0, 0 ), 1.0f, 0 ) );

	// Render the scene
	if( SUCCEEDED( device->BeginScene() ) )
	{
		switch( m_AppState )
		{
			case APP_STARTUP:
				//render the game menu
				V( m_MenuScreen.OnRender( fElapsedTime ) );
				break;
			
			case APP_RENDER_GAME:
				//get the game to render all of its components
				g_pGame->render( device );
				break;
		}
	}
	V( device->EndScene() );
}


//--------------------------------------------------------------------------------------
// Function: OnCreateDevice
// This function is called when a device is created.  We are using this function
// as the place to initialize the game (by initializing its components with the device)
//--------------------------------------------------------------------------------------
HRESULT CALLBACK RacerApp::OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	HRESULT hr;

	V_RETURN( m_ResourceManager.OnD3D9CreateDevice( pd3dDevice ) );
	
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
	m_ResourceManager.OnD3D9DestroyDevice();

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
	m_MenuController = new XBox360Controller(1); 

	m_AppState = APP_STARTUP;

	// Set up the audio
	HRESULT hr = PrepareXACT();
	
	//initialize the resource manager to keep track of all our screens and HUD
	m_MenuScreen.Init( &m_ResourceManager );
    m_OnePlayerScreen.Init( &m_ResourceManager );

	//g_audioState.pSoundBank->Play(g_audioState.iApplicationStart, 0, 0, NULL);
	//Set the font of the menu screen
    m_MenuScreen.SetFont( 1, L"Arial", 24, FW_BOLD );
    CDXUTElement* pElement = m_MenuScreen.GetDefaultElement( DXUT_CONTROL_STATIC, 0 );
    if( pElement )
    {
        pElement->iFont = 1;
        pElement->dwTextFormat = DT_CENTER | DT_BOTTOM;
    }

    m_MenuScreen.SetCallback( OnGUIEvent ); 
	
	//Add buttons/text to the menu screen
	m_MenuScreen.AddStatic( -1, L"Select your game mode:", 0, 10, 300, 22 );
    m_MenuScreen.AddButton( GUI_BTN_SINGLE_PLAYER, L"Single Player", 90, 42, 125, 40 );
}


//--------------------------------------------------------------------------------------
RacerApp::~RacerApp()
{
	if( g_pGame )
	{
		g_pGame->processCallback( ON_LOST );
		g_pGame->processCallback( ON_DESTROY ); //release the memory/objects used by the game
		delete g_pGame;
		g_pGame = NULL;
	}
	if( m_MenuController )
		delete m_MenuController;

}