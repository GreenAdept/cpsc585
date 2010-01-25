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

#define OBJ_FILE		L"Media\\Meshes\\Box.x"
#define OBJ_EFFECT		L"Source Code\\Effect Files\\Scene.fx"
#define TERRAIN_FILE	L"Media\\Meshes\\Terrain\\terrain01.x"

enum ProcessType { ON_LOST, ON_RESET, ON_DESTROY };

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

#endif

