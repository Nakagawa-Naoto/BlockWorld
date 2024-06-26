//#include <cb.hlsli>
cbuffer World : register(b0)
{
    Matrix world;
};
cbuffer View : register(b1)
{
    Matrix view;
};
cbuffer Projection : register(b2)
{
    Matrix projection;
};

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

struct VS_IN
{
    float4 pos : POSITION0;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};
struct VS_OUT
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

VS_OUT main(VS_IN vin)
{
    
    matrix wvp;
    wvp = mul(world, view);
    wvp = mul(wvp, projection);

    VS_OUT vout;
    vout.pos = mul(vin.pos, wvp);
    vout.normal = vin.normal;
    vout.color = vin.color * Material.Diffuse;
    vout.uv = vin.uv;
    
    
    return vout;
}