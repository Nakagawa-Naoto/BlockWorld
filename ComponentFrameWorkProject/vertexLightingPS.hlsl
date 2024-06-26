
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
SamplerState g_SamplerState : register(s0);

struct PS_IN
{
    float4 Position : SV_Position;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
};



float4 main(PS_IN In) : SV_TARGET
{
    float4 outDiffuse;
    if (Material.TextureEnable)
    {
        outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
        outDiffuse *= In.Diffuse;
    }
    else
    {
        outDiffuse.xyz = In.Diffuse.xyz;
        outDiffuse = In.Diffuse;
        outDiffuse.a = 1.0f;
    }
    
    return outDiffuse;
}