/*
* The Rendering subsystem handles all drawing to the screen for our game.  This includes all the 
* game menus, the gameplay and HUD.
*/

#include "Renderer.h"
#include "MessageManager.h"

//////////////////////////////////////////////////////////////////////////////////////////////		
// 
//	Public Functions
//		
//////////////////////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------
// Function: CONSTRUCTOR
//--------------------------------------------------------------------------------------
Renderer::Renderer( )
{
	//startup menu buttons
	m_iButtonImages[ GUI_BTN_SINGLE_PLAYER ] = ONEPLAYER_ACTIVE_IMAGE;
	m_iButtonImages[ GUI_BTN_TWO_PLAYER ] = TWOPLAYER_IMAGE;
	m_iButtonImages[ GUI_BTN_TIMETRIAL ] = TIMETRIAL_IMAGE;
	m_iButtonImages[ GUI_BTN_GAMERULES ] = GAMERULES_IMAGE;
	m_iButtonImages[ GUI_BTN_EXIT ] = EXIT_NOTACTIVE_IMAGE;

	//pause screen buttons
	m_iButtonImages[ GUI_BTN_UNPAUSE ] = UNPAUSE_ACTIVE_IMAGE;
	m_iButtonImages[ GUI_BTN_GAMERULES2 ] = GAMERULES2_IMAGE;
	m_iButtonImages[ GUI_BTN_EXIT2 ] = EXIT2_NOTACTIVE_IMAGE;

	//victory screen buttons
	m_iButtonImages[ GUI_BTN_MAINMENU ] = MAINMENU_ACTIVE_IMAGE;
	m_iButtonImages[ GUI_BTN_EXITSMALL ] = EXIT_SMALL_IMAGE;

	//Set up ball animation images for loading screen
	for( int i=0; i < NUM_LOADING_BALLS; i++ )
		m_iBallImages[i] = LOADING_SMALLBALL_IMAGE;
	//m_iBallImages[0] = LOADING_BIGBALL_IMAGE;

	//Set up time images to be zeros
	for( int i=0; i < 8; i++ )
		m_iTimeImages[i] = ZERO_IMAGE;
	m_iTimeImages[2] = COLON_IMAGE;
	m_iTimeImages[5] = COLON_IMAGE;

	m_iPlayerOneRank	= FIRST_IMAGE;
	m_iPlayerOneImage	= FIRST_IMAGE;
	m_iClockImage		= ZERO_IMAGE;
	m_fSpeedRotation	= 0;
	
	for( int i=0; i < NUM_PLAYERS; i++ )
	{
		m_bDrawWrongWay[i] = false;
		m_iLapImages[ i ] = LAPONE_IMAGE;
		m_sVictoryTimes[ i ] = L"TIME:\n00:00:00";
		m_sVictoryRanks[ i ] = L"PLAYER #1";
	}
	m_sVictoryTimes[ 0 ] = L"TIME: 00:00:00";

	//initialize the resource manager to keep track of all our screens and HUD
    m_GameScreen.Init( &m_ResourceManager );
}


//--------------------------------------------------------------------------------------
// Function: DECONSTRUCTOR
// Clears up all memory and releases all sprites/fonts used by the Renderer.
//--------------------------------------------------------------------------------------
Renderer::~Renderer( )
{
	SAFE_RELEASE( m_pFont );
	SAFE_RELEASE( m_pTextSprite );
	SAFE_RELEASE( m_pImageSprite );
	
	for( int i=0; i < NUM_IMAGES; i++ )
		SAFE_RELEASE( m_Images[i] );
}


//////////////////////////////////////////////////////////////////////////////////////////////		
// 
//	Public Device-Related Functions
//		
//////////////////////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------
// Function: OnReset
// Creates sprites, loads images and sets the render state for the device.
//--------------------------------------------------------------------------------------
HRESULT Renderer::OnReset( Device* device, const D3DSURFACE_DESC* pBack )
{
	HRESULT hr;
	UINT width = pBack->Width, 
		 height = pBack->Height;

	V_RETURN( m_ResourceManager.OnD3D9ResetDevice() );

	// Create a sprite to help batch calls when drawing many lines of text
	D3DXCreateSprite( device, &m_pTextSprite );
	D3DXCreateSprite( device, &m_pImageSprite );
	
	if( m_pFont )
       V_RETURN( m_pFont->OnResetDevice() );

	if( m_pFontVictoryBig )
       V_RETURN( m_pFontVictoryBig->OnResetDevice() );

	if( m_pFontVictorySmall )
       V_RETURN( m_pFontVictorySmall->OnResetDevice() );

	// Load all images used in the game as textures
	loadImages( device, width, height );
}


//--------------------------------------------------------------------------------------
// Function: OnCreate
// Creates the fonts attached to the device.
// INPUT: Device* device -> the device that has been created.	
//--------------------------------------------------------------------------------------
HRESULT Renderer::OnCreate( Device* device )
{
	HRESULT hr;

	//initialize the resource manager for our HUD and Pause Screen.
	V_RETURN( m_ResourceManager.OnD3D9CreateDevice( device ) );

	//set the size and font of text to be used in "RenderText" function
    V_RETURN( D3DXCreateFont( device, 24, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
                          OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                          L"Arial", &m_pFont ));

    V_RETURN( D3DXCreateFont( device, 30, 10, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
                      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                      L"Comic Sans MS", &m_pFontVictoryBig ) );

	V_RETURN( D3DXCreateFont( device, 20, 10, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
                   OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                   TEXT("Comic Sans MS"), &m_pFontVictorySmall ) );
	
}


//--------------------------------------------------------------------------------------
// Function: OnLost
// Calls OnLostDevice on any fonts used by the renderer and releases sprites.
//--------------------------------------------------------------------------------------
void Renderer::OnLost( )
{
	m_ResourceManager.OnD3D9LostDevice();

	if( m_pFont )
        m_pFont->OnLostDevice();

	if( m_pFontVictoryBig )
		m_pFontVictoryBig->OnLostDevice( );

	if( m_pFontVictorySmall )
		m_pFontVictorySmall->OnResetDevice( );

    SAFE_RELEASE( m_pTextSprite );
	SAFE_RELEASE( m_pImageSprite );
}


//--------------------------------------------------------------------------------------
// Function: OnDestroy
// Releases any fonts used by the renderer.  Call this when the device the fonts are 
// attached to is destroyed.
//--------------------------------------------------------------------------------------
void Renderer::OnDestroy( )
{
	m_ResourceManager.OnD3D9DestroyDevice( );

	SAFE_RELEASE( m_pFont );
	SAFE_RELEASE( m_pFontVictoryBig );
	SAFE_RELEASE( m_pFontVictorySmall );
}


//////////////////////////////////////////////////////////////////////////////////////////////		
// 
//	Public Drawing Functions
//		
//////////////////////////////////////////////////////////////////////////////////////////////


//--------------------------------------------------------------------------------------
// Function: drawPauseGameRules
// This function draws the game rules screen as an overlay to the game (as a smaller
// rectangle inside the screen)
//--------------------------------------------------------------------------------------
void Renderer::drawPauseGameRules( )
{
	HRESULT hr;
	Vec3 center( 0, 0, 0 );
	int image = 0;

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


//--------------------------------------------------------------------------------------
// Function: drawPauseScreen( )
// Renders the pause menu as an overlay to the game (as a smaller rectangle in the center
// of the screen)
//--------------------------------------------------------------------------------------
void Renderer::drawPauseScreen( )
{
	HRESULT hr;
	Vec3 center( 0, 0, 0 );
	int image = 0;

	m_pImageSprite->Begin( NULL );
					
	//draw a black rectangle to the background of our pause menu
	m_pImageSprite->Flush();
	m_GameScreen.DrawRect( &m_PauseRECT, D3DCOLOR_ARGB( 255, 0,0,0 ) );

	//draw the menu buttons centered in the black rectangle
	for( int i=GUI_BTN_UNPAUSE; i <= GUI_BTN_EXIT2; i++ )
	{
		m_pImageSprite->Flush();
		image = m_iButtonImages[ i ];
		V( m_pImageSprite->Draw( m_Images[ image ], NULL, &center, &m_ImageLocations[image], D3DCOLOR_ARGB( 255,255,255,255 ) ) );
	}
	m_pImageSprite->Flush();
	m_pImageSprite->End( );
}


//--------------------------------------------------------------------------------------
// Function: drawHUD( )
// Renders the speedometer, time display, ranking and minimap overtop of the game.
//--------------------------------------------------------------------------------------
void Renderer::drawHUD( )
{
	HRESULT hr;
	Matrix mat;
	D3DXVECTOR2 center( 85, 15 );
	D3DXVECTOR2 trans( m_ImageLocations[ SPEEDWAND_IMAGE ].x, m_ImageLocations[ SPEEDWAND_IMAGE ].y);
	Matrix rotateZ;
	int image = 0;

	//render the HUD
	m_pImageSprite->Begin( D3DXSPRITE_ALPHABLEND );
	
	//draw the speedometer 
	m_pImageSprite->Flush();
	V( m_pImageSprite->Draw( m_Images[ SPEEDOMETER_IMAGE ], NULL, NULL, &m_ImageLocations[ SPEEDOMETER_IMAGE ], D3DCOLOR_ARGB( 255,255,255,255 ) ) );
	m_pImageSprite->Flush();

	//rotate the speedometer wand image to correct position
	D3DXMatrixTransformation2D(&mat, NULL, 0.0, NULL, &center, m_fSpeedRotation, &trans );
	m_pImageSprite->SetTransform( &mat );
	V( m_pImageSprite->Draw( m_Images[ SPEEDWAND_IMAGE ], NULL, NULL, NULL, D3DCOLOR_ARGB( 255,255,255,255 ) ) );

	//reset sprite transform matrix so all sprites aren't rotated
	D3DXMatrixIdentity( &mat );
	m_pImageSprite->SetTransform( &mat );

	//draw the minimap
	m_pImageSprite->Flush();
	V( m_pImageSprite->Draw( m_Images[ MINIMAP_IMAGE ], NULL, NULL, &m_ImageLocations[ MINIMAP_IMAGE ], D3DCOLOR_ARGB( 255,255,255,255 ) ) );

	//draw the ranking
	m_pImageSprite->Flush();
	V( m_pImageSprite->Draw( m_Images[ m_iPlayerOneRank ], NULL, NULL, &m_ImageLocations[ m_iPlayerOneRank ], D3DCOLOR_ARGB( 255,255,255,255 ) ) );

	//draw the clock
	for( int i=0; i < 8; i++ )
	{
		m_pImageSprite->Flush();
		image = m_iTimeImages[ i ];
		V( m_pImageSprite->Draw( m_Images[ image ], NULL, NULL, &m_TimeLocations[i], D3DCOLOR_ARGB( 255,255,255,255 ) ) );
	}

	//draw wrong way message if applicable
	m_pImageSprite->Flush();
	if( m_bDrawWrongWay[ PLAYER1 ] )
		V( m_pImageSprite->Draw( m_Images[ WRONGWAY_IMAGE ], NULL, NULL, &m_ImageLocations[ WRONGWAY_IMAGE ], D3DCOLOR_ARGB( 255,255,255,255 ) ) );

	// draw lap counter
	image = m_iLapImages[ PLAYER1 ];
	V( m_pImageSprite->Draw( m_Images[ image ], NULL, NULL, &m_ImageLocations[ image ], D3DCOLOR_ARGB( 255,255,255,255 ) ) );

	m_pImageSprite->Flush();
	m_pImageSprite->End( );
}


//--------------------------------------------------------------------------------------
// Function: drawStartupMenu( )
// Renders the startup menu.
//--------------------------------------------------------------------------------------
void Renderer::drawStartupMenu( )
{
	HRESULT hr;
	int image = 0;

	//render the game menu
	m_pImageSprite->Begin( D3DXSPRITE_ALPHABLEND );
	
	m_pImageSprite->Flush();
	V( m_pImageSprite->Draw( m_Images[ MENU_IMAGE ], NULL, NULL, &m_ImageLocations[ MENU_IMAGE ], D3DCOLOR_ARGB( 255,255,255,255 ) ) );

	//draw the menu buttons
	for( int i=GUI_BTN_SINGLE_PLAYER; i <= GUI_BTN_EXIT; i++ )
	{
		m_pImageSprite->Flush();
		image = m_iButtonImages[ i ];
		V( m_pImageSprite->Draw( m_Images[ image ], NULL, NULL, &m_ImageLocations[image], D3DCOLOR_ARGB( 255,255,255,255 ) ) );
	}

	m_pImageSprite->Flush();
	m_pImageSprite->End( );
}


//--------------------------------------------------------------------------------------
// Function: drawGameRules
// This function draws a full-sized game rules screen (so it will fill up the entire
// screen).
//--------------------------------------------------------------------------------------
void Renderer::drawGameRules( )
{
	HRESULT hr;

	//Draw the game rules image (as entire screen)
	m_pImageSprite->Begin( D3DXSPRITE_ALPHABLEND );
	m_pImageSprite->Flush();
	V( m_pImageSprite->Draw( m_Images[GAMERULES_INFO_IMAGE], NULL, NULL, 
			&m_ImageLocations[GAMERULES_INFO_IMAGE], D3DCOLOR_ARGB( 255,255,255,255 ) ) );
	m_pImageSprite->End();
}


//--------------------------------------------------------------------------------------
// Function: drawLoadingScreen( )
// Renders the loading screen, including the ball animation.
//--------------------------------------------------------------------------------------
void Renderer::drawLoadingScreen( )
{
	HRESULT hr;
	int image = 0;

	m_pImageSprite->Begin( D3DXSPRITE_ALPHABLEND );
	m_pImageSprite->Flush();

	V( m_pImageSprite->Draw( m_Images[LOADING_IMAGE], NULL, NULL, &m_ImageLocations[LOADING_IMAGE], D3DCOLOR_ARGB( 255,255,255,255 ) ) );
	
	//draw animating balls
	for( int i=0; i < NUM_LOADING_BALLS; i++ )
	{
		image = m_iBallImages[ i ];
		m_pImageSprite->Flush();
		V( m_pImageSprite->Draw( m_Images[ image ], NULL, NULL, &m_BallLocations[i], D3DCOLOR_ARGB( 255,255,255,255 ) ) );
	}

	m_pImageSprite->Flush();
	V( m_pImageSprite->Draw( m_Images[CONTROLS_IMAGE], NULL, NULL, &m_ImageLocations[CONTROLS_IMAGE], D3DCOLOR_ARGB( 255,255,255,255 ) ) );
	
	m_pImageSprite->End( );
}


//--------------------------------------------------------------------------------------
// Function:  renderFPS
// This function renders the frame per second as text in the upper left hand corner.
//--------------------------------------------------------------------------------------
void Renderer::renderFPS( )
{
    CDXUTTextHelper txtHelper( m_pFont, m_pTextSprite, 15 );

    txtHelper.Begin();
    txtHelper.SetInsertionPos( 10, 100 );
	txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	LPCWSTR stats = DXUTGetFrameStats( true );

	wstring str( stats ), fps;
	fps = str.substr( 5, 10 );
    txtHelper.DrawTextLine( fps.c_str() ); // Show FPS
		
	txtHelper.End();
}


//--------------------------------------------------------------------------------------
// Function:  drawVictoryScreen
// This function renders the victory screen. 
//--------------------------------------------------------------------------------------
void Renderer::drawVictoryScreen( )
{
	HRESULT hr;
	int image = 0;

	//render the game menu
	m_pImageSprite->Begin( D3DXSPRITE_ALPHABLEND );
	
	m_pImageSprite->Flush();
	V( m_pImageSprite->Draw( m_Images[ VICTORY_IMAGE ], NULL, NULL, &m_ImageLocations[ VICTORY_IMAGE ], D3DCOLOR_ARGB( 255,255,255,255 ) ) );

	//draw the victory buttons
	for( int i=GUI_BTN_MAINMENU; i <= GUI_BTN_EXITSMALL; i++ )
	{
		m_pImageSprite->Flush();
		image = m_iButtonImages[ i ];
		V( m_pImageSprite->Draw( m_Images[ image ], NULL, NULL, &m_ImageLocations[image], D3DCOLOR_ARGB( 255,255,255,255 ) ) );
	}

	//Draw rankings and times as text
	m_pFontVictoryBig->DrawTextW( NULL, this->m_sVictoryRanks[0], -1, &m_VictoryRecs[0], DT_CENTER, D3DCOLOR_ARGB( 255,0,0,0 ) );
	m_pFontVictoryBig->DrawTextW( NULL, this->m_sVictoryTimes[0], -1, &m_VictoryRecs[4], DT_CENTER, D3DCOLOR_ARGB( 255,0,0,0 ) );

	m_pFontVictorySmall->DrawTextW( NULL, this->m_sVictoryRanks[1], -1, &m_VictoryRecs[1], DT_CENTER, D3DCOLOR_ARGB( 255,0,0,0 ) );
	m_pFontVictorySmall->DrawTextW( NULL, this->m_sVictoryTimes[1], -1, &m_VictoryRecs[5], DT_CENTER, D3DCOLOR_ARGB( 255,0,0,0 ) );

	m_pFontVictorySmall->DrawTextW( NULL, this->m_sVictoryRanks[2], -1, &m_VictoryRecs[2], DT_CENTER, D3DCOLOR_ARGB( 255,0,0,0 ) );
	m_pFontVictorySmall->DrawTextW( NULL, this->m_sVictoryTimes[2], -1, &m_VictoryRecs[6], DT_CENTER, D3DCOLOR_ARGB( 255,0,0,0 ) );

	m_pFontVictorySmall->DrawTextW( NULL, this->m_sVictoryRanks[3], -1, &m_VictoryRecs[3], DT_CENTER, D3DCOLOR_ARGB( 255,0,0,0 ) );
	m_pFontVictorySmall->DrawTextW( NULL, this->m_sVictoryTimes[3], -1, &m_VictoryRecs[7], DT_CENTER, D3DCOLOR_ARGB( 255,0,0,0 ) );

	m_pImageSprite->Flush();
	m_pImageSprite->End( );
}


//--------------------------------------------------------------------------------------
// Function: renderGame
// Draws game objects to the specified device.  The renderables of these objects 
// are passed to this function and contain all the rendering-specific information needed
// by the renderer.  The latest cameras are also required.
//--------------------------------------------------------------------------------------
void Renderer::renderGame( Device* device, vector<Renderable*> renderables, vector<GameCamera*> cameras ) 
{
	HRESULT		hr;	
	Matrix		mWorld,		
				mView,
				mProj,
				mWorldViewProjection,
				mMeshWorld;
	LPD3DXMESH	tempMesh;
	Mesh		*dxMesh;	//the DXUT wrapper version of the mesh with helpful functions
	Renderable	*tempR;		//pointer to current renderable
	Effect		*pEffect;	//the effect currently being used
	MCamera		camera = cameras[0]->getCamera();

	// sort renderables here...perhaps by type and then take out the hidden objects

	// now go through and render each object via its renderable
	for( unsigned int i = 0; i < renderables.size(); i++ )
	{
		// initialize some temporary variables to make things easier
		tempR = renderables[i];
		dxMesh = tempR->m_pMesh;
		tempMesh = dxMesh->GetMesh();
		pEffect = tempR->m_pEffect;
		mMeshWorld = tempR->m_matWorld;
   
		// get the projection & view matrix from the camera class
		mWorld = *camera.GetWorldMatrix();
		mProj = *camera.GetProjMatrix();
		mView = *camera.GetViewMatrix();

		// create the world view projection matrix
		mWorldViewProjection = mMeshWorld * mWorld * mView * mProj;

		// set the effect to be used
		pEffect->SetTechnique( tempR->m_hRenderObj );

		// Update the effect's variables.  The SDK examples say it would 
		// be more efficient to cache a handle to the parameter by calling 
		// ID3DXEffect::GetParameterByName, so we should eventually implement that
		V( pEffect->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection ) );
		V( pEffect->SetMatrix( "g_mWorld", &mWorld ) );

		// start rendering
		UINT cPasses;
        V( pEffect->Begin( &cPasses, 0 ) );
        for( UINT p = 0; p < cPasses; ++p )
        {
            V( pEffect->BeginPass( p ) );

			// set and draw each of the materials in the mesh
			for( UINT i = 0; i < dxMesh->m_dwNumMaterials; i++ )
			{
				V( pEffect->SetVector( "g_vDiffuse", ( Vec4* )&dxMesh->m_pMaterials[i].Diffuse ) );
				V( pEffect->SetTexture( "g_txScene", dxMesh->m_pTextures[i] ) );
				V( pEffect->CommitChanges() );

				tempMesh->DrawSubset( i );
			}

			V( pEffect->EndPass() );
		}
        V( pEffect->End() );
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////		
// 
//	Public Utility Functions
//		
//////////////////////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------
// Function: adjustButtonImage
// This function increments or decrements the specified button image index.   
// Each button has two images, one for being selected and one for being inactive. The
// caller of this function should be aware which buttons are active and inactive and
// call this function accordingly, because calling a decrement on an inactive button 
// will cause it to display the active image of another button.  
// INPUTS:  int buttonIndex	- button number to adjust image of
//			int adjust		- use "+1" to make active, and "-1" to make inactive.
//--------------------------------------------------------------------------------------
void Renderer::adjustButtonImage( int buttonIndex, int adjust )
{
	m_iButtonImages[ buttonIndex ] += adjust;
}


//--------------------------------------------------------------------------------------
// Function: adjustBallImage
// This function increments or decrements the specified ball image index.   
// There are two ball images, one small and one big.  The order is small then big.
// INPUTS:  int bballIndex	- ball number to adjust image of
//			int adjust		- use "+1" to make ball bigger, and "-1" to make ball smaller.
//--------------------------------------------------------------------------------------
void Renderer::adjustBallImage( int ballIndex, int adjust )
{
	m_iBallImages[ ballIndex ] += adjust;
}


//--------------------------------------------------------------------------------------
// Function: adjustRankImage
// This function sets the ranking image for the player.
// INPUTS:  int rank - 1, 2, 3 or 4
//--------------------------------------------------------------------------------------
void Renderer::adjustRankImage( int rank )
{
	m_iPlayerOneRank = m_iPlayerOneImage + rank - 1;
}


//--------------------------------------------------------------------------------------
// Function: adjustClockImages
// This function sets the clock images for the HUD time display.
// INPUTS:  string time - the time comes in the format mm:ss:msms
//--------------------------------------------------------------------------------------
void Renderer::adjustClockImages( string time )
{
	char temp;
	//mm
	temp = time.at(0);
	m_iTimeImages[0] = m_iClockImage + atoi( &temp );
	temp = time.at(1);
	m_iTimeImages[1] = m_iClockImage + atoi( &temp );
	
	//ss
	temp = time.at(3);
	m_iTimeImages[3] = m_iClockImage + atoi( &temp );
	temp = time.at(4);
	m_iTimeImages[4] = m_iClockImage + atoi( &temp );

	//ms ms
	temp = time.at(6);
	m_iTimeImages[6] = m_iClockImage + atoi( &temp );
	temp = time.at(7);
	m_iTimeImages[7] = m_iClockImage + atoi( &temp );
}


//--------------------------------------------------------------------------------------
// Function: adjustSpeedImage
// This function sets the speedometer wand image rotation based on the incoming speed.
// INPUTS:  float speed - in meters/ms
//--------------------------------------------------------------------------------------
void Renderer::adjustSpeedImage( float speed )
{
	float maxSpeed = 85;
	float maxRadians = 4.41238898;

	speed = speed / (MAX_SPEED_TIME_ELAPSED+0.1) * maxSpeed;

	if( speed > maxSpeed )
		speed = maxSpeed;

	m_fSpeedRotation = maxRadians * ( speed / maxSpeed );
}


//--------------------------------------------------------------------------------------
// Function: adjustVictoryRank
// This function formats the strings that will display on the victory screen.  The 
// strings will reflect the ranking of players when the game is over.
//--------------------------------------------------------------------------------------
void Renderer::adjustVictoryRank( vector<int>& ranks )
{
	for( int i=0; i < ranks.size(); i++ )
	{
		m_sVictoryRanks[ ranks[i] ] =  L"PLAYER #" + i;
	}
}


//--------------------------------------------------------------------------------------
// Function: adjustWrongWay
// This function sets the flag that tells the renderer to draw the Wrong Way message.
// INPUTS:  int playerNum - the player number to set the wrong way image for (Player 1 = 0)
//			bool drawWrongWay -  TRUE = draw the message, FALSE = don't draw it.
//--------------------------------------------------------------------------------------
void Renderer::adjustWrongWay( int playerNum, bool drawWrongWay )
{
	if( playerNum >= NUM_PLAYERS || playerNum < 0 )
		return;

	m_bDrawWrongWay[playerNum] = drawWrongWay;
}


//--------------------------------------------------------------------------------------
// Function: adjustLapImage
// This function sets the lap image for the specified player.  There are only images
// for one lap and two laps...This can easily be changed if we decide there should
// be more laps.
// INPUTS:  int playerNum - the player number to set the lap image for (Player 1 = 0)
//			int lapNum	  - the lap number (1 or 2 are the only valid arguments)
//--------------------------------------------------------------------------------------
void Renderer::adjustLapImage( int playerNum, int lapNum )
{
	if( playerNum >= NUM_PLAYERS || playerNum < 0 )
		return;

	if( lapNum == 1 )
		m_iLapImages[ playerNum ] = LAPONE_IMAGE;
	else if( lapNum == 2 )
		m_iLapImages[ playerNum ] = LAPTWO_IMAGE;
}


//////////////////////////////////////////////////////////////////////////////////////////////		
// 
//	Private Utility Functions
//		
//////////////////////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------
// Function: loadImages
// Creates textures for all images used in game and stores their rendering positions
// to be used later.
// INPUTS:  Device* device	- the IDirect3DDevice9 device to load images on
//			UINT width		- the width of the back buffer surface
//			UINT height		- the height of the back buffer surface
//--------------------------------------------------------------------------------------
void Renderer::loadImages( Device* device, UINT width, UINT height )
{
	//Non-button images
	createTexture( m_Images[ MENU_IMAGE ], MENU_IMAGE_FILE, device );
	createTexture( m_Images[ LOADING_IMAGE ],  LOADING_IMAGE_FILE, device );
	createTexture( m_Images[ LOADING_SMALLBALL_IMAGE ], LOADING_SMALLBALL_IMAGE_FILE, device );
	createTexture( m_Images[ LOADING_BIGBALL_IMAGE ], LOADING_BIGBALL_IMAGE_FILE, device );
	createTexture( m_Images[ CONTROLS_IMAGE ], CONTROLS_IMAGE_FILE, device );
	createTexture( m_Images[ GAMERULES_INFO_IMAGE ], GAMERULES_INFO_IMAGE_FILE, device );
	createTexture( m_Images[ GAMERULES_INFO_SMALL_IMAGE ], GAMERULES_INFO_SMALL_IMAGE_FILE, device );
	
	//HUD images
	createTexture( m_Images[ SPEEDOMETER_IMAGE ], SPEEDOMETER_IMAGE_FILE, device );
	createTexture( m_Images[ SPEEDWAND_IMAGE ], SPEEDWAND_IMAGE_FILE, device );
	createTexture( m_Images[ MINIMAP_IMAGE ], MINIMAP_IMAGE_FILE, device );
	createTexture( m_Images[ FIRST_IMAGE ], FIRST_IMAGE_FILE, device );
	createTexture( m_Images[ SECOND_IMAGE ], SECOND_IMAGE_FILE, device );
	createTexture( m_Images[ THIRD_IMAGE ], THIRD_IMAGE_FILE, device );
	createTexture( m_Images[ FOURTH_IMAGE ], FOURTH_IMAGE_FILE, device );
	createTexture( m_Images[ WRONGWAY_IMAGE ], WRONGWAY_IMAGE_FILE, device );
	createTexture( m_Images[ LAPONE_IMAGE ], LAPONE_IMAGE_FILE, device );
	createTexture( m_Images[ LAPTWO_IMAGE ], LAPTWO_IMAGE_FILE, device );

	//Number images
	createTexture( m_Images[ ZERO_IMAGE ], ZERO_IMAGE_FILE, device );
	createTexture( m_Images[ ONE_IMAGE ], ONE_IMAGE_FILE, device );
	createTexture( m_Images[ TWO_IMAGE ], TWO_IMAGE_FILE, device );
	createTexture( m_Images[ THREE_IMAGE ], THREE_IMAGE_FILE, device );
	createTexture( m_Images[ FOUR_IMAGE ], FOUR_IMAGE_FILE, device );
	createTexture( m_Images[ FIVE_IMAGE ], FIVE_IMAGE_FILE, device );
	createTexture( m_Images[ SIX_IMAGE ], SIX_IMAGE_FILE, device );
	createTexture( m_Images[ SEVEN_IMAGE ], SEVEN_IMAGE_FILE, device );
	createTexture( m_Images[ EIGHT_IMAGE ], EIGHT_IMAGE_FILE, device );
	createTexture( m_Images[ NINE_IMAGE ], NINE_IMAGE_FILE, device );
	createTexture( m_Images[ COLON_IMAGE ], COLON_IMAGE_FILE, device );

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

	//Victory screen images and buttons
	createTexture( m_Images[ VICTORY_IMAGE ], VICTORY_IMAGE_FILE, device );
	createTexture( m_Images[ MAINMENU_IMAGE ],MAINMENU_IMAGE_FILE, device );
	createTexture( m_Images[ MAINMENU_ACTIVE_IMAGE ], MAINMENU_ACTIVE_IMAGE_FILE, device );
	createTexture( m_Images[ EXIT_SMALL_IMAGE ], EXIT_SMALL_IMAGE_FILE, device );
	createTexture( m_Images[ EXIT_SMALL_ACTIVE_IMAGE ], EXIT_SMALL_ACTIVE_IMAGE_FILE, device );

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

	//Position Game Rule screen images
	w = ( width - 800 ) / 2; if( w<0 ) w=0;
	h = ( height - 700 ) / 2; if( h<0 ) h=0;
	m_ImageLocations[GAMERULES_INFO_IMAGE] = Vec3( w, h, 0 );

	w = ( width - 650 ) / 2; if( w<0 ) w=0;
	h = ( height - 550 ) / 2; if( h<0 ) h=0;
	m_ImageLocations[GAMERULES_INFO_SMALL_IMAGE] = Vec3( w, h, 0 );

	//Position HUD images
	m_ImageLocations[SPEEDOMETER_IMAGE] = Vec3( 10, height-180, 0 );
	m_ImageLocations[SPEEDWAND_IMAGE] = Vec3( 50, height-70, 0 );
	m_ImageLocations[MINIMAP_IMAGE] = Vec3( width-260, height-180, 0 );
	m_ImageLocations[FIRST_IMAGE] = Vec3( width-160, 10, 0 );
	m_ImageLocations[SECOND_IMAGE] = Vec3( width-160, 10, 0 );
	m_ImageLocations[THIRD_IMAGE] = Vec3( width-160, 10, 0 );
	m_ImageLocations[FOURTH_IMAGE] = Vec3( width-160, 10, 0 );
	m_ImageLocations[WRONGWAY_IMAGE] = Vec3( (width-400)/2, 10, 0 );
	m_ImageLocations[LAPONE_IMAGE] = Vec3( 10, 10, 0 );
	m_ImageLocations[LAPTWO_IMAGE] = Vec3( 10, 10, 0 );

	//Position number images
	w = ( width - 160 ); if( w<0 ) w=0;
	for( int i=0; i < 8; i++ )
	{
		m_TimeLocations[i] = Vec3( w, 85, 0 );
		w += 18;
	}

	//Position victory screen images
	w = ( width - 800 ) / 2; if( w<0 ) w=0;
	h = ( height - 700 ) / 2; if( h<0 ) h=0;
	m_ImageLocations[VICTORY_IMAGE] = Vec3( w, h, 0 );

	//Position the victory screen text
	RECT temp;
	//Winner
	temp.left = w+305;  temp.right= temp.left+200; temp.top=h+215;  temp.bottom=temp.top+40;
	m_VictoryRecs[0] = RECT(temp);
	temp.left = w+305;  temp.right= temp.left+200; temp.top=h+260;  temp.bottom=temp.top+40;
	m_VictoryRecs[NUM_PLAYERS] = RECT(temp);

	//2nd Place
	temp.left = w+70;  temp.right=temp.left+140; temp.top=h+420;  temp.bottom=temp.top+20;
	m_VictoryRecs[1] = RECT(temp);
	temp.left = w+100;  temp.right= temp.left+90; temp.top=h+450;  temp.bottom=temp.top+60;
	m_VictoryRecs[NUM_PLAYERS+1] = RECT(temp);

	//3rd Place
	temp.left = w+340;  temp.right=temp.left+140; temp.top=h+420;  temp.bottom=temp.top+20;
	m_VictoryRecs[2] = RECT(temp);
	temp.left = w+365;  temp.right= temp.left+90; temp.top=h+450;  temp.bottom=temp.top+60;
	m_VictoryRecs[NUM_PLAYERS+2] = RECT(temp);

	//4th Place
	temp.left = w+600;  temp.right=temp.left+140; temp.top=h+420;  temp.bottom=temp.top+20;
	m_VictoryRecs[3] = RECT(temp);
	temp.left = w+625;  temp.right= temp.left+90; temp.top=h+450;  temp.bottom=temp.top+60;
	m_VictoryRecs[NUM_PLAYERS+3] = RECT(temp);


	w = width - 300; if( w<0 ) w=0;
	h = height - 100;  if( h<0 ) h=0;
	m_ImageLocations[MAINMENU_IMAGE] = Vec3( w, h, 0 );
	m_ImageLocations[MAINMENU_ACTIVE_IMAGE] = Vec3( w, h, 0 );
	h += 40;
	m_ImageLocations[EXIT_SMALL_IMAGE] = Vec3( w, h, 0 );
	m_ImageLocations[EXIT_SMALL_ACTIVE_IMAGE] = Vec3( w, h, 0 );
}


//--------------------------------------------------------------------------------------
// Function: createTexture
// Creates a sprite and stores it in the specified texture variable.
// INPUTS:	LPCSTR file		- filename of image to load
//			Device* device	- the IDirect3DDevice9 device to load images on
// OUTPUT:	Sprite& texture	- LPDIRECT3DTEXTURE9 reference to be
//--------------------------------------------------------------------------------------
void Renderer::createTexture( Sprite& texture, LPCSTR file, Device* device )
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
