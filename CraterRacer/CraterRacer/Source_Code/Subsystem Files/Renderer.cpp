/*
* The Rendering subsystem handles all drawing to the screen for our game.  This includes all the 
* game menus, the gameplay and HUD.
*/

#include "Renderer.h"
#include "MessageManager.h"

VElement g_aVertDecl[] =
{
    { 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
    { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
    { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
    D3DDECL_END()
};


void Renderer::setLightParams( Vec3& pos, Vec3& lookAt )
{
	m_vFromPt = pos;
	m_vLookatPt = lookAt;
	m_LCamera.SetViewParams( &m_vFromPt, &m_vLookatPt );
}

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
	m_pVertDecl		= NULL;
	m_pTexDef		= NULL;
	m_pShadowMap	= NULL;
	m_pDSShadow		= NULL;
	m_pEffect		= NULL;
	m_dwFlush       = 512;  // Number of point sprites to load before sending them to hardware

	m_pVB = NULL;
	m_ptexParticle = NULL;

	// Initialize the camera
    m_vFromPt = Vec3( -220.0f, 300.0f, 10.0f  );
    m_vLookatPt = Vec3( -219.0f, -1.0f, -1.0f );
    m_LCamera.SetViewParams( &m_vFromPt, &m_vLookatPt );

    // Initialize the spot light
    m_fLightFov = D3DX_PI / 1.5f;

    m_Light.Diffuse.r = 1.0f;
    m_Light.Diffuse.g = 1.0f;
    m_Light.Diffuse.b = 1.0f;
    m_Light.Diffuse.a = 1.0f;
    m_Light.Position = Vec3( -220.0f, 200.0f, 0.0f );
    m_Light.Direction = Vec3( 0.0f, -1.0f, 0.0f );
    D3DXVec3Normalize( ( Vec3* )&m_Light.Direction, ( Vec3* )&m_Light.Direction );
    m_Light.Range = 1000.0f;
    m_Light.Theta = m_fLightFov / 2.0f;
    m_Light.Phi = m_fLightFov / 2.0f;

	//startup menu buttons
	m_iButtonImages[ GUI_BTN_SINGLE_PLAYER ] = ONEPLAYER_ACTIVE_IMAGE;
	m_iButtonImages[ GUI_BTN_TWO_PLAYER ] = TWOPLAYER_IMAGE;
	m_iButtonImages[ GUI_BTN_TIMETRIAL ] = TIMETRIAL_IMAGE;
	m_iButtonImages[ GUI_BTN_GAMERULES ] = GAMERULES_IMAGE;
	m_iButtonImages[ GUI_BTN_EXIT ] = EXIT_NOTACTIVE_IMAGE;

	//pause screen buttons
	m_iButtonImages[ GUI_BTN_UNPAUSE ] = UNPAUSE_ACTIVE_IMAGE;
	m_iButtonImages[ GUI_BTN_GAMERULES2 ] = GAMERULES2_IMAGE;
	m_iButtonImages[ GUI_BTN_PAUSE_MAINMENU ] = MAINMENU_PAUSE_IMAGE;
	m_iButtonImages[ GUI_BTN_EXIT2 ] = EXIT2_NOTACTIVE_IMAGE;

	//victory screen buttons
	m_iButtonImages[ GUI_BTN_MAINMENU ] = MAINMENU_ACTIVE_IMAGE;
	m_iButtonImages[ GUI_BTN_EXITSMALL ] = EXIT_SMALL_IMAGE;
	bestTimesIndex = 5;
	bestTimesName = L"";
	finishTime = L"";
	sBestTimesName = "";

	//Set up ball animation images for loading screen
	for( int i=0; i < NUM_LOADING_BALLS; i++ )
		m_iBallImages[i] = LOADING_SMALLBALL_IMAGE;

	//Set up time images to be zeros
	for( int i=0; i < 16; i++ )
		m_iTimeImages[i] = ZERO_IMAGE;

	m_iTimeImages[2]	= COLON_IMAGE;
	m_iTimeImages[5]	= COLON_IMAGE;
	m_iTimeImages[10]	= COLON_IMAGE;
	m_iTimeImages[13]	= COLON_IMAGE;

	m_iPlayerOneRank	= FIRST_IMAGE;
	m_iPlayerOneImage	= FIRST_IMAGE;
	m_iPlayerTwoRank	= FIRST_IMAGE;
	m_iPlayerTwoImage	= FIRST_IMAGE;

	m_iClockImage		= ZERO_IMAGE;
	m_fSpeedRotations[0]= 0;
	m_fSpeedRotations[1]= 0;
	
	m_bDrawWrongWay[ 0 ]= false;
	m_iLapImages[ 0 ]	= LAPONE_IMAGE;
	m_bDrawWrongWay[ 1 ]= false;
	m_iLapImages[ 1 ]	= LAPONE_IMAGE;

	m_bIsTwoPlayer = false;
	m_bIsTimeTrial = false;
	m_ScaleVal = Vec2( 1.0, 1.0 );

	//initialize the resource manager to keep track of all our screens and HUD
    m_GameScreen.Init( &m_ResourceManager );

	m_dwVBOffset       = 0;    // Gives the offset of the vertex buffer chunk that's currently being filled
    m_dwDiscard        = 2048; // Max number of point sprites the vertex buffer can load until we are forced to discard and start over
	m_fSize            = 50.0f;
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

	for( int i=0; i < NUM_HUD_IMAGES; i++ )
	{
		SAFE_RELEASE( m_HUDImages[i] );
		SAFE_RELEASE( m_HUDImages2[i] );
	}

	if( m_ptexParticle != NULL )
		m_ptexParticle->Release();
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

	// Load Texture Map for particles
	//createTexture( m_ptexParticle,  "Media\\Images\\particle.bmp", device );
	D3DXCreateTextureFromFile( device, L"Media\\Images\\particle.bmp", &m_ptexParticle );

	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	if( m_pEffect )
        V_RETURN( m_pEffect->OnResetDevice() );

	    // Setup the camera's projection parameters
    float fAspectRatio = pBack->Width / ( FLOAT )pBack->Height;

    m_LCamera.SetProjParams( D3DX_PI / 4, fAspectRatio, 0.1f, 100.0f );

    // Create the default texture (used when a triangle does not use a texture)
    V_RETURN( device->CreateTexture( 1, 1, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTexDef, NULL ) );
    D3DLOCKED_RECT lr;
    V_RETURN( m_pTexDef->LockRect( 0, &lr, NULL, 0 ) );
    *( LPDWORD )lr.pBits = D3DCOLOR_RGBA( 255, 255, 255, 255 );
    V_RETURN( m_pTexDef->UnlockRect( 0 ) );

	
    // Restore the effect variables
    V_RETURN( m_pEffect->SetVector( "g_vLightDiffuse", ( D3DXVECTOR4* )&m_Light.Diffuse ) );
    V_RETURN( m_pEffect->SetFloat( "g_fCosTheta", cosf( m_Light.Theta ) ) );

    // Create the shadow map texture
    V_RETURN( device->CreateTexture( SHADOWMAP_SIZE, SHADOWMAP_SIZE,
                                         1, D3DUSAGE_RENDERTARGET,
                                         D3DFMT_R32F,
                                         D3DPOOL_DEFAULT,
                                         &m_pShadowMap,
                                         NULL ) );

    DXUTDeviceSettings d3dSettings = DXUTGetDeviceSettings();
    V_RETURN( device->CreateDepthStencilSurface( SHADOWMAP_SIZE,
                                                     SHADOWMAP_SIZE,
                                                     d3dSettings.d3d9.pp.AutoDepthStencilFormat,
                                                     D3DMULTISAMPLE_NONE,
                                                     0,
                                                     TRUE,
                                                     &m_pDSShadow,
                                                     NULL ) );

    // Initialize the shadow projection matrix
    D3DXMatrixPerspectiveFovLH( &m_mShadowProj, m_fLightFov, fAspectRatio, 0.01f, 100.0f );

	// Particle stuff
    if( FAILED( hr = device->CreateVertexBuffer( 
        m_dwDiscard * sizeof(PointVertex), 
        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS, 
        PointVertex::FVF_Flags, // Our custom FVF
        D3DPOOL_DEFAULT, 
        &m_pVB, NULL )))
    {
        return E_FAIL;
    }


    return S_OK;
}


//--------------------------------------------------------------------------------------
// Function: OnCreate
// Creates the fonts attached to the device.
// INPUT: Device* device -> the device that has been created.	
//--------------------------------------------------------------------------------------
HRESULT Renderer::OnCreate( Device* device )
{
	HRESULT hr;
	//D3DXCreateTextureFromFile( device, L"Media//Images//Logo.png", &m_Skybox );

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
	

	DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;

	// If this fails, there should be debug output as to 
    // they the .fx file failed to compile
    V_RETURN( D3DXCreateEffectFromFile( device, L"Source_Code\\Effect_Files\\ShadowMap.fx", NULL, NULL, dwShaderFlags,
                                        NULL, &m_pEffect, NULL ) );

	// Create vertex declaration
    V_RETURN( device->CreateVertexDeclaration( g_aVertDecl, &m_pVertDecl ) );

	// World transform to identity
    Matrix mIdent;
    D3DXMatrixIdentity( &mIdent );
    V_RETURN( device->SetTransform( D3DTS_WORLD, &mIdent ) );


	//Particle stuff
	// Get max point size
    D3DCAPS9 d3dCaps;
    device->GetDeviceCaps( &d3dCaps );
    m_fMaxPointSize = d3dCaps.MaxPointSize;

	// Check and see if we can change the size of point sprites 
    // in hardware by sending D3DFVF_PSIZE with the FVF.
    if( d3dCaps.FVFCaps & D3DFVFCAPS_PSIZE )
        m_bDeviceSupportsPSIZE = true;
    else
        m_bDeviceSupportsPSIZE = false;

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

	if( m_pEffect )
        m_pEffect->OnLostDevice();

    SAFE_RELEASE( m_pDSShadow );
    SAFE_RELEASE( m_pShadowMap );
    SAFE_RELEASE( m_pTexDef );

	if( m_pVB != NULL )
		m_pVB->Release();

    if( m_ptexParticle != NULL )
		m_ptexParticle->Release();
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

    SAFE_RELEASE( m_pEffect );
    SAFE_RELEASE( m_pVertDecl );
}


//////////////////////////////////////////////////////////////////////////////////////////////		
// 
//	Public Drawing Functions
//		
//////////////////////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------
// Function: drawParticles
// This function draws a list of particles.
//--------------------------------------------------------------------------------------
HRESULT Renderer::drawParticles( Device* device, vector<Particle*> particles )
{
	HRESULT hr;
	
	//Set up render states
	device->SetTexture(0,this->m_ptexParticle);
	device->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	device->SetRenderState( D3DRS_DESTBLEND,D3DBLEND_ONE );
	device->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

	// Set the render states for using point sprites..
    device->SetRenderState( D3DRS_POINTSPRITEENABLE, TRUE );		// Turn on point sprites
    device->SetRenderState( D3DRS_POINTSCALEENABLE,  TRUE );		// Allow sprites to be scaled with distance
    device->SetRenderState( D3DRS_POINTSIZE,     FtoDW(1.0f) );		// Float value that specifies the size to use for point size computation in cases where point size is not specified for each vertex.
    device->SetRenderState( D3DRS_POINTSIZE_MIN, FtoDW(0.0f) );		// Float value that specifies the minimum size of point primitives. Point primitives are clamped to this size during rendering. 
    device->SetRenderState( D3DRS_POINTSCALE_A,  FtoDW(0.0f) );     // Default 1.0
    device->SetRenderState( D3DRS_POINTSCALE_B,  FtoDW(0.0f) );     // Default 0.0
    device->SetRenderState( D3DRS_POINTSCALE_C,  FtoDW(1.0f) );     // Default 0.0
	
	Particle* pParticle;
    PointVertex *pVertices;

	//Lock the vertex buffer
	m_pVB->Lock( 0, particles.size() * sizeof(PointVertex), (void**)&pVertices, D3DLOCK_DISCARD );

	// Render each particle
    for( int i=0; i < particles.size(); i++ )
    {
		pParticle = particles[i];
        pVertices->posit = pParticle->position;
        pVertices->color = D3DXCOLOR(0.3f,0.2f,0.08f,1.0f); //brown
        pVertices++;
    }
    // Unlock the vertex buffer
    m_pVB->Unlock();

    // Render any remaining particles
	device->SetStreamSource( 0, m_pVB, 0, sizeof(PointVertex) );
	device->SetFVF( PointVertex::FVF_Flags );

    if(FAILED(hr = device->DrawPrimitive( D3DPT_POINTLIST, m_dwVBOffset, particles.size() )))
		return hr;

    // Reset render states...
    device->SetRenderState( D3DRS_POINTSPRITEENABLE, FALSE );
    device->SetRenderState( D3DRS_POINTSCALEENABLE,  FALSE );
	device->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	device->SetRenderState( D3DRS_ALPHABLENDENABLE, false );

    return S_OK;
}


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
// Function: 
// This function draws the game rules screen as an overlay to the game (as a smaller
// rectangle inside the screen)
//--------------------------------------------------------------------------------------
void Renderer::drawCountdown( int num )
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
	m_GameScreen.DrawRect( &m_PauseRECT, D3DCOLOR_ARGB( 255, 0,0,0 ) );

	//draw the menu buttons centered in the black rectangle
	for( int i=GUI_BTN_UNPAUSE; i <= GUI_BTN_EXIT2; i++ )
	{
		m_pImageSprite->Flush();
		image = m_iButtonImages[ i ];
		V( m_pImageSprite->Draw( m_Images[ image ], NULL, &center, &m_ImageLocations[image], D3DCOLOR_ARGB( 255,255,255,255 ) ) );
	}
	m_pImageSprite->End( );
}


//--------------------------------------------------------------------------------------
// Function: drawHUD( )
// Renders the speedometer, time display, ranking and minimap overtop of the game.
//--------------------------------------------------------------------------------------
void Renderer::drawHUD( int playerNum )
{	
	//render the HUD
	m_pImageSprite->Begin( D3DXSPRITE_ALPHABLEND );
	
	if( playerNum == PLAYER2 )
		drawAHUD( m_HUDImages2, m_HUDImageLocations2, PLAYER2 );
	else
		drawAHUD( m_HUDImages, m_HUDImageLocations, PLAYER1 );

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
// Function:  renderCountDown
// This function renders the count down. Needs to be rendered nicely!
//--------------------------------------------------------------------------------------
void Renderer::renderCountDown( int count )
{
	HRESULT hr;
	Matrix mat;
	Vec2 trans = Vec2( this->m_CountdownLocation.x, m_CountdownLocation.y );

	Vec2 scaling( 1.0, 1.0 );
	int numImage = ZERO_COUNT_IMAGE + count;
	
	m_pImageSprite->Begin( D3DXSPRITE_ALPHABLEND );

	m_pImageSprite->Flush();
	D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, NULL, 0.0, &trans );
	m_pImageSprite->SetTransform( &mat );
	V( m_pImageSprite->Draw( m_Images[numImage], NULL, NULL, NULL, D3DCOLOR_ARGB( 255,255,255,255 ) ) );

	m_pImageSprite->End();
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
	for (int i = 0; i < 5; i++) {
		m_pFontVictorySmall->DrawTextW( NULL, this->m_sVictoryRanks[i].c_str(), -1, &m_VictoryRecs[i], DT_CENTER, D3DCOLOR_ARGB( 255,0,0,0 ) );
		m_pFontVictorySmall->DrawTextW( NULL, this->m_sVictoryTimes[i].c_str(), -1, &m_VictoryRecs[i+NUM_PLAYERS], DT_CENTER, D3DCOLOR_ARGB( 255,0,0,0 ) );
	}

	m_pImageSprite->End( );
}


//--------------------------------------------------------------------------------------
// Function:  drawTimesScreen
// This function renders the times screen. 
//--------------------------------------------------------------------------------------
void Renderer::drawTimesScreen(int letter)
{
	HRESULT hr;
	int image = 0;

	//render the game menu
	m_pImageSprite->Begin( D3DXSPRITE_ALPHABLEND );
	
	m_pImageSprite->Flush();
	V( m_pImageSprite->Draw( m_Images[ TIMES_IMAGE ], NULL, NULL, &m_ImageLocations[ TIMES_IMAGE ], D3DCOLOR_ARGB( 255,255,255,255 ) ) );

	//draw the times buttons
	for( int i=GUI_BTN_MAINMENU; i <= GUI_BTN_EXITSMALL; i++ )
	{
		m_pImageSprite->Flush();
		image = m_iButtonImages[ i ];
		V( m_pImageSprite->Draw( m_Images[ image ], NULL, NULL, &m_ImageLocations[image], D3DCOLOR_ARGB( 255,255,255,255 ) ) );
	}

	//Draw best times as text
	for (int i = 0; i < 5; i++) {
		if (i == bestTimesIndex) {
			m_pFontVictoryBig->DrawTextW( NULL, this->m_sBestTimes[i].c_str(), -1, &m_BestTimesRecs[i], DT_EXPANDTABS, D3DCOLOR_ARGB( 255,0,0,255 ) );
			m_pFontVictoryBig->DrawTextW( NULL, bestTimesName.c_str(), -1, &m_BestNamesRecs[i], DT_EXPANDTABS, D3DCOLOR_ARGB( 255,0,0,255 ) );

			if (bestTimesName.length() < 3) {
				wstring tempStr = L"";
				tempStr.push_back((char)letter);
				RECT tempRect(m_BestNamesRecs[i]);
				tempRect.left += (bestTimesName.length()*15);

				m_pFontVictoryBig->DrawTextW( NULL, tempStr.c_str(), -1, &tempRect, DT_EXPANDTABS, D3DCOLOR_ARGB( 255,255,0,0 ) );

				if (bestTimesName.length() < 2) {
					tempStr = L"-";
					tempRect = m_BestNamesRecs[i];
					tempRect.left += 30;

					if (bestTimesName.length() == 0) {
						tempStr.push_back('-');
						tempRect.left -= 15;
					}

					m_pFontVictoryBig->DrawTextW( NULL, tempStr.c_str(), -1, &tempRect, DT_EXPANDTABS, D3DCOLOR_ARGB( 255,0,0,255 ) );
				}
			}
		}
		else {
			m_pFontVictoryBig->DrawTextW( NULL, this->m_sBestTimes[i].c_str(), -1, &m_BestTimesRecs[i], DT_EXPANDTABS, D3DCOLOR_ARGB( 255,0,0,0 ) );
			m_pFontVictoryBig->DrawTextW( NULL, this->m_sBestNames[i].c_str(), -1, &m_BestNamesRecs[i], DT_EXPANDTABS, D3DCOLOR_ARGB( 255,0,0,0 ) );
		}
	}

	if (bestTimesIndex >= 5) {
		m_pFontVictoryBig->DrawTextW( NULL, finishTime.c_str(), -1, &m_finishTime, DT_EXPANDTABS, D3DCOLOR_ARGB( 255,0,255,0 ) );
	}

	m_pImageSprite->End( );
}


//--------------------------------------------------------------------------------------
// Function: RenderScene
// Draws game objects to the specified device.  The renderables of these objects 
// are passed to this function and contain all the rendering-specific information needed
// by the renderer.  
//--------------------------------------------------------------------------------------
void Renderer::RenderScene( Device* device, bool bRenderShadow, const D3DXMATRIX* pmView,
                  const D3DXMATRIX* pmProj, vector<Renderable*> renderables )
{
    HRESULT hr;
	Vec3		vScale( 0.5, 1.0, 0.9 );
	Matrix		mScale, mView;
	LPD3DXMESH	tempMesh;
	Mesh		*dxMesh;	//the DXUT wrapper version of the mesh with helpful functions
	Renderable	*tempR;		//pointer to current renderable
	Effect		*pEffect;	//the effect currently being used

    // Set the projection matrix
    V( m_pEffect->SetMatrix( "g_mProj", pmProj ) );

    // Get light parameters from the light camera.
    D3DXVECTOR4 v4;
    D3DXVec3Transform( &v4, &m_vFromPt, pmView );
    V( m_pEffect->SetVector( "g_vLightPos", &v4 ) );
    *( Vec3* )&v4 = *m_LCamera.GetWorldAhead();
    v4.w = 0.0f;  // Set w 0 so that the translation part doesn't come to play
    D3DXVec4Transform( &v4, &v4, pmView );  // Direction in view space
    D3DXVec3Normalize( ( D3DXVECTOR3* )&v4, ( D3DXVECTOR3* )&v4 );
    V( m_pEffect->SetVector( "g_vLightDir", &v4 ) );

    // Clear the render buffers
    V( device->Clear( 0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L ) );

    if( bRenderShadow )
        V( m_pEffect->SetTechnique( "RenderShadow" ) );

    if( !bRenderShadow )
        V( m_pEffect->SetTechnique( "RenderScene" ) );

       // now go through and render each object via its renderable
	for( unsigned int i = 0; i < renderables.size(); i++ )
	{
		// initialize some temporary variables to make things easier
		tempR = renderables[i];
		dxMesh = tempR->m_pMesh;
		tempMesh = dxMesh->GetMesh();
   
		if( m_bIsTwoPlayer )
		{
			D3DXMatrixTransformation( &mScale, NULL, NULL, &vScale, NULL, NULL, NULL );
			D3DXMatrixMultiply( &mView, pmView, &mScale );
		}
		else 
			mView = *pmView;

		// create the world view projection matrix
        Matrix mWorldView;
        D3DXMatrixMultiply( &mWorldView, &tempR->m_matWorld, &mView );
        V( m_pEffect->SetMatrix( "g_mWorldView", &mWorldView ) );

            UINT cPass;
            V( m_pEffect->Begin( &cPass, 0 ) );
            for( UINT p = 0; p < cPass; ++p )
            {
                V( m_pEffect->BeginPass( p ) );

                for( DWORD i = 0; i < dxMesh->m_dwNumMaterials; ++i )
                {
                    D3DXVECTOR4 vDif( dxMesh->m_pMaterials[i].Diffuse.r,
                                      dxMesh->m_pMaterials[i].Diffuse.g,
                                      dxMesh->m_pMaterials[i].Diffuse.b,
                                      dxMesh->m_pMaterials[i].Diffuse.a );
                    V( m_pEffect->SetVector( "g_vMaterial", &vDif ) );
                    if( dxMesh->m_pTextures[i] )
                        V( m_pEffect->SetTexture( "g_txScene", dxMesh->m_pTextures[i] ) )
                    else
                        V( m_pEffect->SetTexture( "g_txScene", m_pTexDef ) )
                    V( m_pEffect->CommitChanges() );
                    V( tempMesh->DrawSubset( i ) );
                }
                V( m_pEffect->EndPass() );
            }
            V( m_pEffect->End() );
        }
}

//--------------------------------------------------------------------------------------
// This callback function will be called at the end of every frame to perform all the 
// rendering calls for the scene, and it will also be called if the window needs to be 
// repainted. After this function has returned, DXUT will call 
// IDirect3DDevice9::Present to display the contents of the next buffer in the swap chain
//--------------------------------------------------------------------------------------
void Renderer::RenderFrame( Device* device, vector<Renderable*> renderables, vector<GameCamera*> cameras, int playerID, D3DVIEWPORT9* viewport )
{
    HRESULT hr;
	MCamera	camera = cameras[ playerID ]->getCamera();
  
    // Compute the view matrix for the light
    Matrix mLightView = *m_LCamera.GetViewMatrix();
    
    // Render the shadow map
    Surface pOldRT = NULL;
    V( device->GetRenderTarget( 0, &pOldRT ) );
    Surface pShadowSurf;
    if( SUCCEEDED( m_pShadowMap->GetSurfaceLevel( 0, &pShadowSurf ) ) )
    {
        device->SetRenderTarget( 0, pShadowSurf );
		device->SetViewport( viewport );
        SAFE_RELEASE( pShadowSurf );
    }
    Surface pOldDS = NULL;
    if( SUCCEEDED( device->GetDepthStencilSurface( &pOldDS ) ) )
	{
        device->SetDepthStencilSurface( m_pDSShadow );
	}

	RenderScene( device, true, &mLightView, &m_mShadowProj, renderables );

    if( pOldDS )
    {
        device->SetDepthStencilSurface( pOldDS );
        pOldDS->Release();
    }
    device->SetRenderTarget( 0, pOldRT );
	device->SetViewport( viewport );
    SAFE_RELEASE( pOldRT );

    // Now that we have the shadow map, render the scene.
    const D3DXMATRIX* pmView = m_LCamera.GetViewMatrix();

    // Initialize required parameter
    V( m_pEffect->SetTexture( "g_txShadow", m_pShadowMap ) );

    // Compute the matrix to transform from view space to light projection space.  
    Matrix mViewToLightProj = *pmView;
    D3DXMatrixInverse( &mViewToLightProj, NULL, &mViewToLightProj );
    D3DXMatrixMultiply( &mViewToLightProj, &mViewToLightProj, &mLightView );
    D3DXMatrixMultiply( &mViewToLightProj, &mViewToLightProj, &m_mShadowProj );
    V( m_pEffect->SetMatrix( "g_mViewToLightProj", &mViewToLightProj ) );
    
	RenderScene( device, false, camera.GetViewMatrix(), camera.GetProjMatrix(), renderables );
    
    m_pEffect->SetTexture( "g_txShadow", NULL );

	//vector<Particle*> particles;
	//Particle* p, *p2, *p3;

	//Matrix w;
	//D3DXMatrixIdentity( &w );
	//device->SetTransform( D3DTS_WORLD, &w );
	////Set the default viewport and perspective matrices
	//device->SetTransform( D3DTS_VIEW, camera.GetViewMatrix() ); 
	//device->SetTransform( D3DTS_PROJECTION, camera.GetProjMatrix() );
	//p = new Particle();
	//p->position = Vec3(245, 31, 10);
	//particles.push_back( p );
	//p2 = new Particle();
	//p2->position = Vec3(245, 41, 10);
	//particles.push_back( p2 );
	//p3 = new Particle();
	//p3->position = Vec3(235, 41, 10);
	//particles.push_back( p3 );
	//
	//drawParticles( device, particles );
	//delete p;
	//delete p2;
	//delete p3;
}


//////////////////////////////////////////////////////////////////////////////////////////////		
// 
//	Public Utility Functions
//		
//////////////////////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------
// Function: adjustButtonImage
// This function increments or decrements the specified button image index.   
// Each button has two images, one for being selected and one for being inactive. 
// INPUTS:  int buttonIndex	- button number to adjust image of
//			int adjust		- use "+1" to make active, and "-1" to make inactive.
//--------------------------------------------------------------------------------------
void Renderer::adjustButtonImage( int buttonIndex, int adjust )
{
	if( adjust > 0 && m_iButtonImages[ buttonIndex ] % 2 == 0 )
		return;
	if( adjust < 0 && m_iButtonImages[ buttonIndex ] % 2 != 0 )
		return;

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
void Renderer::adjustRankImage( int rank, int playerID )
{
	if( playerID == PLAYER1 )
		m_iPlayerOneRank = m_iPlayerOneImage + rank - 1;
	else
		m_iPlayerTwoRank = m_iPlayerTwoImage + rank - 1;
}


//--------------------------------------------------------------------------------------
// Function: adjustClockImages
// This function sets the clock images for the HUD time display.
// INPUTS:  string time - the time comes in the format mm:ss:msms
//--------------------------------------------------------------------------------------
void Renderer::adjustClockImages( string time, int playerID )
{
	int off = playerID * 8;
	char temp;
	//mm
	temp = time.at(0);
	m_iTimeImages[0 + off] = m_iClockImage + atoi( &temp );
	temp = time.at(1);
	m_iTimeImages[1 + off] = m_iClockImage + atoi( &temp );
	
	//ss
	temp = time.at(3);
	m_iTimeImages[3 + off] = m_iClockImage + atoi( &temp );
	temp = time.at(4);
	m_iTimeImages[4 + off] = m_iClockImage + atoi( &temp );

	//ms ms
	temp = time.at(6);
	m_iTimeImages[6 + off] = m_iClockImage + atoi( &temp );
	temp = time.at(7);
	m_iTimeImages[7 + off] = m_iClockImage + atoi( &temp );
}


//--------------------------------------------------------------------------------------
// Function: adjustSpeedImage
// This function sets the speedometer wand image rotation based on the incoming speed.
// INPUTS:  float speed	 - in meters/ms
//			int playerID - since we are only doing two players, this will only be 0 or 1.
//--------------------------------------------------------------------------------------
void Renderer::adjustSpeedImage( float speed, int playerID )
{
	float maxSpeed = 80; // m/s
	float maxRadians = 4.41238898; //max rotation (where the 85 m/s mark on the speedometer would be)

	if( playerID < PLAYER1 || playerID > PLAYER2 )
		return;

	if( speed > maxSpeed )
		speed = maxSpeed;

	m_fSpeedRotations[playerID] = maxRadians * ( speed / maxSpeed );
}


//--------------------------------------------------------------------------------------
// Function: adjustVictoryRank
// This function formats the strings that will display on the victory screen.  The 
// strings will reflect the ranking of players when the game is over.
//--------------------------------------------------------------------------------------
void Renderer::adjustVictoryRank( vector<int>& ranks, vector<string>& times )
{
	string default = "--:--:--";

	//set rank text
	for( int i=0; i < ranks.size(); i++ )
	{
		m_sVictoryRanks[ ranks[i]-1 ] = L"Player #";
		m_sVictoryRanks[ ranks[i]-1 ].push_back (L'1' + i);

		if (ranks[i] == 1) {
			m_sVictoryTimes[ ranks[i]-1 ] = L"Time: ";
		}
		else {
			m_sVictoryTimes[ ranks[i]-1 ] = L"Time:\n";
		}
		if (times[i] != "00:00:00") {
			for (int j = 0; j < times[i].size(); j++) {
				m_sVictoryTimes[ ranks[i]-1 ].push_back ((WCHAR)times[i].at(j));
			}
		}
		else {
			for (int j = 0; j < default.size(); j++) {
				m_sVictoryTimes[ ranks[i]-1 ].push_back ((WCHAR)default.at(j));
			}
		}
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
	if( playerNum > PLAYER2 || playerNum < PLAYER1 )
		return;

	m_bDrawWrongWay[playerNum] = drawWrongWay;
}


//--------------------------------------------------------------------------------------
// Function: adjustBestTimes
//--------------------------------------------------------------------------------------
void Renderer::adjustBestTimes( vector<string>& bestTimeEntries, vector<string>& bestNameEntries, string time, int index)
{
	if( bestTimeEntries.size() != 5 || bestNameEntries.size() != 5 )
		return;

	bestTimesIndex = index;
	finishTime = L"Your Time: ";

	//set times text
	for( int i=0; i < 5; i++ )
	{
		m_sBestTimes[i] = L"";
		m_sBestNames[i] = L"";

		for (int j = 0; j < bestTimeEntries[i].size(); j++) {
			m_sBestTimes[i].push_back ((WCHAR)bestTimeEntries[i].at(j));
		}

		for (int j = 0; j < bestNameEntries[i].size(); j++) {
			m_sBestNames[i].push_back ((WCHAR)bestNameEntries[i].at(j));
		}
	}

	for (int i = 0; i < time.length(); i++) {
		finishTime.push_back((WCHAR)time.at(i));
	}
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
	if( playerNum > PLAYER2 || playerNum < PLAYER1 )
		return;

	if( lapNum == 1 )
		m_iLapImages[ playerNum ] = LAPONE_IMAGE;
	else if( lapNum == 2 )
		m_iLapImages[ playerNum ] = LAPTWO_IMAGE;
}


//--------------------------------------------------------------------------------------
// Function: adjustTwoPlayer
// This functions sets the flag for two player rendering (split screen horizontally).
// It rescales the HUD images to fit the new mode.
// INPUTS:	nool isTwoPlayer - new flag value
//			int width		 - the width of the back buffer surface
//			int height		 - the height of the back buffer surface
//--------------------------------------------------------------------------------------
void Renderer::adjustTwoPlayer( bool isTwoPlayer, int width, int height )
{
	m_bIsTwoPlayer = isTwoPlayer;
	m_ScaleVal = (m_bIsTwoPlayer) ? Vec2(0.75,0.75) : Vec2(1.0,1.0);
	positionHUDImages( width, height );
	m_bIsTimeTrial = false;
}


//--------------------------------------------------------------------------------------
// Function: adjustTimeTrial
// Call this function when running the time trial, to set the time trial flag.
//--------------------------------------------------------------------------------------
void Renderer::adjustTimeTrial( )
{
	m_bIsTimeTrial = true;
	m_bIsTwoPlayer = false;
}

//--------------------------------------------------------------------------------------
// Function: addLetter
// Adds a letter to the name being entered for high score on time trial
//--------------------------------------------------------------------------------------
void Renderer::addLetter( int letter )
{
	char ch = (char)letter;

	bestTimesName.push_back(ch);
	
	string temp;
	stringstream ss;
	ss << ch;
	ss >> temp;
	sBestTimesName = sBestTimesName.append(temp);

	if (sBestTimesName.length() == 3) {
		Emit(Events::ENameEntered, bestTimesIndex, sBestTimesName);
	}
}

//--------------------------------------------------------------------------------------
// Function: resetBestName
// Resets the two strings containing the best name for time trial
//--------------------------------------------------------------------------------------
void Renderer::resetBestName( )
{
	bestTimesName = L"";
	sBestTimesName = "";
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
	
	//Times screen
	createTexture( m_Images[ TIMES_IMAGE ], TIMES_IMAGE_FILE, device );
	createTexture( m_Images[ MAINMENU2_IMAGE ], MAINMENU_IMAGE_FILE, device );
	createTexture( m_Images[ MAINMENU2_ACTIVE_IMAGE ], MAINMENU_ACTIVE_IMAGE_FILE, device );
	createTexture( m_Images[ EXITSMALL2_IMAGE ], EXIT_SMALL_IMAGE_FILE, device );
	createTexture( m_Images[ EXITSMALL2_ACTIVE_IMAGE ], EXIT_SMALL_ACTIVE_IMAGE_FILE, device );
	
	createTexture( m_Images[ ZERO_COUNT_IMAGE ], ZERO_COUNT_IMAGE_FILE, device );
	createTexture( m_Images[ ONE_COUNT_IMAGE ], ONE_COUNT_IMAGE_FILE, device );
	createTexture( m_Images[ TWO_COUNT_IMAGE ], TWO_COUNT_IMAGE_FILE, device );
	createTexture( m_Images[ THREE_COUNT_IMAGE ], THREE_COUNT_IMAGE_FILE, device );
	
	//HUD images
	createTexture( m_HUDImages[ SPEEDOMETER_IMAGE ], SPEEDOMETER_IMAGE_FILE, device );
	createTexture( m_HUDImages[ SPEEDWAND_IMAGE ], SPEEDWAND_IMAGE_FILE, device );
	createTexture( m_HUDImages[ MINIMAP_IMAGE ], MINIMAP_IMAGE_FILE, device );
	createTexture( m_HUDImages[ FIRST_IMAGE ], FIRST_IMAGE_FILE, device );
	createTexture( m_HUDImages[ SECOND_IMAGE ], SECOND_IMAGE_FILE, device );
	createTexture( m_HUDImages[ THIRD_IMAGE ], THIRD_IMAGE_FILE, device );
	createTexture( m_HUDImages[ FOURTH_IMAGE ], FOURTH_IMAGE_FILE, device );
	createTexture( m_HUDImages[ FIFTH_IMAGE ], FIFTH_IMAGE_FILE, device );
	createTexture( m_HUDImages[ SIXTH_IMAGE ], SIXTH_IMAGE_FILE, device );
	createTexture( m_HUDImages[ SEVENTH_IMAGE ], SEVENTH_IMAGE_FILE, device );
	createTexture( m_HUDImages[ EIGTH_IMAGE ], EIGTH_IMAGE_FILE, device );
	createTexture( m_HUDImages[ WRONGWAY_IMAGE ], WRONGWAY_IMAGE_FILE, device );
	createTexture( m_HUDImages[ LAPONE_IMAGE ], LAPONE_IMAGE_FILE, device );
	createTexture( m_HUDImages[ LAPTWO_IMAGE ], LAPTWO_IMAGE_FILE, device );

	//2 Player HUD images
	createTexture( m_HUDImages2[ SPEEDOMETER_IMAGE ], SPEEDOMETER2_IMAGE_FILE, device );
	createTexture( m_HUDImages2[ SPEEDWAND_IMAGE ], SPEEDWAND_IMAGE_FILE, device );
	createTexture( m_HUDImages2[ MINIMAP_IMAGE ], MINIMAP_IMAGE_FILE, device );
	createTexture( m_HUDImages2[ FIRST_IMAGE ], FIRST2_IMAGE_FILE, device );
	createTexture( m_HUDImages2[ SECOND_IMAGE ], SECOND2_IMAGE_FILE, device );
	createTexture( m_HUDImages2[ THIRD_IMAGE ], THIRD2_IMAGE_FILE, device );
	createTexture( m_HUDImages2[ FOURTH_IMAGE ], FOURTH2_IMAGE_FILE, device );
	createTexture( m_HUDImages2[ FIFTH_IMAGE ], FIFTH2_IMAGE_FILE, device );
	createTexture( m_HUDImages2[ SIXTH_IMAGE ], SIXTH2_IMAGE_FILE, device );
	createTexture( m_HUDImages2[ SEVENTH_IMAGE ], SEVENTH2_IMAGE_FILE, device );
	createTexture( m_HUDImages2[ EIGTH_IMAGE ], EIGTH2_IMAGE_FILE, device );
	createTexture( m_HUDImages2[ WRONGWAY_IMAGE ], WRONGWAY_IMAGE_FILE, device );
	createTexture( m_HUDImages2[ LAPONE_IMAGE ], LAPONE2_IMAGE_FILE, device );
	createTexture( m_HUDImages2[ LAPTWO_IMAGE ], LAPTWO2_IMAGE_FILE, device );

	//Number images
	createTexture( m_HUDImages[ ZERO_IMAGE ], ZERO_IMAGE_FILE, device );
	createTexture( m_HUDImages[ ONE_IMAGE ], ONE_IMAGE_FILE, device );
	createTexture( m_HUDImages[ TWO_IMAGE ], TWO_IMAGE_FILE, device );
	createTexture( m_HUDImages[ THREE_IMAGE ], THREE_IMAGE_FILE, device );
	createTexture( m_HUDImages[ FOUR_IMAGE ], FOUR_IMAGE_FILE, device );
	createTexture( m_HUDImages[ FIVE_IMAGE ], FIVE_IMAGE_FILE, device );
	createTexture( m_HUDImages[ SIX_IMAGE ], SIX_IMAGE_FILE, device );
	createTexture( m_HUDImages[ SEVEN_IMAGE ], SEVEN_IMAGE_FILE, device );
	createTexture( m_HUDImages[ EIGHT_IMAGE ], EIGHT_IMAGE_FILE, device );
	createTexture( m_HUDImages[ NINE_IMAGE ], NINE_IMAGE_FILE, device );
	createTexture( m_HUDImages[ COLON_IMAGE ], COLON_IMAGE_FILE, device );

	//Number images for player2
	createTexture( m_HUDImages2[ ZERO_IMAGE ], ZERO2_IMAGE_FILE, device );
	createTexture( m_HUDImages2[ ONE_IMAGE ], ONE2_IMAGE_FILE, device );
	createTexture( m_HUDImages2[ TWO_IMAGE ], TWO2_IMAGE_FILE, device );
	createTexture( m_HUDImages2[ THREE_IMAGE ], THREE2_IMAGE_FILE, device );
	createTexture( m_HUDImages2[ FOUR_IMAGE ], FOUR2_IMAGE_FILE, device );
	createTexture( m_HUDImages2[ FIVE_IMAGE ], FIVE2_IMAGE_FILE, device );
	createTexture( m_HUDImages2[ SIX_IMAGE ], SIX2_IMAGE_FILE, device );
	createTexture( m_HUDImages2[ SEVEN_IMAGE ], SEVEN2_IMAGE_FILE, device );
	createTexture( m_HUDImages2[ EIGHT_IMAGE ], EIGHT2_IMAGE_FILE, device );
	createTexture( m_HUDImages2[ NINE_IMAGE ], NINE2_IMAGE_FILE, device );
	createTexture( m_HUDImages2[ COLON_IMAGE ], COLON2_IMAGE_FILE, device );

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
	createTexture( m_Images[ MAINMENU_PAUSE_IMAGE ], MAINMENU_IMAGE_FILE, device );
	createTexture( m_Images[ MAINMENU_PAUSE_ACTIVE_IMAGE ], MAINMENU_ACTIVE_IMAGE_FILE, device );
	createTexture( m_Images[ EXIT2_NOTACTIVE_IMAGE ], EXIT_NOTACTIVE_IMAGE_FILE, device );
	createTexture( m_Images[ EXIT2_ACTIVE_IMAGE ], EXIT_ACTIVE_IMAGE_FILE, device );

	//Victory screen images and buttons
	createTexture( m_Images[ VICTORY_IMAGE ], VICTORY_IMAGE_FILE, device );
	createTexture( m_Images[ MAINMENU_IMAGE ],MAINMENU_IMAGE_FILE, device );
	createTexture( m_Images[ MAINMENU_ACTIVE_IMAGE ], MAINMENU_ACTIVE_IMAGE_FILE, device );
	createTexture( m_Images[ EXIT_SMALL_IMAGE ], EXIT_SMALL_IMAGE_FILE, device );
	createTexture( m_Images[ EXIT_SMALL_ACTIVE_IMAGE ], EXIT_SMALL_ACTIVE_IMAGE_FILE, device );

	positionMainImages( width, height );
	positionHUDImages( width, height );
}


//--------------------------------------------------------------------------------------
// Function: positionMainImages
// This function sets the location vectors for all of the menu images and non-HUD images.
// INPUTS:	int width		- width of the screen to draw to
//			int height		- height of screen to draw to
//--------------------------------------------------------------------------------------
void Renderer::positionMainImages( int width, int height )
{
	//Position the startup menu buttons and images --------------
	int w = (width - 200)/2; if( w<0 ) w=0;
	int h = (height - 200)/2; if( h<0 ) h=0;

	this->m_CountdownLocation.x = w;
	this->m_CountdownLocation.y = h;

	//Position the startup menu buttons and images --------------
	 w = (width - 400)/2; if( w<0 ) w=0;
	 h = (height - 430)/2; if( h<0 ) h=0;

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
	int w2 = (width - 200) / 2; if( w<0 ) w=0;
	h = (height - 160) / 2;  if( h<0 ) h=0;
	m_ImageLocations[UNPAUSE_IMAGE] = Vec3( w, h, 0 );
	m_ImageLocations[UNPAUSE_ACTIVE_IMAGE] = Vec3( w, h, 0 );
	h += 40;
	m_ImageLocations[GAMERULES2_IMAGE] = Vec3( w, h, 0 );
	m_ImageLocations[GAMERULES2_ACTIVE_IMAGE] = Vec3( w, h, 0 );
	h += 40;
	m_ImageLocations[MAINMENU_PAUSE_IMAGE] = Vec3( w2, h, 0 );
	m_ImageLocations[MAINMENU_PAUSE_ACTIVE_IMAGE] = Vec3( w2, h, 0 );
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

	w = ( width - 800 ) / 2; if( w<0 ) w=0;
	h = ( height - 700 ) / 2; if( h<0 ) h=0;
	m_ImageLocations[TIMES_IMAGE] = Vec3( w, h, 0 );

	w = width - 300; if( w<0 ) w=0;
	h = height - 100;  if( h<0 ) h=0;
	m_ImageLocations[MAINMENU_IMAGE] = Vec3( w, h, 0 );
	m_ImageLocations[MAINMENU_ACTIVE_IMAGE] = Vec3( w, h, 0 );
	m_ImageLocations[MAINMENU2_IMAGE] = Vec3( w, h, 0 );
	m_ImageLocations[MAINMENU2_ACTIVE_IMAGE] = Vec3( w, h, 0 );
	h += 40;
	m_ImageLocations[EXIT_SMALL_IMAGE] = Vec3( w, h, 0 );
	m_ImageLocations[EXIT_SMALL_ACTIVE_IMAGE] = Vec3( w, h, 0 );
	m_ImageLocations[EXITSMALL2_IMAGE] = Vec3( w, h, 0 );
	m_ImageLocations[EXITSMALL2_ACTIVE_IMAGE] = Vec3( w, h, 0 );

	//Position best times text
	w = ( width - 800 ) / 2; if( w<0 ) w=0;
	h = ( height - 700 ) / 2; if( h<0 ) h=0;
	
	//1.
	temp.left = w+475;  temp.right=temp.left+400; temp.top=h+210;  temp.bottom=temp.top+40;
	m_BestTimesRecs[0] = RECT(temp);
	//2.
	temp.left = w+475;  temp.right=temp.left+400; temp.top=h+272;  temp.bottom=temp.top+40;
	m_BestTimesRecs[1] = RECT(temp);
	//3.
	temp.left = w+475;  temp.right=temp.left+400; temp.top=h+335;  temp.bottom=temp.top+40;
	m_BestTimesRecs[2] = RECT(temp);
	//4.
	temp.left = w+475;  temp.right=temp.left+400; temp.top=h+395;  temp.bottom=temp.top+40;
	m_BestTimesRecs[3] = RECT(temp);
	//5.
	temp.left = w+475;  temp.right=temp.left+400; temp.top=h+460;  temp.bottom=temp.top+40;
	m_BestTimesRecs[4] = RECT(temp);

	//Position best names text
	
	//1.
	temp.left = w+250;  temp.right=temp.left+400; temp.top=h+210;  temp.bottom=temp.top+40;
	m_BestNamesRecs[0] = RECT(temp);
	//2.
	temp.left = w+250;  temp.right=temp.left+400; temp.top=h+272;  temp.bottom=temp.top+40;
	m_BestNamesRecs[1] = RECT(temp);
	//3.
	temp.left = w+250;  temp.right=temp.left+400; temp.top=h+335;  temp.bottom=temp.top+40;
	m_BestNamesRecs[2] = RECT(temp);
	//4.
	temp.left = w+250;  temp.right=temp.left+400; temp.top=h+395;  temp.bottom=temp.top+40;
	m_BestNamesRecs[3] = RECT(temp);
	//5.
	temp.left = w+250;  temp.right=temp.left+400; temp.top=h+460;  temp.bottom=temp.top+40;
	m_BestNamesRecs[4] = RECT(temp);

	//Position the finish time text
	temp.left = w+250;  temp.right=temp.left+400; temp.top=h+600;  temp.bottom=temp.top+40;
	m_finishTime = RECT(temp);
}


//--------------------------------------------------------------------------------------
// Function: createTexture
// Creates a sprite and stores it in the specified texture variable.
// INPUTS:	LPCSTR file		- filename of image to load
//			Device* device	- the IDirect3DDevice9 device to load images on
// OUTPUT:	Sprite& texture	- LPDIRECT3DTEXTURE9 object
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


//--------------------------------------------------------------------------------------
// Function: drawTransformedSprite( )
// Renders the supplied image by transforming a sprite with specified scaling and rotation
// factors.  If the game is in two player mode, the sprites are scaled 75% down.
// INPUTS:	float rot		- radian value of rotation around supplied "center"
//			Vec3& location	- location on screen to draw image (top left corner of image)
//			Vec2 center		- the center to rotate around
//			Sprite& image	- the LPDIRECT3DTEXTURE9 to display
//--------------------------------------------------------------------------------------
void Renderer::drawTransformedSprite( float rot, Vec3& location, Vec2 center, Sprite& image )
{
	HRESULT hr;
	Matrix mat;
	Vec2 trans = Vec2( location.x,  location.y );

	m_pImageSprite->Flush();
	D3DXMatrixTransformation2D(&mat, NULL, 0.0, &m_ScaleVal, &center, rot, &trans );
	m_pImageSprite->SetTransform( &mat );
	V( m_pImageSprite->Draw( image, NULL, NULL, NULL, D3DCOLOR_ARGB( 255,255,255,255 ) ) );
}


//--------------------------------------------------------------------------------------
// Function: drawAHUD
// This function draws one complete set of HUD for the specified player.  
// INPUTS:	Sprite* images	- the list of images to use (there are seperate ones for first
//							  and second players)
//			Vec3* locations - the list of locations for the images to use
//			int playerID	- either PLAYER1 or PLAYER2
//--------------------------------------------------------------------------------------
void Renderer::drawAHUD( Sprite* images, Vec3* locations, int playerID )
{
	HRESULT hr;
	Matrix mat;
	Matrix rotateZ;
	int image = 0;

	//draw the speedometer 
	drawTransformedSprite( 0.0, locations[SPEEDOMETER_IMAGE], Vec2(0,0), images[ SPEEDOMETER_IMAGE ] );
	
	//rotate the speedometer wand image to correct position
	if( m_bIsTwoPlayer )
		drawTransformedSprite( m_fSpeedRotations[ playerID ], locations[SPEEDWAND_IMAGE], Vec2(64,11), images[ SPEEDWAND_IMAGE ] );
	else
		drawTransformedSprite( m_fSpeedRotations[ playerID ], locations[SPEEDWAND_IMAGE], Vec2(84,15), images[ SPEEDWAND_IMAGE ] );
	
	//draw the minimap
	//drawTransformedSprite( 0.0, locations[MINIMAP_IMAGE], Vec2(0,0), images[ MINIMAP_IMAGE ] );

	//draw the ranking if it's not time trial
	if (!m_bIsTimeTrial) {
		int iRank = playerID == PLAYER1 ? m_iPlayerOneRank : m_iPlayerTwoRank;
		drawTransformedSprite( 0.0, locations[ iRank ], Vec2(0,0), images[ iRank ]);
	}

	//draw wrong way message if applicable
	if( m_bDrawWrongWay[ playerID ] )
		drawTransformedSprite( 0.0, locations[ WRONGWAY_IMAGE ], Vec2(0,0), images[ WRONGWAY_IMAGE ]);

	// draw lap counter
	image = m_iLapImages[ playerID ];
	drawTransformedSprite( 0.0, locations[ image ], Vec2(0,0), images[ image ]);

	int timeOffset = 8 * playerID;
	//draw the clock
	for( int i=timeOffset; i < 8+timeOffset; i++ )
	{
		image = m_iTimeImages[ i ];
		drawTransformedSprite( 0.0, m_TimeLocations[i], Vec2(0,0), images[ image ]);
	}

	//reset sprite transform matrix so all sprites aren't rotated
	D3DXMatrixIdentity( &mat );
	m_pImageSprite->SetTransform( &mat );
}


//--------------------------------------------------------------------------------------
// Function: positionHUDImages
// This function creates the position vectors for all of the HUD images.  
// INPUTS:	int width		- width of the screen to draw to
//			int height		- height of screen to draw to
//--------------------------------------------------------------------------------------
void Renderer::positionHUDImages( int width, int height )
{
	int w, h;
	int Height = height/2;

	if( m_bIsTwoPlayer )
	{
		m_ScaleVal = Vec2( 0.75, 0.75 );

		//Position HUD images
		m_HUDImageLocations[SPEEDOMETER_IMAGE] = Vec3( 10, Height-140, 0 );
		m_HUDImageLocations[SPEEDWAND_IMAGE] = Vec3( 40, Height-60, 0 );
		m_HUDImageLocations[MINIMAP_IMAGE] = Vec3( width-195, Height-140, 0 );
		m_HUDImageLocations[FIRST_IMAGE] = Vec3( width-120, 10, 0 );
		m_HUDImageLocations[SECOND_IMAGE] = Vec3( width-120, 10, 0 );
		m_HUDImageLocations[THIRD_IMAGE] = Vec3( width-120, 10, 0 );
		m_HUDImageLocations[FOURTH_IMAGE] = Vec3( width-120, 10, 0 );
		m_HUDImageLocations[FIFTH_IMAGE] = Vec3( width-120, 10, 0 );
		m_HUDImageLocations[SIXTH_IMAGE] = Vec3( width-120, 10, 0 );
		m_HUDImageLocations[SEVENTH_IMAGE] = Vec3( width-120, 10, 0 );
		m_HUDImageLocations[EIGTH_IMAGE] = Vec3( width-120, 10, 0 );
		m_HUDImageLocations[WRONGWAY_IMAGE] = Vec3( (width-300)/2, 10, 0 );
		m_HUDImageLocations[LAPONE_IMAGE] = Vec3( 10, 10, 0 );
		m_HUDImageLocations[LAPTWO_IMAGE] = Vec3( 10, 10, 0 );

		//Position HUD images
		m_HUDImageLocations2[SPEEDOMETER_IMAGE] = Vec3( 10, height-140, 0 );
		m_HUDImageLocations2[SPEEDWAND_IMAGE] = Vec3( 40, height-60, 0 );
		m_HUDImageLocations2[MINIMAP_IMAGE] = Vec3( width-195, height-140, 0 );
		m_HUDImageLocations2[FIRST_IMAGE] = Vec3( width-120, Height+10, 0 );
		m_HUDImageLocations2[SECOND_IMAGE] = Vec3( width-120, Height+10, 0 );
		m_HUDImageLocations2[THIRD_IMAGE] = Vec3( width-120, Height+10, 0 );
		m_HUDImageLocations2[FOURTH_IMAGE] = Vec3( width-120, Height+10, 0 );
		m_HUDImageLocations2[FIFTH_IMAGE] = Vec3( width-120, Height+10, 0 );
		m_HUDImageLocations2[SIXTH_IMAGE] = Vec3( width-120, Height+10, 0 );
		m_HUDImageLocations2[SEVENTH_IMAGE] = Vec3( width-120, Height+10, 0 );
		m_HUDImageLocations2[EIGTH_IMAGE] = Vec3( width-120, Height+10, 0 );
		m_HUDImageLocations2[WRONGWAY_IMAGE] = Vec3( (width-300)/2, Height+10, 0 );
		m_HUDImageLocations2[LAPONE_IMAGE] = Vec3( 10, Height+10, 0 );
		m_HUDImageLocations2[LAPTWO_IMAGE] = Vec3( 10, Height+10, 0 );

		//Position number images for player1
		w = ( width - 120 ); if( w<0 ) w=0;
		for( int i=0; i < 8; i++ )
		{
			m_TimeLocations[i] = Vec3( w, 64, 0 );
			w += 13;
		}

		//Position number images for player2
		w = ( width - 120 ); if( w<0 ) w=0;
		for( int i=8; i < 16; i++ )
		{
			m_TimeLocations[i] = Vec3( w, Height+64, 0 );
			w += 13;
		}
	}
	else
	{
		m_ScaleVal = Vec2( 1.0, 1.0 );

		//Position HUD images
		m_HUDImageLocations[SPEEDOMETER_IMAGE] = Vec3( 10, height-180, 0 );
		m_HUDImageLocations[SPEEDWAND_IMAGE] = Vec3( 50, height-70, 0 );
		m_HUDImageLocations[MINIMAP_IMAGE] = Vec3( width-260, height-180, 0 );
		m_HUDImageLocations[FIRST_IMAGE] = Vec3( width-160, 10, 0 );
		m_HUDImageLocations[SECOND_IMAGE] = Vec3( width-160, 10, 0 );
		m_HUDImageLocations[THIRD_IMAGE] = Vec3( width-160, 10, 0 );
		m_HUDImageLocations[FOURTH_IMAGE] = Vec3( width-160, 10, 0 );
		m_HUDImageLocations[FIFTH_IMAGE] = Vec3( width-160, 10, 0 );
		m_HUDImageLocations[SIXTH_IMAGE] = Vec3( width-160, 10, 0 );
		m_HUDImageLocations[SEVENTH_IMAGE] = Vec3( width-160, 10, 0 );
		m_HUDImageLocations[EIGTH_IMAGE] = Vec3( width-160, 10, 0 );
		m_HUDImageLocations[WRONGWAY_IMAGE] = Vec3( (width-400)/2, 10, 0 );
		m_HUDImageLocations[LAPONE_IMAGE] = Vec3( 10, 10, 0 );
		m_HUDImageLocations[LAPTWO_IMAGE] = Vec3( 10, 10, 0 );

		//Position number images
		w = ( width - 160 ); if( w<0 ) w=0;
		for( int i=0; i < 8; i++ )
		{
			m_TimeLocations[i] = Vec3( w, 85, 0 );
			w += 18;
		}
	}
}


