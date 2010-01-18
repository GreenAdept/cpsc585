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
		delete m_pGame;
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
void CALLBACK RacerApp::OnKeyboard ( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
	if( bKeyDown )
	{
		// online virtual key codes: http://msdn.microsoft.com/en-us/library/ms927178.aspx
		switch( nChar ) 
		{
		case  VK_SPACE:
			DXUTToggleFullScreen();
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
    if( pDeviceSettings->ver == DXUT_D3D9_DEVICE )
    {
        IDirect3D9* pD3D = DXUTGetD3D9Object();
        D3DCAPS9 Caps;
        pD3D->GetDeviceCaps( pDeviceSettings->d3d9.AdapterOrdinal, pDeviceSettings->d3d9.DeviceType, &Caps );

        // If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW 
        // then switch to SWVP.
        if( ( Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ) == 0 ||
            Caps.VertexShaderVersion < D3DVS_VERSION( 1, 1 ) )
        {
            pDeviceSettings->d3d9.BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
        }

        // Debugging vertex shaders requires either REF or software vertex processing 
        // and debugging pixel shaders requires REF.  
#ifdef DEBUG_VS
        if( pDeviceSettings->d3d9.DeviceType != D3DDEVTYPE_REF )
        {
            pDeviceSettings->d3d9.BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
            pDeviceSettings->d3d9.BehaviorFlags &= ~D3DCREATE_PUREDEVICE;
            pDeviceSettings->d3d9.BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
        }
#endif
#ifdef DEBUG_PS
        pDeviceSettings->d3d9.DeviceType = D3DDEVTYPE_REF;
#endif
    }

    // For the first device created if its a REF device, optionally display a warning dialog box
    static bool s_bFirstTime = true;
    if( s_bFirstTime )
    {
        s_bFirstTime = false;
        if( ( DXUT_D3D9_DEVICE == pDeviceSettings->ver && pDeviceSettings->d3d9.DeviceType == D3DDEVTYPE_REF ) ||
            ( DXUT_D3D10_DEVICE == pDeviceSettings->ver &&
              pDeviceSettings->d3d10.DriverType == D3D10_DRIVER_TYPE_REFERENCE ) )
            DXUTDisplaySwitchingToREFWarning( pDeviceSettings->ver );
    }

    return true;
}