#pragma once
#ifndef CRATER_RACER_APP_H
#define CRATER_RACER_APP_H

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include "Resource.h"
#include "GameObj.h"
#include "XBox360Controller.h"
#include "Constants.h"
#include "Sound.h"

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

	static ResourceManager		m_ResourceManager;	// manager for shared resources of dialogs
	static ApplicationState		m_AppState;
	static vector<CDXUTButton*> buttons; 
	static UINT					m_uiCurrentButton;

	// dialog for standard game controls
	static Dialog				m_MenuScreen;		// dialog for sample specific controls
	static Dialog				m_OnePlayerScreen;	// game view
	static Dialog				m_PauseScreen;		//

	static XBox360Controller*	m_MenuController;
};

extern GameObj *g_pGame; //Our global instantiation of the game which will be used by the RacerApp class

#endif CRATER_RACER_APP_H