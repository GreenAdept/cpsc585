//--------------------------------------------------------------------------------------
// File: CraterRacerApp.cpp
//
//--------------------------------------------------------------------------------------
#include "CraterRacerApp.h"
#include "MessageManager.h"

GameObj* g_pGame = NULL;	//global game used by the RacerApp class 

//instatiating all the static member variables used by RacerApp
ApplicationState		RacerApp::m_AppState;
XBox360Controller*		RacerApp::m_MenuController;
UINT					RacerApp::m_uiCurrentButton;
SceneLoader*			RacerApp::m_SceneLoader;
HANDLE					RacerApp::m_hThread;
DWORD					RacerApp::m_dwThreadID;
bool					RacerApp::m_bGameIsReady;
CRITICAL_SECTION		RacerApp::m_CriticalSection;
int						RacerApp::m_iCurrentBall;
UINT_PTR				RacerApp::m_AnimationID;
float					RacerApp::m_fCheckTime;
ApplicationState		RacerApp::m_iPreviousAppState;
Renderer*				RacerApp::m_Renderer;			// rendering subsystem
bool					RacerApp::m_bIsLoading;

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
	DWORD dwReturnVal;

	if( m_bIsLoading )
		startGame( 0 );

	else
	{
		//The game is active, so go into game loop processing
		if( g_pGame && m_AppState == APP_RENDER_GAME )
		{
			g_pGame->processInput( fElapsedTime );
			g_pGame->simulate( fElapsedTime );
			g_pGame->think( );
			UpdateAudio();
		}

		else if( g_pGame && m_AppState == APP_GAME_LOADING )
		{
			if( m_bGameIsReady )
			{
				//do start game animation/countdown here, but until we
				//have one, we'll just start the game

				//make sure the thread to load the game will not run anymore
				//HWND fWindow = FindWindow(NULL, L"Crater Racer"); // Find the window
				//SuspendThread( m_hThread );

				//stop loading animation
				//KillTimer( fWindow, m_AnimationID );

				//start background music
				if( g_audioState.pSoundBank )
				{
					g_audioState.pSoundBank->Play(g_audioState.iGameStart, 0, 0, NULL);
					g_audioState.pSoundBank->Play(g_audioState.iEngine, 0, 0, NULL);
				}

				//start game
				Emit( EStartClock );//g_pGame->startClock();
				m_AppState = APP_RENDER_GAME;
			}
		}
		else
			doControllerProcessing = true;
		
		//just check the controller if a certain amount of time has passed
		m_fCheckTime += fElapsedTime;
		if( doControllerProcessing && m_fCheckTime > 0.18 )
		{
			m_MenuController->Update( m_fCheckTime );
			//move menu up
			if( m_MenuController->LeftThumbstick.GetY() > 0.2  )
				moveMenuUp( );
			//move menu down
			else if( m_MenuController->LeftThumbstick.GetY() < -0.2 )
				moveMenuDown( );
			
			//A selection was made using the A button
			if( m_MenuController->A.WasPressed() )
				processMenuSelection( );

			m_fCheckTime = 0.0f;
		}
	}
}


//--------------------------------------------------------------------------------------
// Function: OnKeyboard
// When a key on the keyboard is pressed or released, this function is called.
//--------------------------------------------------------------------------------------
void CALLBACK RacerApp::OnKeyboard ( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
	//Terminate game if esc key is pressed (and cleanup all memory first)
	if( nChar == VK_ESCAPE )
		DXUTShutdown( 0 );

	if( g_pGame && m_AppState == APP_RENDER_GAME && bKeyDown )
	{
		// online virtual key codes: http://msdn.microsoft.com/en-us/library/ms927178.aspx
		switch( nChar ) 
		{
			case  VK_SPACE:
				//pause game here
				Emit( EPauseGame );

				//g_pGame->pauseGame();
				m_uiCurrentButton = GUI_BTN_UNPAUSE;
				m_AppState = APP_PAUSED;
				break;

			default:
				break;
		}
	}
	else if( bKeyDown ) //A menu is active, so we want check for button selection
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
	if( m_AppState == APP_STARTUP && m_uiCurrentButton != GUI_BTN_SINGLE_PLAYER ||
		g_pGame && m_AppState == APP_PAUSED && m_uiCurrentButton != GUI_BTN_UNPAUSE ||
		m_AppState == APP_VICTORY && m_uiCurrentButton != GUI_BTN_MAINMENU )
	{
		//send message to current button to go back to normal state
		m_Renderer->adjustButtonImage( m_uiCurrentButton, -1 );
		m_uiCurrentButton--;
		m_Renderer->adjustButtonImage( m_uiCurrentButton, +1 );
	}
}


//--------------------------------------------------------------------------------------
// Function: moveMenuDown
// Moves the menu button selection down (if not at the bottom of the menu already)
//--------------------------------------------------------------------------------------
void RacerApp::moveMenuDown( )
{
	if( m_AppState == APP_STARTUP && m_uiCurrentButton != GUI_BTN_EXIT ||
		g_pGame && m_AppState == APP_PAUSED && m_uiCurrentButton != GUI_BTN_EXIT2 ||
		m_AppState == APP_VICTORY && m_uiCurrentButton != GUI_BTN_EXITSMALL )
	{
		//tell current button to go back to normal/unselected state
		m_Renderer->adjustButtonImage( m_uiCurrentButton, -1 );
		m_uiCurrentButton++;
		m_Renderer->adjustButtonImage( m_uiCurrentButton, +1 );
	}
}


//--------------------------------------------------------------------------------------
// Function: processMenuSelection( )
// Processes the loading of the game or exiting, based on current button selected.
//--------------------------------------------------------------------------------------
void RacerApp::processMenuSelection( )
{
		HWND fWindow; // Window handle to our window

		if( m_AppState == APP_SHOW_GAMERULES || m_AppState == APP_SHOW_GAMERULES2 )
			m_AppState = m_iPreviousAppState;

		else
		{
			//a selection was made
			switch( m_uiCurrentButton )
			{
				//start the game
				case GUI_BTN_SINGLE_PLAYER:
				case GUI_BTN_TWO_PLAYER:
				case GUI_BTN_TIMETRIAL:

					//start loading game in a separate thread
					m_AppState = APP_GAME_LOADING;
					//m_hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)startGame,NULL,0,&m_dwThreadID);

					//start ball loading animation
					//fWindow = FindWindow(NULL, L"Crater Racer");
					//SetTimer( fWindow, m_AnimationID, 500, (TIMERPROC)animateBall);
					break;
					
				case GUI_BTN_UNPAUSE:
					Emit( EUnpauseGame );
					m_AppState = APP_RENDER_GAME; 
					break;
				
				case GUI_BTN_MAINMENU:
					m_AppState = APP_STARTUP; 
					m_uiCurrentButton = GUI_BTN_SINGLE_PLAYER;
					break;

				case GUI_BTN_GAMERULES:
					m_iPreviousAppState = m_AppState;
					m_AppState = APP_SHOW_GAMERULES;
					break;

				//in pause screen
				case GUI_BTN_GAMERULES2: 
					m_iPreviousAppState = m_AppState;
					m_AppState = APP_SHOW_GAMERULES2;
					break;

				//or exit 
				case GUI_BTN_EXIT:
				case GUI_BTN_EXITSMALL:
				case GUI_BTN_EXIT2:
					DXUTShutdown();
					break;
			}
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

	if( g_pGame )
		g_pGame->processCallback( ON_RESET, device, pBackBufferSurfaceDesc );

	m_Renderer->OnReset( device, pBackBufferSurfaceDesc );

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

	m_Renderer->OnLost( );
}



//--------------------------------------------------------------------------------------
// Function: OnRender
// This callback function is called every frame to render the scene.  
//--------------------------------------------------------------------------------------
void CALLBACK RacerApp::OnRender( Device* device, double dTime, float fElapsedTime, void* pUserContext  )
{
	HRESULT hr;
	
	// Clear the render target and the zbuffer 
	V( device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 255,0,0,0 ), 1.0f, 0 ) );

	// Render the scene
	if( SUCCEEDED( device->BeginScene() ) )
	{
		switch( m_AppState )
		{
			case APP_STARTUP:
				
				//m_Renderer->drawVictoryScreen( );
				m_Renderer->drawStartupMenu( );
				break;

			case APP_SHOW_GAMERULES:
				m_Renderer->drawGameRules( );
				break;

			case APP_GAME_LOADING:

				m_Renderer->drawLoadingScreen( );
				
				m_bIsLoading = m_bIsLoading ? false : true;
				break;

			case APP_VICTORY:

				m_Renderer->drawVictoryScreen( );
				break;

			case APP_RENDER_GAME:
			case APP_SHOW_GAMERULES2:
			case APP_PAUSED:

				if( g_pGame )
				{
					//Adjust all HUD images to reflect current state
					m_Renderer->adjustRankImage( g_pGame->m_Victory.getRank( PLAYER1 ) );
					m_Renderer->adjustClockImages( g_pGame->getTime() );
					m_Renderer->adjustSpeedImage( g_pGame->getVehicleSpeed( PLAYER1 ) );

					//get the game to render all of its components
					g_pGame->render( device, m_Renderer );

					m_Renderer->renderFPS( );
					m_Renderer->drawHUD( );
				}

				if( m_AppState == APP_PAUSED )
					m_Renderer->drawPauseScreen( );

				if( m_AppState == APP_SHOW_GAMERULES2 )
					m_Renderer->drawPauseGameRules( );
				
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

	m_Renderer->OnCreate( device );
	m_SceneLoader->initVars( device, pBackBufferSurfaceDesc, INIT_SCENE_FILE );

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

	m_Renderer->OnDestroy( );
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
// Function: startGame
// Function call started by initialization thread.
//--------------------------------------------------------------------------------------
long WINAPI RacerApp::startGame( long lParam )
{
	//Enter the critical section -- other threads are locked out
    //EnterCriticalSection(&m_CriticalSection);
 
	m_SceneLoader->initScene( &g_pGame );

	m_SceneLoader->startGame( ONE_PLAYER_SCENE_FILE ); //load one player game

	//Leave the critical section -- other threads can now EnterCriticalSection() 
   // LeaveCriticalSection(&m_CriticalSection);


	m_bGameIsReady = true;
	InitEmitter( g_pGame );

	return 0;
}


//--------------------------------------------------------------------------------------
// Function: animateBall
// Increments the big ball location in the loading screen animation.  This is the timer
// callback function started in the KeyPress event function.
//--------------------------------------------------------------------------------------
void RacerApp::animateBall( )
{
	m_Renderer->adjustBallImage( m_iCurrentBall, -1 );
	m_iCurrentBall++;

	if( m_iCurrentBall >= NUM_LOADING_BALLS )
		m_iCurrentBall = 0;

	m_Renderer->adjustBallImage( m_iCurrentBall, +1 );
}


//--------------------------------------------------------------------------------------
// Function: doLoadScreen
// Here is where we will start the loading animation
//--------------------------------------------------------------------------------------
void RacerApp::doLoadScreen( )
{


}


//--------------------------------------------------------------------------------------
// Function: CONSTRUCTOR
//--------------------------------------------------------------------------------------
RacerApp::RacerApp()
{
	m_MenuController = new XBox360Controller(0);
	m_SceneLoader = new SceneLoader( );

	//Initialize the critical section before entering multi-threaded context.
	InitializeCriticalSection(&m_CriticalSection);

	g_pGame			  = NULL;
	m_AppState		  = APP_STARTUP;
	m_uiCurrentButton = GUI_BTN_SINGLE_PLAYER;
	m_iCurrentBall	  = 0;
	m_fCheckTime	  = 0.0f;
	m_Renderer		  = new Renderer( );
	m_bIsLoading	  = false;

	// Set up the audio
	HRESULT hr = PrepareXACT( BG_WAVEBANK_FILE, SE_WAVEBANK_FILE, BG_SETTINGS_FILE, BG_SOUNDBANK_FILE );
}


//--------------------------------------------------------------------------------------
// Function: DECONSTRUCTOR
//--------------------------------------------------------------------------------------
RacerApp::~RacerApp()
{
	cleanupAll();
}


//--------------------------------------------------------------------------------------
// Function: cleanupAll( )
// Clears up all memory used by game and releases all devices.
//--------------------------------------------------------------------------------------
void RacerApp::cleanupAll( )
{
	if( g_pGame )
	{
		OnLostDevice( NULL );
		OnDestroyDevice( NULL );
		delete g_pGame;
		g_pGame = NULL;
	}
	if( m_MenuController )
		delete m_MenuController;

	if( m_Renderer )
		delete m_Renderer;

	if( m_SceneLoader )
		delete m_SceneLoader;

	if( m_hThread )
			CloseHandle( m_hThread );

	// Release system object when all finished 
	DeleteCriticalSection(&m_CriticalSection);
}