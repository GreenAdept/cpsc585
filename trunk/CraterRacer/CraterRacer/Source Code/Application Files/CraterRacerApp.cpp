//--------------------------------------------------------------------------------------
// File: CraterRacerApp.cpp
//
//--------------------------------------------------------------------------------------
#include "CraterRacerApp.h"

GameObj* m_pGame = NULL;

//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
RacerApp::RacerApp()
{
	m_pGame = new GameObj();
}


//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
RacerApp::~RacerApp()
{
	if( m_pGame )
	{
		m_pGame->processCallback( ON_DESTROY );
		delete m_pGame;
		m_pGame = NULL;
	}
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
	return 0;
}


//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
void CALLBACK RacerApp::OnKeyboard ( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
	if( m_pGame )
	{
		// online virtual key codes: http://msdn.microsoft.com/en-us/library/ms927178.aspx
		switch( nChar ) 
		{
			case  VK_SPACE:
				//pause game here
				break;

			default:
				m_pGame->addInput( bKeyDown, nChar );
				break;
		}
	}
}


//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
void CALLBACK RacerApp::OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{


}


//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK RacerApp::OnResetDevice( IDirect3DDevice9* device, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{

	if( m_pGame )
		m_pGame->processCallback( ON_RESET, device, pBackBufferSurfaceDesc );
	
	device->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

	return S_OK;
}


//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
void CALLBACK RacerApp::OnLostDevice(void* pUserContext )
{
	if( m_pGame )
		m_pGame->processCallback( ON_LOST );
}



//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
void CALLBACK RacerApp::OnRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext  )
{
	HRESULT hr;

    // Clear the render target and the zbuffer 
    V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 150,0, 20, 30 ), 1.0f, 0 ) );

    // Render the scene
    if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {
		m_pGame->render( pd3dDevice );

        V( pd3dDevice->EndScene() );
    }
}


//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
HRESULT CALLBACK RacerApp::OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	m_pGame->initGame( pd3dDevice, pBackBufferSurfaceDesc );

	return S_OK;
}


//-------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
void CALLBACK RacerApp::OnDestroyDevice( void* pUserContext )
{
	if( m_pGame )
		m_pGame->processCallback( ON_DESTROY );
}


//--------------------------------------------------------------------------------------
// Rejects any D3D9 devices that aren't acceptable to the app by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK RacerApp::IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat,
                                      D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
    // Skip backbuffer formats that don't support alpha blending
    IDirect3D9* pD3D = DXUTGetD3D9Object();
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                                         AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
                                         D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;

    // No fallback defined by this app, so reject any device that 
    // doesn't support at least ps2.0
    if( pCaps->PixelShaderVersion < D3DPS_VERSION( 2, 0 ) )
        return false;

    return true;
}


//--------------------------------------------------------------------------------------
// Called right before creating a D3D9 or D3D10 device, allowing the app to modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK RacerApp::ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
    return true;
}

