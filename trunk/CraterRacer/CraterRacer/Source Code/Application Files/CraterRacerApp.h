#pragma once
#ifndef CRATER_RACER_APP_H
#define CRATER_RACER_APP_H

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include "resource.h"
#include "GameObj.h"

#include "..\Utility Files\Constants.h"

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

	static	LRESULT CALLBACK MsgProc ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void *pUserContext );
    static  void CALLBACK OnKeyboard ( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
	void	CALLBACK OnGUIEvent		 ( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );


	// DIRECTX Functions ---------------------------------------

	static HRESULT  CALLBACK OnResetDevice		 ( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void *pUserContext  );
	static void     CALLBACK OnLostDevice		 ( void *pUserContext );
	static void		CALLBACK OnUpdateGame		 ( double fTime, float fElapsedTime, void *pUserContext );
	static void		CALLBACK OnRender			 ( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void *pUserContext );
	static HRESULT	CALLBACK OnCreateDevice		 ( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
	static void		CALLBACK OnDestroyDevice	 ( void* pUserContext );
	static bool		CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );
	static bool		CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9*, D3DFORMAT, D3DFORMAT, bool, void* );
	int				GetExitCode					 ( ) { return DXUTGetExitCode(); }


	// GAME Stuff ----------------------------------------------


	// GUI Data Members ----------------------------------------

	//CDXUTDialogResourceManager*	 m_dResourceManager;	// manager for shared resources of dialogs
	//CDXUTTextHelper*				m_pTxtHelper;
	//Dialog						m_dHUD;                 // dialog for standard game controls
	//Dialog						m_dMenu;				// dialog for sample specific controls

};

extern GameObj *m_pGame;

#endif CRATER_RACER_APP_H