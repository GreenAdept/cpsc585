//--------------------------------------------------------------------------------------
// File: CraterRacerApp.cpp
//
//--------------------------------------------------------------------------------------
#include "CraterRacerApp.h"

GameObj* g_pGame = NULL;	//global game used by the RacerApp class 

//instatiating all the static member variables used by RacerApp
ResourceManager			RacerApp::m_ResourceManager;
ApplicationState		RacerApp::m_AppState;
Dialog					RacerApp::m_GameScreen;
XBox360Controller*		RacerApp::m_MenuController;
UINT					RacerApp::m_uiCurrentButton;
SceneLoader*			RacerApp::m_SceneLoader;
ID3DXFont*				RacerApp::m_pFont;
ID3DXSprite*            RacerApp::m_pTextSprite;
HANDLE					RacerApp::m_hThread;
DWORD					RacerApp::m_dwThreadID;
bool					RacerApp::m_bGameIsReady;
CRITICAL_SECTION		RacerApp::m_CriticalSection;
ID3DXSprite*			RacerApp::m_pImageSprite;
Sprite					RacerApp::m_Images[ NUM_IMAGES ];
Vec3					RacerApp::m_ImageLocations[ NUM_IMAGES ];
Vec3					RacerApp::m_BallLocations[ NUM_LOADING_BALLS ];
int						RacerApp::m_iButtonImage[ NUM_BUTTONS ];
int						RacerApp::m_iBallImages[ NUM_LOADING_BALLS ];
int						RacerApp::m_iCurrentBall;
RECT					RacerApp::m_PauseRECT;
UINT_PTR				RacerApp::m_AnimationID;
float					RacerApp::m_fCheckTime;
ApplicationState		RacerApp::m_iPreviousAppState;


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

	else if( g_pGame && m_AppState == APP_GAME_LOADING )
	{
		if( m_bGameIsReady )
		{
			//do start game animation/countdown here, but until we
			//have one, we'll just start the game

			//make sure the thread to load the game will not run anymore
			HWND fWindow = FindWindow(NULL, L"Crater Racer"); // Find the window
			SuspendThread( m_hThread );

			//stop loading animation
			KillTimer( fWindow, m_AnimationID );

			//start background music
			if( g_audioState.pSoundBank )
			{
				g_audioState.pSoundBank->Play(g_audioState.iGameStart, 0, 0, NULL);
				g_audioState.pSoundBank->Play(g_audioState.iEngine, 0, 0, NULL);
			}

			//start game
			g_pGame->startClock();
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
		
		//A selection was made
		if( m_MenuController->A.WasPressed() )
			processMenuSelection( );

		m_fCheckTime = 0.0f;
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
				g_pGame->pauseGame();
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
		g_pGame && m_AppState == APP_PAUSED && m_uiCurrentButton != GUI_BTN_UNPAUSE )
	{
		//send message to current button to go back to normal state
		m_iButtonImage[ m_uiCurrentButton ]--;
		m_uiCurrentButton--;
		m_iButtonImage[ m_uiCurrentButton ]++;
	}
	/*else if(  )
	{
		m_iButtonImage[ m_uiCurrentButton ]--;
		m_uiCurrentButton--;
		m_iButtonImage[ m_uiCurrentButton ]++;
	}*/
}


//--------------------------------------------------------------------------------------
// Function: moveMenuDown
// Moves the menu button selection down (if not at the bottom of the menu already)
//--------------------------------------------------------------------------------------
void RacerApp::moveMenuDown( )
{
	if( m_AppState == APP_STARTUP && m_uiCurrentButton != GUI_BTN_EXIT ||
		g_pGame && m_AppState == APP_PAUSED && m_uiCurrentButton != GUI_BTN_EXIT2 )
	{
		//tell current button to go back to normal/unselected state
		m_iButtonImage[ m_uiCurrentButton ]--;
		m_uiCurrentButton++;
		m_iButtonImage[ m_uiCurrentButton ]++;
	}
	//else if(  )
	//{
	//	//tell current button to go back to normal/unselected state
	//	m_iButtonImage[ m_uiCurrentButton ]--;
	//	m_uiCurrentButton++;
	//	m_iButtonImage[ m_uiCurrentButton ]++;
	//}
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
					m_hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)startGame,NULL,0,&m_dwThreadID);

					//start ball loading animation
					fWindow = FindWindow(NULL, L"Crater Racer");
					SetTimer( fWindow, m_AnimationID, 500, (TIMERPROC)animateBall);
					break;
					
				case GUI_BTN_UNPAUSE:
					g_pGame->unpauseGame( );
					m_AppState = APP_RENDER_GAME; 
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
	UINT width = pBackBufferSurfaceDesc->Width, 
		 height = pBackBufferSurfaceDesc->Height;

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	device->SetRenderState(D3DRS_ALPHAREF, (DWORD)100);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE); 

	V_RETURN( m_ResourceManager.OnD3D9ResetDevice() );

	if( g_pGame )
		g_pGame->processCallback( ON_RESET, device, pBackBufferSurfaceDesc );

	if( m_pFont )
       V_RETURN( m_pFont->OnResetDevice() );

    // Create a sprite to help batch calls when drawing many lines of text
    V_RETURN( D3DXCreateSprite( device, &m_pTextSprite ) );
	V_RETURN( D3DXCreateSprite( device, &m_pImageSprite ) );
	
	// Load all images used in the game as textures
	loadImages( device, width, height );

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

	if( m_pFont )
        m_pFont->OnLostDevice();

    SAFE_RELEASE( m_pTextSprite );
	SAFE_RELEASE( m_pImageSprite );
}



//--------------------------------------------------------------------------------------
// Function: OnRender
// This callback function is called every frame to render the scene.  
//--------------------------------------------------------------------------------------
void CALLBACK RacerApp::OnRender( Device* device, double dTime, float fElapsedTime, void* pUserContext  )
{
	HRESULT hr;
	Vec3 center( 0, 0, 0 );
	int image = 0;
	
	CDXUTTextHelper txtHelper( m_pFont, m_pTextSprite, 15 );

	// Clear the render target and the zbuffer 
	V( device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 255, 0,0,0 ), 1.0f, 0 ) );

	// Render the scene
	if( SUCCEEDED( device->BeginScene() ) )
	{
		switch( m_AppState )
		{
			case APP_STARTUP:
				
				drawStartupMenu( );
				break;

			case APP_SHOW_GAMERULES:

				//Draw the game rules image (as entire screen)
				m_pImageSprite->Begin( NULL );
				m_pImageSprite->Flush();
				V( m_pImageSprite->Draw( m_Images[GAMERULES_INFO_IMAGE], NULL, &center, 
						&m_ImageLocations[GAMERULES_INFO_IMAGE], D3DCOLOR_ARGB( 255,255,255,255 ) ) );
				m_pImageSprite->End();

				break;

			case APP_GAME_LOADING:

				drawLoadingScreen( );
				break;

			case APP_RENDER_GAME:
			case APP_SHOW_GAMERULES2:
			case APP_PAUSED:

				if( g_pGame )
				{
					//get the game to render all of its components
					g_pGame->render( device );

						//render the player's rank
						txtHelper.Begin();
						txtHelper.SetInsertionPos( 10, 50 );
						txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
						wstring str = g_pGame->m_Victory.getFormattedString(0);
						txtHelper.DrawFormattedTextLine( str.c_str(), g_pGame->m_Victory.getRank(0) );
						txtHelper.End();

					renderFPS( );
					renderClock( );
					drawHUD();
				}

				if( m_AppState == APP_PAUSED )
					drawPauseScreen( );

				if( m_AppState == APP_SHOW_GAMERULES2 )
				{
					//render the game rules screen overtop the game in the center of the screen
					RECT temp;
					temp.top = 0; temp.right = 650;
					temp.bottom = 550; temp.left = 0;
					m_pImageSprite->Begin( NULL );
					m_pImageSprite->Flush();
					V( m_pImageSprite->Draw( m_Images[GAMERULES_INFO_SMALL_IMAGE], &temp, &center, 
							&m_ImageLocations[GAMERULES_INFO_SMALL_IMAGE], D3DCOLOR_ARGB( 255,255,255,255 ) ) );
					m_pImageSprite->End();
				}
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
	
	//set the size and font of text to be used in "RenderText" function
    V_RETURN( D3DXCreateFont( device, 24, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
                          OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                          L"Arial", &m_pFont ) );

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
	m_ResourceManager.OnD3D9DestroyDevice( );

	if( g_pGame )
		g_pGame->processCallback( ON_DESTROY );

	SAFE_RELEASE( m_pFont );
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
// Function:  renderFPS
//--------------------------------------------------------------------------------------
void RacerApp::renderFPS( )
{
    CDXUTTextHelper txtHelper( m_pFont, m_pTextSprite, 15 );

    txtHelper.Begin();
    txtHelper.SetInsertionPos( 10, 10 );
	txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	LPCWSTR stats = DXUTGetFrameStats( true );

	wstring str( stats ), fps;
	fps = str.substr( 5, 10 );
    txtHelper.DrawTextLine( fps.c_str() ); // Show FPS
		
	txtHelper.End();
}


//--------------------------------------------------------------------------------------
// Function: renderClock
// Renders the game time in the right hand corner of the screen
//--------------------------------------------------------------------------------------
void RacerApp::renderClock( )
{
	CDXUTTextHelper txtHelper( m_pFont, m_pTextSprite, 15 );

    txtHelper.Begin();
    txtHelper.SetInsertionPos( 10, 35 );
	txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	string time = g_pGame->getTime();

	wstring wTime(time.length(), L' ');
	copy(time.begin(), time.end(), wTime.begin());
    txtHelper.DrawTextLine( wTime.c_str() );
		
	txtHelper.End();
}


//--------------------------------------------------------------------------------------
// Function: drawPauseScreen( )
// Renders the pause menu.
//--------------------------------------------------------------------------------------
void RacerApp::drawPauseScreen( )
{
	HRESULT hr;
	Vec3 center( 0, 0, 0 );
	int image = 0;

	m_pImageSprite->Begin( NULL );
					
	m_pImageSprite->Flush();
	m_GameScreen.DrawRect( &m_PauseRECT, D3DCOLOR_ARGB( 255, 0,0,0 ) );

	//draw the menu buttons
	for( int i=GUI_BTN_UNPAUSE; i <= GUI_BTN_EXIT2; i++ )
	{
		m_pImageSprite->Flush();
		image = m_iButtonImage[ i ];
		V( m_pImageSprite->Draw( m_Images[ image ], NULL, &center, &m_ImageLocations[image], D3DCOLOR_ARGB( 255,255,255,255 ) ) );
	}
	m_pImageSprite->Flush();
	m_pImageSprite->End( );
}


//--------------------------------------------------------------------------------------
// Function: drawHUD( )
// Renders the speedometer, time display, ranking and minimap
//--------------------------------------------------------------------------------------
void RacerApp::drawHUD( )
{
	HRESULT hr;
	Vec3 center( 0, 0, 0 );
	int image = 0;

	//render the HUD
	m_pImageSprite->Begin( NULL );
	
	m_pImageSprite->Flush();
	V( m_pImageSprite->Draw( m_Images[ SPEEDOMETER_IMAGE ], NULL, &center, &m_ImageLocations[ SPEEDOMETER_IMAGE ], D3DCOLOR_ARGB( 255,255,255,255 ) ) );

	m_pImageSprite->Flush();
	
	m_pImageSprite->End( );
}

//--------------------------------------------------------------------------------------
// Function: drawStartupMenu( )
// Renders the startup menu.
//--------------------------------------------------------------------------------------
void RacerApp::drawStartupMenu( )
{
	HRESULT hr;
	Vec3 center( 0, 0, 0 );
	int image = 0;

	//render the game menu
	m_pImageSprite->Begin( NULL );
	
	m_pImageSprite->Flush();
	V( m_pImageSprite->Draw( m_Images[ MENU_IMAGE ], NULL, &center, &m_ImageLocations[ MENU_IMAGE ], D3DCOLOR_ARGB( 255,255,255,255 ) ) );

	//draw the menu buttons
	for( int i=GUI_BTN_SINGLE_PLAYER; i <= GUI_BTN_EXIT; i++ )
	{
		m_pImageSprite->Flush();
		image = m_iButtonImage[ i ];
		V( m_pImageSprite->Draw( m_Images[ image ], NULL, &center, &m_ImageLocations[image], D3DCOLOR_ARGB( 255,255,255,255 ) ) );
	}
	m_pImageSprite->Flush();
	m_pImageSprite->End( );
}


//--------------------------------------------------------------------------------------
// Function: drawLoadingScreen( )
// Renders the loading screen and animation.
//--------------------------------------------------------------------------------------
void RacerApp::drawLoadingScreen( )
{
	HRESULT hr;
	Vec3 center( 0, 0, 0 );
	int image = 0;

	m_pImageSprite->Begin( NULL );
	m_pImageSprite->Flush();

	V( m_pImageSprite->Draw( m_Images[LOADING_IMAGE], NULL, &center, &m_ImageLocations[LOADING_IMAGE], D3DCOLOR_ARGB( 255,255,255,255 ) ) );
	
	//draw animating balls
	for( int i=0; i < NUM_LOADING_BALLS; i++ )
	{
		image = m_iBallImages[ i ];
		m_pImageSprite->Flush();
		V( m_pImageSprite->Draw( m_Images[ image ], NULL, &center, &m_BallLocations[i], D3DCOLOR_ARGB( 255,255,255,255 ) ) );
	}

	m_pImageSprite->Flush();
	V( m_pImageSprite->Draw( m_Images[CONTROLS_IMAGE], NULL, &center, &m_ImageLocations[CONTROLS_IMAGE], D3DCOLOR_ARGB( 255,255,255,255 ) ) );
	
	m_pImageSprite->End( );

}

//--------------------------------------------------------------------------------------
// Function: startGame
// Function call started by initialization thread.
//--------------------------------------------------------------------------------------
long WINAPI RacerApp::startGame( long lParam )
{
	//Enter the critical section -- other threads are locked out
    EnterCriticalSection(&m_CriticalSection);
 
	m_SceneLoader->initScene( &g_pGame );

	m_SceneLoader->startGame( ONE_PLAYER_SCENE_FILE ); //load one player game

	 //Leave the critical section -- other threads can now EnterCriticalSection() 
    LeaveCriticalSection(&m_CriticalSection);
	
	m_bGameIsReady = true;

	return 0;
}


//--------------------------------------------------------------------------------------
// Function: animateBall
// Increments the big ball location in the loading screen animation.  This is the timer
// callback function started in the KeyPress event function.
//--------------------------------------------------------------------------------------
void RacerApp::animateBall( )
{
	m_iBallImages[ m_iCurrentBall ]--;
	m_iCurrentBall++;

	if( m_iCurrentBall >= NUM_LOADING_BALLS )
		m_iCurrentBall = 0;

	m_iBallImages[ m_iCurrentBall ]++;
}


//--------------------------------------------------------------------------------------
// Function: doLoadScreen
// Here is where we will start the loading animation
//--------------------------------------------------------------------------------------
void RacerApp::doLoadScreen( )
{


}


//--------------------------------------------------------------------------------------
// Function: loadImages
// Creates textures for all images used in game and stores their rendering positions
// to be used later.
//--------------------------------------------------------------------------------------
void RacerApp::loadImages( Device* device, UINT width, UINT height )
{
	//Non-button images
	createTexture( m_Images[ MENU_IMAGE ], MENU_IMAGE_FILE, device );
	createTexture( m_Images[ LOADING_IMAGE ],  LOADING_IMAGE_FILE, device );
	createTexture( m_Images[ LOADING_SMALLBALL_IMAGE ], LOADING_SMALLBALL_IMAGE_FILE, device );
	createTexture( m_Images[ LOADING_BIGBALL_IMAGE ], LOADING_BIGBALL_IMAGE_FILE, device );
	createTexture( m_Images[ CONTROLS_IMAGE ], CONTROLS_IMAGE_FILE, device );
	createTexture( m_Images[ GAMERULES_INFO_IMAGE ], GAMERULES_INFO_IMAGE_FILE, device );
	createTexture( m_Images[ GAMERULES_INFO_SMALL_IMAGE ], GAMERULES_INFO_SMALL_IMAGE_FILE, device );
	createTexture( m_Images[ SPEEDOMETER_IMAGE ], SPEEDOMETER_IMAGE_FILE, device );

	//Startup Menu Buttons
	createTexture( m_Images[ ONEPLAYER_NOTACTIVE_IMAGE ], ONEPLAYER_NOTACTIVE_IMAGE_FILE, device );
	createTexture( m_Images[ ONEPLAYER_ACTIVE_IMAGE ], ONEPLAYER_ACTIVE_IMAGE_FILE, device );
	createTexture( m_Images[ TWOPLAYER_IMAGE ], TWOPLAYER_IMAGE_FILE, device );
	createTexture( m_Images[ TWOPLAYER_ACTIVE_IMAGE ], TWOPLAYER_ACTIVE_IMAGE_FILE, device );
	createTexture( m_Images[ TIMETRIAL_IMAGE ], TIMETRIAL_IMAGE_FILE, device );
	createTexture( m_Images[ TIMETRIAL_ACTIVE_IMAGE ], TIMETRIAL_ACTIVE_IMAGE_FILE, device );
	createTexture( m_Images[ GAMERULES_IMAGE ], GAMERULES_IMAGE_FILE, device );
	createTexture( m_Images[ GAMERULES_ACTIVE_IMAGE ], GAMERULES_ACTIVE_IMAGE_FILE, device );
	createTexture( m_Images[ EXIT_NOTACTIVE_IMAGE ], EXIT_NOTACTIVE_IMAGE_FILE, device );
	createTexture( m_Images[ EXIT_ACTIVE_IMAGE ], EXIT_ACTIVE_IMAGE_FILE, device );
	
	//Pause Menu Buttons
	createTexture( m_Images[ UNPAUSE_IMAGE ], UNPAUSE_IMAGE_FILE, device );
	createTexture( m_Images[ UNPAUSE_ACTIVE_IMAGE ], UNPAUSE_ACTIVE_IMAGE_FILE, device );
	createTexture( m_Images[ GAMERULES2_IMAGE ], GAMERULES_IMAGE_FILE, device );
	createTexture( m_Images[ GAMERULES2_ACTIVE_IMAGE ], GAMERULES_ACTIVE_IMAGE_FILE, device );
	createTexture( m_Images[ EXIT2_NOTACTIVE_IMAGE ], EXIT_NOTACTIVE_IMAGE_FILE, device );
	createTexture( m_Images[ EXIT2_ACTIVE_IMAGE ], EXIT_ACTIVE_IMAGE_FILE, device );

	//Position the startup menu buttons and images --------------
	int w = (width - 400)/2; if( w<0 ) w=0;
	int h = (height - 430)/2; if( h<0 ) h=0;

	m_ImageLocations[MENU_IMAGE] = Vec3( w, h, 0 );
	h += 230;
	m_ImageLocations[ONEPLAYER_NOTACTIVE_IMAGE] = Vec3( w, h, 0 );
	m_ImageLocations[ONEPLAYER_ACTIVE_IMAGE] = Vec3( w, h, 0 );
	h += 40;
	m_ImageLocations[TWOPLAYER_IMAGE] = Vec3( w, h, 0 );
	m_ImageLocations[TWOPLAYER_ACTIVE_IMAGE] = Vec3( w, h, 0 );
	h += 40;
	m_ImageLocations[TIMETRIAL_IMAGE] = Vec3( w, h, 0 );
	m_ImageLocations[TIMETRIAL_ACTIVE_IMAGE] = Vec3( w, h, 0 );
	h += 40;
	m_ImageLocations[GAMERULES_IMAGE] = Vec3( w, h, 0 );
	m_ImageLocations[GAMERULES_ACTIVE_IMAGE] = Vec3( w, h, 0 );
	h += 40;
	m_ImageLocations[EXIT_NOTACTIVE_IMAGE] = Vec3( w, h, 0 );
	m_ImageLocations[EXIT_ACTIVE_IMAGE] = Vec3( w, h, 0 );
	
	//Position the loading screen images ----------------------
	h = ( height - 480 ) / 2; if( h<0 ) h=0;
	m_ImageLocations[LOADING_IMAGE] = Vec3( w, h, 0 );

	//Position the loading screen animating balls
	h += 40;  w += 122; 
	for( int i=0; i < NUM_LOADING_BALLS; i++ )
	{
		m_BallLocations[i] = Vec3( w, h, 0 );
		w += 30;
	}
	h += 40;
	w = ( width - 600 ) / 2; if( w<0 ) w=0;
	m_ImageLocations[CONTROLS_IMAGE] = Vec3( w, h, 0 );

	//Position the pause menu ----------------------------------
	w = ( width - 650 ) / 2; if( w<0 ) w=0;
	h = ( height - 550 ) / 2; if( h<0 ) h=0;
	m_PauseRECT.left = w;   m_PauseRECT.right = w+650;
	m_PauseRECT.top = h;   m_PauseRECT.bottom = h+550;

	w = (width - 400) / 2; if( w<0 ) w=0;
	h = (height - 120) / 2;  if( h<0 ) h=0;
	m_ImageLocations[UNPAUSE_IMAGE] = Vec3( w, h, 0 );
	m_ImageLocations[UNPAUSE_ACTIVE_IMAGE] = Vec3( w, h, 0 );
	h += 40;
	m_ImageLocations[GAMERULES2_IMAGE] = Vec3( w, h, 0 );
	m_ImageLocations[GAMERULES2_ACTIVE_IMAGE] = Vec3( w, h, 0 );
	h += 40;
	m_ImageLocations[EXIT2_NOTACTIVE_IMAGE] = Vec3( w, h, 0 );
	m_ImageLocations[EXIT2_ACTIVE_IMAGE] = Vec3( w, h, 0 );

	//Position HUD images
	m_ImageLocations[SPEEDOMETER_IMAGE] = Vec3( 10, height-140, 0 );

	//Position all other images
	w = ( width - 800 ) / 2; if( w<0 ) w=0;
	h = ( height - 700 ) / 2; if( h<0 ) h=0;
	m_ImageLocations[GAMERULES_INFO_IMAGE] = Vec3( w, h, 0 );

	w = ( width - 650 ) / 2; if( w<0 ) w=0;
	h = ( height - 550 ) / 2; if( h<0 ) h=0;
	m_ImageLocations[GAMERULES_INFO_SMALL_IMAGE] = Vec3( w, h, 0 );
}


//--------------------------------------------------------------------------------------
// Function: createTexture
// Creates a sprite based on input "file" and stores it in the specified "texture" variable.
//--------------------------------------------------------------------------------------
void RacerApp::createTexture( Sprite& texture, LPCSTR file, Device* device )
{
	D3DXIMAGE_INFO pInfo;

	D3DXCreateTextureFromFileExA(
	device,
	file,
	D3DX_DEFAULT,
	D3DX_DEFAULT,
	D3DX_DEFAULT, // mipmap
	0,
	D3DFMT_UNKNOWN,
	D3DPOOL_MANAGED,
	D3DX_FILTER_NONE,
	D3DX_DEFAULT,
	D3DCOLOR_ARGB( 255, 255,255,255 ),	
	&pInfo,
	NULL,
	&texture );
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

	g_pGame = NULL;
	m_AppState = APP_STARTUP;

	m_iButtonImage[ GUI_BTN_SINGLE_PLAYER ] = ONEPLAYER_ACTIVE_IMAGE;
	m_iButtonImage[ GUI_BTN_TWO_PLAYER ] = TWOPLAYER_IMAGE;
	m_iButtonImage[ GUI_BTN_TIMETRIAL ] = TIMETRIAL_IMAGE;
	m_iButtonImage[ GUI_BTN_GAMERULES ] = GAMERULES_IMAGE;
	m_iButtonImage[ GUI_BTN_EXIT ] = EXIT_NOTACTIVE_IMAGE;
	
	m_iButtonImage[ GUI_BTN_UNPAUSE ] = UNPAUSE_ACTIVE_IMAGE;
	m_iButtonImage[ GUI_BTN_GAMERULES2 ] = GAMERULES2_IMAGE;
	m_iButtonImage[ GUI_BTN_EXIT2 ] = EXIT2_NOTACTIVE_IMAGE;

	//Set up ball animation images for loading screen
	for( int i=0; i < NUM_LOADING_BALLS; i++ )
		m_iBallImages[i] = LOADING_SMALLBALL_IMAGE;

	m_iBallImages[0] = LOADING_BIGBALL_IMAGE;
	m_iCurrentBall = 0;

	// Set up the audio
	HRESULT hr = PrepareXACT( BG_WAVEBANK_FILE, SE_WAVEBANK_FILE, BG_SETTINGS_FILE, BG_SOUNDBANK_FILE );

	//initialize the resource manager to keep track of all our screens and HUD
    m_GameScreen.Init( &m_ResourceManager );

	//The currently highlighted button
	m_uiCurrentButton = GUI_BTN_SINGLE_PLAYER;

	m_fCheckTime = 0.0f;
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

	if( m_SceneLoader )
		delete m_SceneLoader;

	SAFE_RELEASE( m_pFont );
	SAFE_RELEASE( m_pTextSprite );
	SAFE_RELEASE( m_pImageSprite );
	
	if( m_hThread )
			CloseHandle( m_hThread );

	for( int i=0; i < NUM_IMAGES; i++ )
		SAFE_RELEASE( m_Images[i] );

	// Release system object when all finished 
	DeleteCriticalSection(&m_CriticalSection);
}