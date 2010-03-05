//--------------------------------------------------------------------------------------
// File: CraterRacerApp.cpp
//
//--------------------------------------------------------------------------------------
#include "CraterRacerApp.h"

GameObj* g_pGame = NULL;	//global game used by the RacerApp class 

//instatiating all the static member variables used by RacerApp
ResourceManager			RacerApp::m_ResourceManager;
ApplicationState		RacerApp::m_AppState;
Dialog					RacerApp::m_MenuScreen;
Dialog					RacerApp::m_OnePlayerScreen;
Dialog					RacerApp::m_PauseScreen;
XBox360Controller*		RacerApp::m_MenuController;
vector<CDXUTButton*>	RacerApp::buttons;
UINT					RacerApp::m_uiCurrentButton;
SceneLoader*			RacerApp::m_SceneLoader;


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
	bool doControllerProcessing = false;

	//The game is active, so go into game loop processing
	if( g_pGame && m_AppState ==  APP_RENDER_GAME )
	{
		g_pGame->processInput( fElapsedTime );

		//check if any of the players paused the game
		if( g_pGame->isPaused( ) )
		{
			m_AppState = APP_PAUSED;
			m_uiCurrentButton = GUI_BTN_UNPAUSE;
			return;
		}
		g_pGame->simulate( fElapsedTime );
		g_pGame->think( );
		UpdateAudio();
	}
	else if( m_AppState == APP_STARTUP )
	{
		//The menu is active, so check for controller movement
		m_MenuController->Update(fElapsedTime);
		doControllerProcessing = true;
	}
	else if( g_pGame )
		doControllerProcessing = true;
	
	if( doControllerProcessing )
	{
		m_MenuController->Update( fElapsedTime );
		//move menu up
		if( m_MenuController->LeftThumbstick.GetY() > 0.2  )
			moveMenuUp( );
		//move menu down
		else if( m_MenuController->LeftThumbstick.GetY() < -0.2 )
			moveMenuDown( );
		
		//a selection was made
		if( m_MenuController->A.WasPressedOrHeld() )
			processMenuSelection( );
	}
}


//--------------------------------------------------------------------------------------
// Function: OnKeyboard
// When a key on the keyboard is pressed or released, this function is called.
//--------------------------------------------------------------------------------------
void CALLBACK RacerApp::OnKeyboard ( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
	if( g_pGame && m_AppState == APP_RENDER_GAME )
	{
		// online virtual key codes: http://msdn.microsoft.com/en-us/library/ms927178.aspx
		switch( nChar ) 
		{
			case  VK_SPACE:
				//pause game here
				//if( g_pGame->pauseGame( true ) )
				if (g_pGame->pauseGame())
				{
					m_uiCurrentButton = GUI_BTN_UNPAUSE;
					m_AppState = APP_PAUSED;
				}
				break;
			default:
				g_pGame->addInput( bKeyDown, nChar );
				break;
		}
	}
	else //The menu is active, so we want check for button selection
	{ 
		switch( nChar )
		{
			case VK_UP:
				moveMenuUp( );				
				break;

			case VK_DOWN:
				moveMenuDown( );
				break;

			case VK_RETURN:
				processMenuSelection( );
		}
	}
}


//--------------------------------------------------------------------------------------
// Function: moveMenuUp
// Moves the menu button selection up (if it is not at the top of the menu already)
//--------------------------------------------------------------------------------------
void RacerApp::moveMenuUp( )
{
	if( m_AppState == APP_STARTUP && m_uiCurrentButton != GUI_BTN_SINGLE_PLAYER )
	{
		//send message to current button to go back to normal state
		buttons[ m_uiCurrentButton ]->OnMouseLeave();
		m_uiCurrentButton--;
		//send message to newly selected button to go to mouse-over state (so it will render differently)
		buttons[ m_uiCurrentButton ]->OnMouseEnter();
	}
	else if( g_pGame && m_AppState == APP_PAUSED && m_uiCurrentButton != GUI_BTN_UNPAUSE )
	{
		//send message to current button to go back to normal state
		buttons[ m_uiCurrentButton ]->OnMouseLeave();
		m_uiCurrentButton--;
		//send message to newly selected button to go to mouse-over state (so it will render differently)
		buttons[ m_uiCurrentButton ]->OnMouseEnter();
	}
}


//--------------------------------------------------------------------------------------
// Function: moveMenuDown
// Moves the menu button selection down (if not at the bottom of the menu already)
//--------------------------------------------------------------------------------------
void RacerApp::moveMenuDown( )
{
	if( m_AppState == APP_STARTUP && m_uiCurrentButton != GUI_BTN_EXIT )
	{
		//tell current button to go back to normal/unselected state
		buttons[ m_uiCurrentButton ]->OnMouseLeave();
		m_uiCurrentButton++;
		//tell the new button to go in mouse-over state (so it will render differently)
		buttons[ m_uiCurrentButton ]->OnMouseEnter();
	}
	else if( g_pGame && m_AppState == APP_PAUSED && m_uiCurrentButton != GUI_BTN_EXIT2 )
	{
		//send message to current button to go back to normal state
		buttons[ m_uiCurrentButton ]->OnMouseLeave();
		m_uiCurrentButton++;
		//send message to newly selected button to go to mouse-over state (so it will render differently)
		buttons[ m_uiCurrentButton ]->OnMouseEnter();
	}
}

//--------------------------------------------------------------------------------------
// Function: processMenuSelection( )
// Processes the loading of the game or exiting, based on current button selected.
//--------------------------------------------------------------------------------------
void RacerApp::processMenuSelection( )
{
		//a selection was made
		switch( m_uiCurrentButton )
		{
			//start the game
			case GUI_BTN_SINGLE_PLAYER:
				if( g_audioState.pSoundBank )
					g_audioState.pSoundBank->Play(g_audioState.iGameStart, 0, 0, NULL);

				g_pGame = m_SceneLoader->startGame( ONE_PLAYER_SCENE_FILE ); //load one player game
				g_pGame->startClock();

				m_AppState = APP_RENDER_GAME; 
				break;
			//start game again
			case GUI_BTN_UNPAUSE:
				g_pGame->pauseGame( );
				m_AppState = APP_RENDER_GAME; 
				break;

			//or exit 
			case GUI_BTN_EXIT:
			case GUI_BTN_EXIT2:
				DXUTShutdown();
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
	
	//initialize the size and position of the pause screen, too
	m_PauseScreen.SetLocation( ( pBackBufferSurfaceDesc->Width - 300 ) / 2,
                             ( pBackBufferSurfaceDesc->Height - 200 ) / 2 );
    m_PauseScreen.SetSize	( 300, 200 );

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
			case APP_PAUSED:

				if( g_pGame )
					//get the game to render all of its components
					g_pGame->render( device );

				if( m_AppState == APP_PAUSED )
					m_PauseScreen.OnRender( fElapsedTime );
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
HRESULT CALLBACK RacerApp::OnCreateDevice( Device* device, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	HRESULT hr;

	//initialize the resource manager for our HUD, Pause Screen and Menu Screen.
	V_RETURN( m_ResourceManager.OnD3D9CreateDevice( device ) );

	m_SceneLoader->initScene( device, pBackBufferSurfaceDesc, INIT_SCENE_FILE );

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
RacerApp::RacerApp()
{
	m_MenuController = new XBox360Controller(0);
	m_SceneLoader = new SceneLoader( );

	g_pGame = NULL;
	m_AppState = APP_STARTUP;

	// Set up the audio
	HRESULT hr = PrepareXACT( BG_WAVEBANK_FILE, BG_SOUNDBANK_FILE );

	//initialize the resource manager to keep track of all our screens and HUD
	m_MenuScreen.Init( &m_ResourceManager );
    m_OnePlayerScreen.Init( &m_ResourceManager );
	m_PauseScreen.Init( &m_ResourceManager );

	//g_audioState.pSoundBank->Play(g_audioState.iApplicationStart, 0, 0, NULL);
	//Set the font of the menu screen
    m_MenuScreen.SetFont( 1, L"Arial", 24, FW_BOLD );
    CDXUTElement* pElement = m_MenuScreen.GetDefaultElement( DXUT_CONTROL_STATIC, 0 );
    if( pElement )
    {
        pElement->iFont = 1;
        pElement->dwTextFormat = DT_CENTER | DT_BOTTOM;
    } 

	//Add buttons/text to the menu screen
	m_MenuScreen.AddStatic( -1, L"SELECT YOUR GAME MODE:", 20, 10, 280, 22 );
    m_MenuScreen.AddButton( GUI_BTN_SINGLE_PLAYER, L"Single Player", 20, 55, 280, 60 );
	m_MenuScreen.AddButton( GUI_BTN_EXIT, L"Exit", 20, 125, 280, 60 );
	
	//Add buttons/text to the menu screen
    m_PauseScreen.AddButton( GUI_BTN_UNPAUSE, L"Return to Game", 20, 55, 280, 60 );
	m_PauseScreen.AddButton( GUI_BTN_EXIT2, L"Exit Game", 20, 125, 280, 60 );

	//Save buttons for easy access later
	buttons.push_back( m_MenuScreen.GetButton( GUI_BTN_SINGLE_PLAYER ) );
	buttons.push_back( m_MenuScreen.GetButton( GUI_BTN_EXIT ) );
	buttons.push_back( m_PauseScreen.GetButton( GUI_BTN_UNPAUSE ) );
	buttons.push_back( m_PauseScreen.GetButton( GUI_BTN_EXIT2 ) );
	buttons[ GUI_BTN_SINGLE_PLAYER ]->OnMouseEnter();
	buttons[ GUI_BTN_UNPAUSE ]->OnMouseEnter();
	
	//The currently highlighted button
	m_uiCurrentButton = GUI_BTN_SINGLE_PLAYER;
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

	if( m_SceneLoader )
		delete m_SceneLoader;
}