
struct MATERIAL
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emission;
    float Shiness;
    bool TextureEnable;
};
cbuffer Material : register(b3)
{
    MATERIAL Material;
};
Texture2D g_Texture : register(t0);
Texture2D g_GrassTex : register(t1);
Texture2D g_DirtTex : register(t2);
Texture2D g_StoneTex : register(t3);
Texture2D g_SandTex : register(t4);
SamplerState g_SamplerState : register(s0);

struct PS_IN
{
    float4 Position : SV_Position;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
    int Block : BlockID;
};

float4 GetBlockColor(const int block,const float2 uv)
{
    if (block == 0)return g_Texture.Sample(g_SamplerState, uv);
    if (block == 1)return g_GrassTex.Sample(g_SamplerState, uv);
    if (block == 2)return g_DirtTex.Sample(g_SamplerState, uv);
    if (block == 3)return g_StoneTex.Sample(g_SamplerState, uv);
    if (block == 4)return g_SandTex.Sample(g_SamplerState, uv);
    return float4(1,0,1,1);
}


float4 main(PS_IN In) : SV_TARGET
{
    float4 outDiffuse;
    
    outDiffuse = GetBlockColor(In.Block, In.TexCoord);   
    
    outDiffuse *= In.Diffuse;
    
    return outDiffuse;
}