//--------------------------------------------------------------------------------------
// File: DisplayMesh.fx
// A simple effect file to get our hardware rendering support working.  This file is 
// adapted from an effect file in the DirectX SDK samples.
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
float4   g_vDiffuse;                // Material diffuse color
float4x4 g_mWorld;					// World matrix for object
float4x4 g_mWorldViewProjection;	// World * View * Projection matrix
texture  g_txScene;


sampler g_samScene =
sampler_state
{
    Texture = <g_txScene>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
};


void VertScene( float4 Pos : POSITION,
                float3 Normal : NORMAL,
                float2 Tex : TEXCOORD0,
                out float4 oPos : POSITION,
                out float2 oTex : TEXCOORD0,
                out float4 Diffuse : COLOR0 )
{
    oPos = mul( Pos, g_mWorldViewProjection );

    float3 N = normalize( mul( Normal, (float3x3)g_mWorld ) );
    Diffuse =  g_vDiffuse; 

    oTex = Tex;
}


float4 PixScene( float2 Tex : TEXCOORD0,
                 float4 Diffuse : COLOR0 ) : COLOR0
{
    return  tex2D( g_samScene, Tex ) * Diffuse; 
}
                

//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique RenderScene
{
    pass P0
    {
        VertexShader = compile vs_2_0 VertScene();
        PixelShader = compile ps_2_0 PixScene();
    }
}
