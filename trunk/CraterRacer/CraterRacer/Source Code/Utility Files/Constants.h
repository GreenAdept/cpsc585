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
#define CAR_BODY_FILE	L"Media\\Meshes\\CarBody.x"
#define WHEEL0_FILE		L"Media\\Meshes\\Wheel0.x"
#define WHEEL1_FILE		L"Media\\Meshes\\Wheel1.x"
#define WHEEL2_FILE		L"Media\\Meshes\\Wheel2.x"
#define WHEEL3_FILE		L"Media\\Meshes\\Wheel3.x"
#define OBJ_EFFECT		L"Source Code\\Effect Files\\Scene.fx"
#define TERRAIN_FILE	L"Media\\Meshes\\Floor.x"
#define SOUNDBANK_FILE	L"Media\\Audio\\SoundBank.xsb"
#define MENU_BG_FILE	L"Media\\Audio\\MenuBG.xwb"

enum ProcessType		{ ON_LOST, ON_RESET, ON_DESTROY };
enum Directions			{ LEFT, FORWARD, RIGHT, BACKWARD }; 
enum GuiControls		{ GUI_BTN_SINGLE_PLAYER, GUI_BTN_EXIT, GUI_BTN_UNPAUSE, GUI_BTN_EXIT2 };
enum ApplicationState	{ APP_STARTUP, APP_RENDER_GAME, APP_PAUSED };
enum Wheels				{ WHEEL0, WHEEL1, WHEEL2, WHEEL3 };


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
	Vec3  m_fCenter;
	float m_fRadius;
};


#endif

