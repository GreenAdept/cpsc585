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
	void adjustRankImage	( int rank, int playerID );
	void adjustClockImages	( string time, int playerID );
	void adjustSpeedImage	( float speed, int playerID );
	void adjustVictoryRank	( vector<int>& ranks, vector<string>& times );
	void adjustWrongWay		( int playerNum, bool drawWrongWay );
	void adjustLapImage		( int playerNum, int lapNum );
	void adjustBestTimes	( vector<string>& bestTimeEntries );
	void adjustTwoPlayer	( bool isTwoPlayer, int width, int height );
	void adjustTimeTrial	( );

	// Drawing functions
	void renderGame			( Device* device, vector<Renderable*>, vector<GameCamera*> cameras, int playerID );
	void drawHUD			( int playerNum );
	void drawPauseScreen	( );
	void drawStartupMenu	( );
	void drawLoadingScreen	( );
	void drawPauseGameRules ( );
	void drawGameRules		( );
	void renderFPS			( );
	void renderCountDown	( int count );
	void drawVictoryScreen	( );
	void drawTimesScreen	( );
	void drawCountdown		( int num );

private:

	// Utility functions
	void loadImages				( Device* device, UINT width, UINT height );
	void createTexture			( Sprite& texture, LPCSTR file, Device* device );
	void drawAHUD				( Sprite* images, Vec3* locations, int playerID );
	void drawTransformedSprite	( float rot, Vec3& location, Vec2 center, Sprite& image );
	void positionHUDImages		( int width, int height );
	void positionMainImages		( int width, int height );


	//Date Members ---------------------------------------

	ID3DXFont*          m_pFont;									//font used to render FPS
	ID3DXFont*          m_pFontVictoryBig;							//
	ID3DXFont*          m_pFontVictorySmall;					
	ID3DXSprite*        m_pTextSprite;								//text sprite used to render FPS
	ID3DXSprite*		m_pImageSprite;								//used in rendering all images
	RECT				m_PauseRECT;								//the pause menu rectangle
	Sprite				m_Images			[ NUM_IMAGES ];			//list of all images used in the game
	Vec3				m_ImageLocations	[ NUM_IMAGES ];			//list of all image locations used in the game

	Sprite				m_HUDImages			[ NUM_HUD_IMAGES ];		//list of all images used in the game
	Vec3				m_HUDImageLocations	[ NUM_HUD_IMAGES ];		//list of all image locations used in the game
	Sprite				m_HUDImages2		[ NUM_HUD_IMAGES ];		//list of all images used in the game for player2
	Vec3				m_HUDImageLocations2[ NUM_HUD_IMAGES ];		//list of all image locations used in the game for player2
	int					m_iTimeImages		[ 16 ];					//clock image indexes( index into m_Images )
	Vec3				m_TimeLocations		[ 16 ];					//clock image locations
	int					m_iLapImages		[ 2 ];
	bool				m_bDrawWrongWay		[ 2 ];
	float				m_fSpeedRotations	[ 2 ];					//degree in radians to rotate speedometer wand image
	//Sprite			m_Skybox;

	Vec3				m_BallLocations		[ NUM_LOADING_BALLS ];	//ball image locations
	int					m_iButtonImages		[ NUM_BUTTONS ];		//current button images (index into m_Images)
	int					m_iBallImages		[ NUM_LOADING_BALLS ];	//current ball images (index into m_Images)
	int					m_iPlayerOneRank;							//current image index of player rank
	int					m_iPlayerOneImage;	
	int					m_iPlayerTwoRank;
	int					m_iPlayerTwoImage;							//starting index for player 2 ranking images
	int					m_iClockImage;								//starting image index for zero
	Dialog				m_GameScreen;								//HUD and pause screen are rendered to this
	ResourceManager		m_ResourceManager;						
	wstring				m_sVictoryRanks		[ NUM_PLAYERS ];
	wstring				m_sVictoryTimes		[ NUM_PLAYERS ];
	wstring				m_sTest				[5];
	RECT				m_VictoryRecs		[ NUM_PLAYERS*2 ];
	LPCWSTR				m_sBestTimes		[ 5 ];
	//wstring				m_sBestTimes		[ 5 ];
	RECT				m_BestTimesRecs		[ 5 ];
	bool				m_bIsTwoPlayer;
	bool				m_bIsTimeTrial;
	Vec2				m_ScaleVal;
	Vec2				m_CountdownLocation;
};


#endif