//--------------------------------------------------------------------------------------
// File: CraterRacerApp.cpp
//
//--------------------------------------------------------------------------------------
#include "CraterRacerApp.h"

GameObj* m_pGame = new GameObj();

//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
RacerApp::RacerApp()
{
}


//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
void CALLBACK RacerApp::OnUpdateGame( double fTime, float fElapsedTime, void* pUserContext  )
{
	m_pGame->processInput( );
	m_pGame->simulate( fElapsedTime );
}



//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------

LRESULT CALLBACK RacerApp::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext )
{

	LRESULT result = 0;

	switch (uMsg) 
	{
        case WM_KEYDOWN: 
		case WM_KEYUP:

			m_pGame->addInput( uMsg, wParam );
			break;
	
		default:
			return 0;
	}

	return result;
}


//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
LRESULT RacerApp::OnDeviceChange(int eventType)
{			
	return TRUE;
}


//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
LRESULT RacerApp::OnDisplayChange(int colorDepth, int width, int height)
{	
	return 0;
}


//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
LRESULT RacerApp::OnClose()
{
	return 0;
}



//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK RacerApp::OnResetDevice(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC*, void* pUserContext  )
{
	return S_OK;
}


//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
void CALLBACK RacerApp::OnLostDevice(void* pUserContext )
{
	
}



//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
void CALLBACK RacerApp::OnRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext  )
{
	HRESULT hr;

    // Clear the render target and the zbuffer 
    V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 50, 45, 50, 170 ), 1.0f, 0 ) );

    // Render the scene
    if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {
		m_pGame->render( );

        V( pd3dDevice->EndScene() );
    }
}


//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
HRESULT CALLBACK RacerApp::OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	return S_OK;
}


//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
void CALLBACK RacerApp::OnDestroyDevice( void* pUserContext )
{
	
}


