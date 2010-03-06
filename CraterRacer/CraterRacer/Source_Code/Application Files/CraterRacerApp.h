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
#define BG_WAVEBANK_FILE		L"Media\\Audio\\Win\\BGMusicWaveBank.xwb"
#define BG_SOUNDBANK_FILE		L"Media\\Audio\\Win\\BGMusicSoundBank.xsb"

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


	// GUI  -------------------------------------------------

	static void	processMenuSelection ( );
	static void moveMenuUp			 ( );
	static void moveMenuDown		 ( );
	static void renderFPS			 ( );
	static void renderClock			 ( );

	static ResourceManager		m_ResourceManager;	// manager for shared resources of dialogs
	static ApplicationState		m_AppState;
	static vector<CDXUTButton*> buttons; 
	static UINT					m_uiCurrentButton;

	// dialog for standard game controls
	static Dialog				m_MenuScreen;		// dialog for sample specific controls
	static Dialog				m_OnePlayerScreen;	// game view
	static Dialog				m_PauseScreen;		//

	static XBox360Controller*	m_MenuController;
	static SceneLoader*			m_SceneLoader;		// used to load the game
	static ID3DXFont*           m_pFont;
	static ID3DXSprite*         m_pTextSprite;

};

extern GameObj *g_pGame; //Our global instantiation of the game which will be used by the RacerApp class

#endif CRATER_RACER_APP_H