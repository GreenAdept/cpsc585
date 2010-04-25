//-----------------------------------------------------------------------------
// File: SkyBox.fx
//
// Desc: Effect file to render a sky box.
//
//-----------------------------------------------------------------------------

float4x4 g_WorldViewProj;
texture  g_EnvironmentMap;

sampler EnvMapS = sampler_state
{
    Texture   = <g_EnvironmentMap>;
    MinFilter = LINEAR; 
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU  = WRAP;
    AddressV  = WRAP;
};


//-----------------------------------------------------------------------------
// Vertex Shader: SkyVS
// Desc: Process vertex for scene
//-----------------------------------------------------------------------------
void SkyVS(float3 posL : POSITION0, 
           out float4 oPosH : POSITION0, 
           out float3 oEnvTex : TEXCOORD0)
{
	// Set z = w so that z/w = 1 (i.e., skydome always on far plane).
    oPosH = mul( float4(posL, 1.0f), g_WorldViewProj ).xyww; 
    
    // Use skymesh vertex position, in local space, as index into cubemap. 
    oEnvTex = posL;
}


//-----------------------------------------------------------------------------
// Pixel Shader: SkyPS
// Desc: Process pixel for scene
//-----------------------------------------------------------------------------
float4 SkyPS(float3 envTex : TEXCOORD0) : COLOR
{
    return texCUBE(EnvMapS, envTex);
}


//-----------------------------------------------------------------------------
// Technique: SkyTechnique
// Desc: Renders skybox
//-----------------------------------------------------------------------------
technique SkyTechnique
{
    pass P0
    {
        vertexShader = compile vs_2_0 SkyVS();
        pixelShader  = compile ps_2_0 SkyPS();
		CullMode = None;
		ZFunc = Always; // Always write sky to depth buffer
		StencilEnable = true;
		StencilFunc   = Always;
		StencilPass   = Replace;
		StencilRef    = 0; // clear to zero
    }
}

