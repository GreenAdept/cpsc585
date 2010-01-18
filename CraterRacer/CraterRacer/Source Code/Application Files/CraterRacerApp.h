#pragma once
#ifndef CRATER_RACER_APP_H
#define CRATER_RACER_APP_H

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include "DXUT.h"
#include "resource.h"
#include "GameObj.h"
#include "SDKmesh.h"

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

	LRESULT OnDeviceChange			 ( int eventType);
	LRESULT OnDisplayChange			 ( int colorDepth, int width, int height );
	LRESULT OnClose					 ( );


	// DIRECTX Functions ---------------------------------------

	static HRESULT  CALLBACK OnResetDevice		 ( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void *pUserContext  );
	static void     CALLBACK OnLostDevice		 ( void *pUserContext );
	static void		CALLBACK OnUpdateGame		 ( double fTime, float fElapsedTime, void *pUserContext );
	static void		CALLBACK OnRender			 ( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void *pUserContext );
	static HRESULT	CALLBACK OnCreateDevice		 ( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
	static void		CALLBACK OnDestroyDevice	 ( void* pUserContext );
	int				GetExitCode					 ( ) { return DXUTGetExitCode(); }


	// GAME Stuff ----------------------------------------------

	
};

extern GameObj *m_pGame;

#endif CRATER_RACER_APP_H