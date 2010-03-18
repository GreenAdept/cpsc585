#pragma once
#ifndef H_RENDERER
#define H_RENDERER

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include <vector>
#include <string>
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
	void adjustBallImage	( int ballIndex, int adjust );
	void adjustButtonImage	( int buttonIndex, int adjust );
	void adjustRankImage	( int rank );
	void adjustClockImages	( string time );
	void adjustSpeedImage	( float speed );
	void adjustVictoryRank	( vector<int>& ranks );
	void adjustWrongWay		( int playerNum, bool drawWrongWay );
	void adjustLapImage		( int playerNum, int lapNum );


	// Drawing functions
	void renderGame			( Device* device, vector<Renderable*>, vector<GameCamera*> cameras );
	void drawHUD			( );
	void drawPauseScreen	( );
	void drawStartupMenu	( );
	void drawLoadingScreen	( );
	void drawPauseGameRules ( );
	void drawGameRules		( );
	void renderFPS			( );
	void drawVictoryScreen	( );

private:

	// Utility functions
	void loadImages			( Device* device, UINT width, UINT height );
	void createTexture		( Sprite& texture, LPCSTR file, Device* device );

	//Date Members ---------------------------------------

	ID3DXFont*          m_pFont;								//font used to render FPS
	ID3DXFont*          m_pFontVictoryBig;							//
	ID3DXFont*          m_pFontVictorySmall;					
	ID3DXSprite*        m_pTextSprite;							//text sprite used to render FPS
	ID3DXSprite*		m_pImageSprite;							//used in rendering all images
	RECT				m_PauseRECT;							//the pause menu rectangle
	Sprite				m_Images		[ NUM_IMAGES ];			//list of all images used in the game
	Vec3				m_ImageLocations[ NUM_IMAGES ];			//list of all image locations used in the game
	Vec3				m_BallLocations	[ NUM_LOADING_BALLS ];	//ball image locations
	Vec3				m_TimeLocations	[ 8 ];					//clock image locations
	int					m_iButtonImages	[ NUM_BUTTONS ];		//current button images (index into m_Images)
	int					m_iBallImages	[ NUM_LOADING_BALLS ];	//current ball images (index into m_Images)
	int					m_iPlayerOneRank;						//current image index of player rank
	int					m_iPlayerOneImage;						//starting index for player ranking images
	int					m_iClockImage;							//starting image index for zero
	int					m_iTimeImages	[ 8 ];					//clock image indexes( index into m_Images )
	Dialog				m_GameScreen;							//HUD and pause screen are rendered to this
	ResourceManager		m_ResourceManager;						
	float				m_fSpeedRotation;						//degree in radians to rotate speedometer wand image
	LPCWSTR				m_sVictoryRanks[ NUM_PLAYERS ];
	LPCWSTR				m_sVictoryTimes[ NUM_PLAYERS ];
	RECT				m_VictoryRecs[ NUM_PLAYERS*2 ];
	bool				m_bDrawWrongWay[ NUM_PLAYERS ];
	int					m_iLapImages[ NUM_PLAYERS ];
};

#endif