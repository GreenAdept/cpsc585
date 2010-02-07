#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H

#define NOMINMAX
#include "DXUT.h"
#include "SDKmesh.h"
#include "DXUTgui.h"
#include "DXUTmisc.h"
#include "DXUTCamera.h"
#include "DXUTSettingsDlg.h"
#include "SDKmisc.h"


//--------------------------------------------------------
//		DEFINITIONS
//--------------------------------------------------------

#define OBJ_FILE		L"Media\\Meshes\\CarBox.x"
#define OBJ_EFFECT		L"Source Code\\Effect Files\\Scene.fx"
#define TERRAIN_FILE	L"Media\\Meshes\\Floor.x"

enum ProcessType		{ ON_LOST, ON_RESET, ON_DESTROY };
enum Directions			{ LEFT, FORWARD, RIGHT, BACKWARD }; 
enum GuiControls		{ GUI_BTN_SINGLE_PLAYER };
enum ApplicationState	{ APP_STARTUP, APP_RENDER_GAME };


//--------------------------------------------------------
//		TYPEDEFS
//--------------------------------------------------------

typedef D3DXVECTOR3					Vec3;
typedef D3DXVECTOR4					Vec4;
typedef CDXUTDialog					Dialog;
typedef CDXUTXFileMesh				Mesh;
typedef D3DXMATRIX					Matrix;
typedef CModelViewerCamera			MCamera;	// A model viewing camera
typedef D3DXHANDLE					Handle;		// Handle to RenderScene technique
typedef ID3DXEffect					Effect;
typedef IDirect3DDevice9			Device;
typedef IDirect3DSurface9			Surface;
typedef IDirect3DTexture9			Texture;
typedef IDirect3DVertexDeclaration9 VDecl;
typedef LPDIRECT3DINDEXBUFFER9		IndexBuffer;
typedef D3DINDEXBUFFER_DESC			IBufferDesc;
typedef CDXUTDialogResourceManager	ResourceManager;


//--------------------------------------------------------
//		STRUCTS
//--------------------------------------------------------

struct BoundingBox
{
	float m_fWidth;
	float m_fHeight;
	float m_fLength;
};


#endif

