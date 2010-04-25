//--------------------------------------------------------------------------------------
// File: CraterRacerApp.cpp
//
//--------------------------------------------------------------------------------------
#include "CraterRacerApp.h"
#include "MessageManager.h"

GameObj* g_pGame = NULL;	//global game used by the RacerApp class 

//instatiating all the static member variables used by RacerApp
ApplicationState		RacerApp::m_AppState;
XBox360Controller*   	RacerApp::m_Controllers [2];
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
int						RacerApp::m_iBackWidth;
int						RacerApp::m_iBackHeight;
bool					RacerApp::m_bIsTwoPlayer;
bool					RacerApp::m_bIsTimeTrial;
string					RacerApp::m_sGameFilename;
int						RacerApp::m_iCount;
Clock*					RacerApp::m_Clock;
int						RacerApp::m_iLastTime;
D3DVIEWPORT9			RacerApp::m_ResetViewPort;
D3DVIEWPORT9			RacerApp::m_TopViewPort;
D3DVIEWPORT9			RacerApp::m_BottomViewPort;

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
	
	//UpdateAudio(m_AppState);
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
		}
		else if (g_pGame && m_AppState == APP_VICTORY) {
			Emit(Events::EVictoryScreenUpdate);
			g_pGame->simulate( fElapsedTime );
			g_pGame->think( );
			doControllerProcessing = true;
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

				//start game
				m_Clock->start();
				m_iLastTime = m_Clock->getTotalTimeInMS();
				m_iCount = 3;
				Emit(Events::EStartOrStopRace, 0);
				//Emit(Events::EGameFinished);
				m_AppState = APP_RENDER_GAME;
			}
		}
		else
			doControllerProcessing = true;
		
		//just check the controller if a certain amount of time has passed
		m_fCheckTime += fElapsedTime;
		if( doControllerProcessing && m_fCheckTime > 0.18 )
		{
			for (int i = 0; i < 2; i++)
				if (m_Controllers[i]->IsConnected()) {
					m_Controllers[i]->Update( m_fCheckTime );
					//move menu up
					if( m_Controllers[i]->LeftThumbstick.GetY() > 0.2  )
						moveMenuUp( );
					//move menu down
					else if( m_Controllers[i]->LeftThumbstick.GetY() < -0.2 )
						moveMenuDown( );
					
					//A selection was made using the A button
					if( m_Controllers[i]->A.WasPressed() )
						processMenuSelection( );
				}
			m_fCheckTime = 0.0f;
		}
	}
}

//--------------------------------------------------------------------------------------
// Function: countDown
// Starts the count down (3, 2, 1, GO!) 2 seconds after getting called.
//--------------------------------------------------------------------------------------
void RacerApp::countDown( ) {
	int nowTime = m_Clock->getTotalTimeInMS();
	int waitTime = 1500;
	if ( m_iCount == 3)
		waitTime = 2500;
	if ( m_iCount == 0)
		waitTime = 1000;
	if ((nowTime - m_iLastTime) >= waitTime)
	{
		m_iLastTime = nowTime;
		m_iCount--;
	}
	if (m_iCount == 0) {
		Emit(Events::EStartOrStopRace, 1);
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
	VarLoader* v;

	if( g_pGame && m_AppState == APP_RENDER_GAME && bKeyDown )
	{
		// online virtual key codes: http://msdn.microsoft.com/en-us/library/ms927178.aspx
		switch( nChar ) 
		{
			case  VK_SPACE:
					Emit( EPauseGame );
				break;

			case 0x4C:
				v = new VarLoader();
				v->loadVars( m_Renderer );
				delete v;
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
	if (m_AppState == APP_ENTERNAME) {
		m_uiCurrentButton++;
		m_uiCurrentButton = m_uiCurrentButton % 26;
	}

	else if( m_AppState == APP_STARTUP && m_uiCurrentButton != GUI_BTN_SINGLE_PLAYER ||
			g_pGame && m_AppState == APP_PAUSED && m_uiCurrentButton != GUI_BTN_UNPAUSE ||
			m_AppState == APP_VICTORY && m_uiCurrentButton != GUI_BTN_MAINMENU ||
			m_AppState == APP_SHOWTIMES && m_uiCurrentButton != GUI_BTN_MAINMENU )
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
	if (m_AppState == APP_ENTERNAME) {
		m_uiCurrentButton--;
		if (m_uiCurrentButton == -1) {
			m_uiCurrentButton = 25;
		}
	}

	else if( m_AppState == APP_STARTUP && m_uiCurrentButton != GUI_BTN_EXIT ||
			g_pGame && m_AppState == APP_PAUSED && m_uiCurrentButton != GUI_BTN_EXIT2 ||
			m_AppState == APP_VICTORY && m_uiCurrentButton != GUI_BTN_EXITSMALL ||
			m_AppState == APP_SHOWTIMES && m_uiCurrentButton != GUI_BTN_EXITSMALL )
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

		if( m_AppState == APP_SHOW_GAMERULES || m_AppState == APP_SHOW_GAMERULES2 ) {
			m_AppState = m_iPreviousAppState;
		}

		else if (m_AppState == APP_ENTERNAME) {
			m_Renderer->addLetter( m_uiCurrentButton + (int)'A');
		}

		else
		{
			//a selection was made
			switch( m_uiCurrentButton )
			{
				//start the game
				case GUI_BTN_TWO_PLAYER:
					m_Renderer->adjustButtonImage( m_uiCurrentButton, -1 );
					m_sGameFilename = TWO_PLAYER_SCENE_FILE;
					m_AppState = APP_GAME_LOADING;
					m_Renderer->adjustTwoPlayer( true, m_iBackWidth, m_iBackHeight );
					m_bIsTwoPlayer = true;
					m_bIsTimeTrial = false;
					break;

				case GUI_BTN_TIMETRIAL:
					m_Renderer->adjustButtonImage( m_uiCurrentButton, -1 );
					m_Renderer->adjustTwoPlayer( false, m_iBackWidth, m_iBackHeight );
					m_sGameFilename = TIME_TRIAL_SCENE_FILE;
					m_AppState = APP_GAME_LOADING;
					m_Renderer->adjustTimeTrial( );
					m_bIsTwoPlayer = false;
					m_bIsTimeTrial = true;
					break;

				case GUI_BTN_SINGLE_PLAYER:
					m_Renderer->adjustButtonImage( m_uiCurrentButton, -1 );
					m_Renderer->adjustTwoPlayer( false, m_iBackWidth, m_iBackHeight );
					m_sGameFilename = ONE_PLAYER_SCENE_FILE;
					m_AppState = APP_GAME_LOADING;
					m_bIsTwoPlayer = false;
					m_bIsTimeTrial = false;
					break;

					//start loading game in a separate thread
					//m_AppState = APP_GAME_LOADING;
					//m_hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)startGame,NULL,0,&m_dwThreadID);

					//start ball loading animation
					//fWindow = FindWindow(NULL, L"Crater Racer");
					//SetTimer( fWindow, m_AnimationID, 500, (TIMERPROC)animateBall);
					//break;
					
				case GUI_BTN_UNPAUSE:
					Emit( EUnpauseGame );
					m_AppState = APP_RENDER_GAME; 
					break;
				
				case GUI_BTN_PAUSE_MAINMENU:
				case GUI_BTN_MAINMENU:
					m_Renderer->adjustButtonImage( m_uiCurrentButton, -1 );
					m_uiCurrentButton = GUI_BTN_SINGLE_PLAYER;
					m_Renderer->adjustButtonImage( m_uiCurrentButton, +1 );
					m_AppState = APP_STARTUP; 
					m_bIsTwoPlayer = false;
					m_bIsTimeTrial= false;
					m_Renderer->adjustTwoPlayer( false, m_iBackWidth, m_iBackHeight );
					m_Renderer->adjustLapImage( PLAYER1, 1 );
					m_Renderer->adjustLapImage( PLAYER2, 1 );
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

	m_iBackWidth = pBackBufferSurfaceDesc->Width;
	m_iBackHeight = pBackBufferSurfaceDesc->Height;

	//Set the default viewport and perspective matrices
	D3DXMATRIX matView;
    D3DXMatrixLookAtLH( &matView, &D3DXVECTOR3(0.0f, 0.0f,-10.0f), 
		                          &D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
		                          &D3DXVECTOR3(0.0f, 1.0f, 0.0f ) );
    device->SetTransform( D3DTS_VIEW, &matView );

    D3DXMATRIX matProj;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DXToRadian( 45.0f ), 1.0f, 1.0f, 200.0f );
    device->SetTransform( D3DTS_PROJECTION, &matProj );
	device->SetRenderState( D3DRS_LIGHTING,  FALSE );

	//Initialize viewport for one player and time trial games (fills entire screen)
	m_ResetViewPort;
	m_ResetViewPort.X      = 0;
    m_ResetViewPort.Y      = 0;
    m_ResetViewPort.Width  = m_iBackWidth;
    m_ResetViewPort.Height = m_iBackHeight;
    m_ResetViewPort.MinZ   = 0.0f;
    m_ResetViewPort.MaxZ   = 1.0f;

	//Initialize two player viewports (top half and bottom half)
    m_TopViewPort.X      = 0;
    m_TopViewPort.Y      = 0;
    m_TopViewPort.Width  = m_iBackWidth;
    m_TopViewPort.Height = m_iBackHeight / 2;
    m_TopViewPort.MinZ   = 0.0f;
    m_TopViewPort.MaxZ   = 1.0f;

	m_BottomViewPort.X      = 0;
    m_BottomViewPort.Y      = m_iBackHeight / 2;
    m_BottomViewPort.Width  = m_iBackWidth;
    m_BottomViewPort.Height = m_iBackHeight / 2;
    m_BottomViewPort.MinZ   = 0.0f;
    m_BottomViewPort.MaxZ   = 1.0f;

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
// Function:  ModeifyDeviceSettings
// This function is called before the device is created, so in here we can put conditions
// on the device.  We use it to turn off vsync.
//--------------------------------------------------------------------------------------
bool CALLBACK RacerApp::ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
    assert( DXUT_D3D9_DEVICE == pDeviceSettings->ver );

    HRESULT hr;
    IDirect3D9* pD3D = DXUTGetD3D9Object();
    D3DCAPS9 caps;

    V( pD3D->GetDeviceCaps( pDeviceSettings->d3d9.AdapterOrdinal,
                            pDeviceSettings->d3d9.DeviceType,
                            &caps ) );

    // Turn vsync off by uncommenting this line.
    //pDeviceSettings->d3d9.pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    return true;
}


//--------------------------------------------------------------------------------------
// Function: renderTwoPlayer
// This function renders the game and HUD for the first player (in top of screen) and
// second player (in bottom of screen) using viewports.
//--------------------------------------------------------------------------------------
void RacerApp::renderTwoPlayer( Device* device )
{
	// Render first player in top half of screen
	g_pGame->render( device, m_Renderer, PLAYER1, &m_TopViewPort );

	m_Renderer->renderFPS( );
	m_Renderer->drawHUD( PLAYER1 );

    device->EndScene();
 
    // Render second player to the bottom half of the screen
    device->BeginScene();
    {
		g_pGame->render( device, m_Renderer, PLAYER2, &m_BottomViewPort );
		m_Renderer->drawHUD( PLAYER2 );
    }

	device->SetViewport( &m_ResetViewPort );
}


//--------------------------------------------------------------------------------------
// Function: OnRender
// This callback function is called every frame to render the scene.  
//--------------------------------------------------------------------------------------
void CALLBACK RacerApp::OnRender( Device* device, double dTime, float fElapsedTime, void* pUserContext  )
{
	HRESULT hr;
	vector<Particle*> particles;
	Particle* p, *p2, *p3;

	// Clear the render target and the zbuffer 
	V( device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 255,0,0,0 ), 1.0f, 0 ) );

	// Render the scene
	if( SUCCEEDED( device->BeginScene() ) )
	{
		g_audioState.gameState = m_AppState;
		switch( m_AppState )
		{
			case APP_STARTUP:
				//Testing particle rendering in startup screen...
				p = new Particle();
				p->position = Vec3(-3,0,0);
				particles.push_back( p );
				p2 = new Particle();
				p2->position = Vec3(-2.5,0.5,0);
				particles.push_back( p2 );
				p3 = new Particle();
				p3->position = Vec3(-2.6,-1.0,0);
				particles.push_back( p3 );
				m_Renderer->drawParticles( device, particles );
				delete p;
				delete p2;
				delete p3;

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

			case APP_SHOWTIMES:
			case APP_ENTERNAME:

				m_Renderer->drawTimesScreen(m_uiCurrentButton + (int)'A');
				break;
				
			case APP_RENDER_GAME:
			case APP_SHOW_GAMERULES2:
			case APP_PAUSED:

				if( g_pGame )
				{
					//Adjust all HUD images to reflect current state
					if (!m_bIsTimeTrial)
						m_Renderer->adjustRankImage( g_pGame->m_Victory.getRank( PLAYER1 ), PLAYER1 );

					m_Renderer->adjustClockImages( g_pGame->getTime(), PLAYER1 );
					m_Renderer->adjustSpeedImage( g_pGame->getVehicleSpeed( PLAYER1 ), PLAYER1 );

					if( m_bIsTwoPlayer )
					{
						m_Renderer->adjustRankImage( g_pGame->m_Victory.getRank( PLAYER2 ), PLAYER2 );
						m_Renderer->adjustClockImages( g_pGame->getTime(), PLAYER2 );
						m_Renderer->adjustSpeedImage( g_pGame->getVehicleSpeed( PLAYER2 ), PLAYER2 );
					}

					if( m_bIsTwoPlayer )
						renderTwoPlayer( device );
					else
					{
						//get the game to render all of its components
						g_pGame->render( device, m_Renderer, PLAYER1, &m_ResetViewPort );

						m_Renderer->renderFPS( );
						m_Renderer->drawHUD( PLAYER1 );
					}
					if (m_iCount >= 0) {
						countDown();
						m_Renderer->renderCountDown(m_iCount);
					}
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

	m_SceneLoader->startGame( m_sGameFilename ); //load one player game

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
	//m_MenuController = new XBox360Controller(0);
	m_SceneLoader = new SceneLoader( );

	m_Controllers[0] = new XBox360Controller(0);
	m_Controllers[1] = new XBox360Controller(1);

	//Initialize the critical section before entering multi-threaded context.
	//InitializeCriticalSection(&m_CriticalSection);

	g_pGame			  = NULL;
	m_AppState		  = APP_STARTUP;
	m_uiCurrentButton = GUI_BTN_SINGLE_PLAYER;
	m_iCurrentBall	  = 0;
	m_fCheckTime	  = 0.0f;
	m_Renderer		  = new Renderer( );
	m_bIsLoading	  = false;
	m_bIsTwoPlayer	  = false;
	m_bIsTimeTrial	  = false;
	m_sGameFilename	  = ONE_PLAYER_SCENE_FILE;
	m_iCount		  = 3;
	m_Clock			  = new Clock();

	// Set up the audio
//	HRESULT hr = PrepareXACT( BG_WAVEBANK_FILE, SE_WAVEBANK_FILE, BG_SETTINGS_FILE, BG_SOUNDBANK_FILE );
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
		delete g_pGame;
		g_pGame = NULL;
	}
	if( m_Controllers ) {
		delete m_Controllers[0];
		delete m_Controllers[1];
	}

	if( m_Renderer )
		delete m_Renderer;

	if( m_SceneLoader )
		delete m_SceneLoader;

	if( m_hThread )
		CloseHandle( m_hThread );

	if ( m_Clock )
		delete m_Clock;
	// Release system object when all finished 
	//DeleteCriticalSection(&m_CriticalSection);
}