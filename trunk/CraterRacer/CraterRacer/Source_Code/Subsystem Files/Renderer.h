#pragma once
#ifndef H_RENDERER
#define H_RENDERER

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include <vector>
#include "Renderable.h"
#include "GameCamera.h"
#include "Constants.h"
#include "ImageConstants.h"


//--------------------------------------------------------
//		CLASS: Renderer
//
//		This rendering subsystem handles all drawing to
//		the screen for our game.  This includes all the 
//		game menus, the gameplay and HUD.
//--------------------------------------------------------

class Renderer 
{

public:

	//Public Interface -----------------------------------

	Renderer ( );
	~Renderer( );

	// Device-related functions
	HRESULT OnReset			( Device* device, const D3DSURFACE_DESC* pBack );
	HRESULT OnCreate		( Device* device );
	void	OnLost			( );
	void	OnDestroy		( );

	// Utility functions
	void adjustBall			( int ballIndex, int adjust );
	void adjustButton		( int buttonIndex, int adjust );

	// Drawing functions
	void renderGame			( Device* device, vector<Renderable*>, vector<GameCamera*> cameras );
	void drawHUD			( );
	void drawPauseScreen	( );
	void drawStartupMenu	( );
	void drawLoadingScreen	( );
	void drawPauseGameRules ( );
	void drawGameRules		( );
	void renderClock		( string time );
	void renderFPS			( );


private:

	// Utility functions
	void loadImages			( Device* device, UINT width, UINT height );
	void createTexture		( Sprite& texture, LPCSTR file, Device* device );

	//Date Members ---------------------------------------

	ID3DXFont*          m_pFont;								//font used to render FPS
	ID3DXSprite*        m_pTextSprite;							//text sprite used to render FPS
	ID3DXSprite*		m_pImageSprite;							//used in rendering all images
	RECT				m_PauseRECT;							//the pause menu rectangle
	Sprite				m_Images		[ NUM_IMAGES ];			//list of all images used in the game
	Vec3				m_ImageLocations[ NUM_IMAGES ];			//list of all image locations used in the game
	Vec3				m_BallLocations	[ NUM_LOADING_BALLS ];	//ball image locations
	int					m_iButtonImage	[ NUM_BUTTONS ];		//current button images (index into m_Images)
	int					m_iBallImages	[ NUM_LOADING_BALLS ];	//current ball images (index into m_Images)
	Dialog				m_GameScreen;							//HUD and pause screen are rendered to this
};

#endif