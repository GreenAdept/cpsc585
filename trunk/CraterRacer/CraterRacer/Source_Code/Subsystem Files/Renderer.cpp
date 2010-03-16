/*
* The Rendering subsystem handles all drawing to the screen for our game.  This includes all the 
* game menus, the gameplay and HUD.
*/

#include "Renderer.h"

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

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	device->SetRenderState(D3DRS_ALPHAREF, (DWORD)100);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE); 

	// Create a sprite to help batch calls when drawing many lines of text
	D3DXCreateSprite( device, &m_pTextSprite );
	D3DXCreateSprite( device, &m_pImageSprite );
	
	if( m_pFont )
       V_RETURN( m_pFont->OnResetDevice() );

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

	//set the size and font of text to be used in "RenderText" function
    V_RETURN( D3DXCreateFont( device, 24, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
                          OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                          L"Arial", &m_pFont ));
}


//--------------------------------------------------------------------------------------
// Function: OnLost
// Calls OnLostDevice on any fonts used by the renderer and releases sprites.
//--------------------------------------------------------------------------------------
void Renderer::OnLost( )
{
	if( m_pFont )
        m_pFont->OnLostDevice();

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
	SAFE_RELEASE( m_pFont );
}


//////////////////////////////////////////////////////////////////////////////////////////////		
// 
//	Public Drawing Functions
//		
//////////////////////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------
// Function: renderClock
// Renders the game time in the right hand corner of the screen
//--------------------------------------------------------------------------------------
void Renderer::renderClock( string time )
{
	CDXUTTextHelper txtHelper( m_pFont, m_pTextSprite, 15 );

    txtHelper.Begin();
    txtHelper.SetInsertionPos( 10, 35 );
	txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );

	wstring wTime(time.length(), L' ');
	copy(time.begin(), time.end(), wTime.begin());
    txtHelper.DrawTextLine( wTime.c_str() );
		
	txtHelper.End();
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
		image = m_iButtonImage[ i ];
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
	Vec3 center( 0, 0, 0 );
	int image = 0;

	//render the HUD
	m_pImageSprite->Begin( NULL );
	
	//draw the speedometer background
	m_pImageSprite->Flush();
	V( m_pImageSprite->Draw( m_Images[ SPEEDOMETER_IMAGE ], NULL, &center, &m_ImageLocations[ SPEEDOMETER_IMAGE ], D3DCOLOR_ARGB( 255,255,255,255 ) ) );

	m_pImageSprite->Flush();
	V( m_pImageSprite->Draw( m_Images[ MINIMAP_IMAGE ], NULL, &center, &m_ImageLocations[ MINIMAP_IMAGE ], D3DCOLOR_ARGB( 255,255,255,255 ) ) );

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

	drawHUD();
}


//--------------------------------------------------------------------------------------
// Function: drawGameRules
// This function draws a full-sized game rules screen (so it will fill up the entire
// screen).
//--------------------------------------------------------------------------------------
void Renderer::drawGameRules( )
{
	HRESULT hr;
	Vec3 center( 0, 0, 0 );

	//Draw the game rules image (as entire screen)
	m_pImageSprite->Begin( NULL );
	m_pImageSprite->Flush();
	V( m_pImageSprite->Draw( m_Images[GAMERULES_INFO_IMAGE], NULL, &center, 
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
// Function:  renderFPS
// This function renders the frame per second as text in the upper left hand corner.
//--------------------------------------------------------------------------------------
void Renderer::renderFPS( )
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
// Function: adjustButton
// This function increments or decrements the specified button image index.   
// Each button has two images, one for being selected and one for being inactive. The
// caller of this function should be aware which buttons are active and inactive and
// call this function accordingly, because calling a decrement on an inactive button 
// will cause it to display the active image of another button.  
// INPUTS:  int buttonIndex	- button number to adjust image of
//			int adjust		- use "+1" to make active, and "-1" to make inactive.
//--------------------------------------------------------------------------------------
void Renderer::adjustButton( int buttonIndex, int adjust )
{
	m_iButtonImage[ buttonIndex ] += adjust;
}


//--------------------------------------------------------------------------------------
// Function: adjustBall
// This function increments or decrements the specified ball image index.   
// There are two ball images, one small and one big.  The order is small then big.
// INPUTS:  int bballIndex	- ball number to adjust image of
//			int adjust		- use "+1" to make ball bigger, and "-1" to make ball smaller.
//--------------------------------------------------------------------------------------
void Renderer::adjustBall( int ballIndex, int adjust )
{
	m_iBallImages[ ballIndex ] += adjust;
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
	createTexture( m_Images[ SPEEDOMETER_IMAGE ], SPEEDOMETER_IMAGE_FILE, device );
	createTexture( m_Images[ MINIMAP_IMAGE ], MINIMAP_IMAGE_FILE, device );

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
	m_ImageLocations[SPEEDOMETER_IMAGE] = Vec3( 10, height-180, 0 );
	m_ImageLocations[MINIMAP_IMAGE] = Vec3( width-260, height-180, 0 );

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
