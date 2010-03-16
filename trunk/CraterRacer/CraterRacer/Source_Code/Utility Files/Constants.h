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

#define WHEEL0_FILE		L"Media\\Meshes\\Wheel0.x"
#define WHEEL1_FILE		L"Media\\Meshes\\Wheel1.x"
#define WHEEL2_FILE		L"Media\\Meshes\\Wheel2.x"
#define WHEEL3_FILE		L"Media\\Meshes\\Wheel3.x"

#define MAX_SPEED_TIME_ELAPSED 0.2

enum ProcessType		{ ON_LOST, ON_RESET, ON_DESTROY };
enum Directions			{ LEFT, FORWARD, RIGHT, BACKWARD }; 
enum Wheels				{ WHEEL0, WHEEL1, WHEEL2, WHEEL3 };
enum GameType			{ ONE_PLAYER, MULTI_PLAYER, TIME_TRIAL };
enum Player				{ PLAYER1 = 0, PLAYER2 };


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
typedef LPDIRECT3DTEXTURE9			Sprite;
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
	Vec3  m_fCenter;
	float m_fRadius;
};


#endif

