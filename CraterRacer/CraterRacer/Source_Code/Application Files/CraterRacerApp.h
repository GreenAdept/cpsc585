#pragma once
#ifndef CRATER_RACER_APP_H
#define CRATER_RACER_APP_H

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include "Resource.h"
#include "Constants.h"
#include "SceneLoader.h"
#include <string>

#define ONE_PLAYER_SCENE_FILE	"Source_Code\\Scene_Files\\OnePlayer.SCENE"
#define TWO_PLAYER_SCENE_FILE	"Source_Code\\Scene_Files\\TwoPlayer.SCENE"
#define TIME_TRIAL_SCENE_FILE	"Source_Code\\Scene_Files\\TimeTrial.SCENE"
#define INIT_SCENE_FILE			"Source_Code\\Scene_Files\\Initialize.SCENE"

//Image constants
#define MENU_IMAGE_FILE					"Media\\Images\\Logo.png"
#define SPEEDOMETER_IMAGE_FILE			"Media\\Images\\Speedometer.png"
#define ONEPLAYER_NOTACTIVE_IMAGE_FILE	"Media\\Images\\OnePlayerNotActive.png"
#define ONEPLAYER_ACTIVE_IMAGE_FILE		"Media\\Images\\OnePlayerActive.png"
#define EXIT_NOTACTIVE_IMAGE_FILE		"Media\\Images\\ExitNotActive.png"
#define EXIT_ACTIVE_IMAGE_FILE			"Media\\Images\\ExitActive.png"
#define LOADING_IMAGE_FILE				"Media\\Images\\Loading.png"
#define LOADING_SMALLBALL_IMAGE_FILE	"Media\\Images\\LoadingSmallBall.png"
#define LOADING_BIGBALL_IMAGE_FILE		"Media\\Images\\LoadingBigBall.png"
#define UNPAUSE_IMAGE_FILE				"Media\\Images\\Unpause.png"
#define UNPAUSE_ACTIVE_IMAGE_FILE		"Media\\Images\\UnpauseActive.png"
#define GAMERULES_IMAGE_FILE			"Media\\Images\\GameRules.png"
#define GAMERULES_ACTIVE_IMAGE_FILE		"Media\\Images\\GameRulesActive.png"
#define TWOPLAYER_IMAGE_FILE			"Media\\Images\\TwoPlayer.png"
#define TWOPLAYER_ACTIVE_IMAGE_FILE		"Media\\Images\\TwoPlayerActive.png"
#define TIMETRIAL_IMAGE_FILE			"Media\\Images\\TimeTrial.png"
#define TIMETRIAL_ACTIVE_IMAGE_FILE		"Media\\Images\\TimeTrialActive.png"
#define GAMERULES_INFO_IMAGE_FILE		"Media\\Images\\GameRulesInfo.png"
#define CONTROLS_IMAGE_FILE				"Media\\Images\\Controls.png"
#define GAMERULES_INFO_SMALL_IMAGE_FILE	"Media\\Images\\GameRulesInfoSmall.png"

#define INIT_LOAD				0
#define GAME_LOAD				1
#define NUM_IMAGES				24
#define NUM_BUTTONS				8
#define NUM_LOADING_BALLS		5

#define BG_WAVEBANK_FILE		L"Media\\Audio\\Win\\BGMusicWaveBank.xwb"
#define SE_WAVEBANK_FILE		L"Media\\Audio\\Win\\SoundEffectWaveBank.xwb"
#define BG_SOUNDBANK_FILE		L"Media\\Audio\\Win\\MainSoundBank.xsb"
#define BG_SETTINGS_FILE		L"Media\\Audio\\Win\\BGMusic.xgs"


enum GuiControls{ GUI_BTN_SINGLE_PLAYER, GUI_BTN_TWO_PLAYER, GUI_BTN_TIMETRIAL,
				  GUI_BTN_GAMERULES, GUI_BTN_EXIT, 
				  GUI_BTN_UNPAUSE, GUI_BTN_GAMERULES2, GUI_BTN_EXIT2 };

enum ImageNames{ MENU_IMAGE, CONTROLS_IMAGE, GAMERULES_INFO_IMAGE, 
				 LOADING_IMAGE, LOADING_SMALLBALL_IMAGE, LOADING_BIGBALL_IMAGE,
				 GAMERULES_INFO_SMALL_IMAGE, SPEEDOMETER_IMAGE,

				 ONEPLAYER_NOTACTIVE_IMAGE, ONEPLAYER_ACTIVE_IMAGE, 
				 TWOPLAYER_IMAGE, TWOPLAYER_ACTIVE_IMAGE,
				 TIMETRIAL_IMAGE, TIMETRIAL_ACTIVE_IMAGE,
				 GAMERULES_IMAGE, GAMERULES_ACTIVE_IMAGE,
				 EXIT_NOTACTIVE_IMAGE, EXIT_ACTIVE_IMAGE, 

				 UNPAUSE_IMAGE, UNPAUSE_ACTIVE_IMAGE,
				 GAMERULES2_IMAGE, GAMERULES2_ACTIVE_IMAGE,
				 EXIT2_NOTACTIVE_IMAGE, EXIT2_ACTIVE_IMAGE };


enum ApplicationState { APP_STARTUP, APP_RENDER_GAME, APP_PAUSED, 
						APP_GAME_LOADING, APP_SHOW_GAMERULES, APP_SHOW_GAMERULES2 };

//--------------------------------------------------------
//		CLASS: RacerApp
//--------------------------------------------------------

class RacerApp
{
public:

	//Public Interface----------------------------------------

	RacerApp();
	~RacerApp();


	// WIN32 Functions ----------------------------------------

    static  void	CALLBACK OnKeyboard				( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
	

	// DXUT Functions ---------------------------------------

	static HRESULT  CALLBACK OnResetDevice			( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void *pUserContext  );
	static void     CALLBACK OnLostDevice			( void *pUserContext );
	static void		CALLBACK OnUpdateGame			( double fTime, float fElapsedTime, void *pUserContext );
	static void		CALLBACK OnRender				( Device* device, double dTime, float fElapsedTime, void *pUserContext );
	static HRESULT	CALLBACK OnCreateDevice			( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
	static void		CALLBACK OnDestroyDevice		( void* pUserContext );
	static bool		CALLBACK ModifyDeviceSettings	( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );
	static bool		CALLBACK IsD3D9DeviceAcceptable	( D3DCAPS9*, D3DFORMAT, D3DFORMAT, bool, void* );


	// Thread Functions -------------------------------------

	static long WINAPI startGame ( long lParam );
	static long WINAPI initGame	 ( long lParam );


	// GUI  -------------------------------------------------

	static void	processMenuSelection ( );
	static void moveMenuUp			 ( );
	static void moveMenuDown		 ( );
	static void renderFPS			 ( );
	static void renderClock			 ( );
	static void doLoadScreen		 ( );
	static void drawHUD				 ( );


	// Other Functions ---------------------------------------

	static void cleanupAll			( );
	static void animateBall			( );
	static void loadImages			( Device* device, UINT width, UINT height );
	static void createTexture		( Sprite& texture, LPCSTR file, Device* device );
	static void drawPauseScreen		( );
	static void drawStartupMenu		( );
	static void drawLoadingScreen	( );


	// Data Members ------------------------------------------

	static ResourceManager		m_ResourceManager;	// manager for shared resources of dialogs
	static ApplicationState		m_AppState;
	static UINT					m_uiCurrentButton;
	static ApplicationState		m_iPreviousAppState;

	static Dialog				m_GameScreen;	// game view

	static XBox360Controller*	m_MenuController;
	static SceneLoader*			m_SceneLoader;		// used to load the game
	static ID3DXFont*           m_pFont;
	static ID3DXSprite*         m_pTextSprite;

	// buttons
	static ID3DXSprite*			m_pImageSprite;
	static RECT					m_PauseRECT;
	static Sprite				m_Images[ NUM_IMAGES ];
	static Vec3					m_ImageLocations[ NUM_IMAGES ];
	static int					m_iButtonImage[ NUM_BUTTONS ];
	static float				m_fCheckTime;
	
	// ball animation on loading screen
	static int					m_iBallImages[ NUM_LOADING_BALLS ];
	static Vec3					m_BallLocations[ NUM_LOADING_BALLS ];
	static int					m_iCurrentBall;
	static UINT_PTR				m_AnimationID;

	// thread stuff and loading
	static HANDLE				m_hThread;
	static DWORD				m_dwThreadID;
	static bool					m_bGameIsReady;
	static CRITICAL_SECTION		m_CriticalSection; // used for multithreading
	
};

extern GameObj *g_pGame; //Our global instantiation of the game which will be used by the RacerApp class

#endif CRATER_RACER_APP_H