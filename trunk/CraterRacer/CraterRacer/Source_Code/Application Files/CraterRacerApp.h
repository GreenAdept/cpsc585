#pragma once
#ifndef CRATER_RACER_APP_H
#define CRATER_RACER_APP_H

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include "Resource.h"
#include "Constants.h"
#include "ImageConstants.h"
#include "SceneLoader.h"
#include <string>

#define ONE_PLAYER_SCENE_FILE	"Source_Code\\Scene_Files\\OnePlayer.SCENE"
#define TWO_PLAYER_SCENE_FILE	"Source_Code\\Scene_Files\\TwoPlayer.SCENE"
#define TIME_TRIAL_SCENE_FILE	"Source_Code\\Scene_Files\\TimeTrial.SCENE"
#define INIT_SCENE_FILE			"Source_Code\\Scene_Files\\Initialize.SCENE"

#define INIT_LOAD				0
#define GAME_LOAD				1

#define BG_WAVEBANK_FILE		L"Media\\Audio\\Win\\BGMusicWaveBank.xwb"
#define SE_WAVEBANK_FILE		L"Media\\Audio\\Win\\SoundEffectWaveBank.xwb"
#define BG_SOUNDBANK_FILE		L"Media\\Audio\\Win\\MainSoundBank.xsb"
#define BG_SETTINGS_FILE		L"Media\\Audio\\Win\\BGMusic.xgs"

enum ApplicationState { APP_STARTUP, APP_RENDER_GAME, APP_PAUSED, 
						APP_GAME_LOADING, APP_SHOW_GAMERULES, APP_SHOW_GAMERULES2,
						APP_VICTORY };

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
	static void doLoadScreen		 ( );

	// Other Functions ---------------------------------------

	static void cleanupAll			( );
	static void animateBall			( );


	// Data Members ------------------------------------------
	static ApplicationState		m_AppState;
	static UINT					m_uiCurrentButton;
	static ApplicationState		m_iPreviousAppState;
	static Renderer*			m_Renderer;			// rendering subsystem
	static XBox360Controller*	m_MenuController;
	static SceneLoader*			m_SceneLoader;		// used to load the game
	static float				m_fCheckTime;
	
	// ball animation on loading screen
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