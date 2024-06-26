struct VS_IN
{
    float4 Position : POSITION0;
    float4 Normal : NORMAL0;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
    int4 BoneIndex : BONEINDEX;
    float4 BoneWeight : BONEWEIGHT;
};

struct VS_OUT
{
    float4 Position : SV_POSITION;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
};


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
struct LIGHT
{
    bool Enable;
    float4 Direction;
    float4 Diffuse;
    float4 Ambient;
};
cbuffer Light : register(b4)
{
    LIGHT Light;
};

cbuffer BoneMatrixBuffer : register(b5)
{
    matrix BoneMatrix[400];
}


VS_OUT main(VS_IN vin)
{
    VS_OUT vout = (VS_OUT) 0;
    
    float4x4 comb = (float4x4) 0;
    for (int i = 0; i < 4; i++)
    {
        // d‚Ý‚ðŒvŽZ‚µ‚È‚ª‚çs—ñ¶¬
        comb += BoneMatrix[vin.BoneIndex[i]] * vin.BoneWeight[i];
    }
    
    float4 pos;
    pos = mul(comb, vin.Position);
    
    vin.Position = pos;
    
    // •ÏŠ·
    
    matrix wvp;
    wvp = mul(world, view);
    wvp = mul(wvp, projection);

    float4 worldNormal, normal;
    normal = float4(vin.Normal.xyz, 0.0);
    worldNormal = mul(normal, world);
    worldNormal = normalize(worldNormal);

    float d = -dot(Light.Direction.xyz, worldNormal.xyz);
    d = saturate(d);

    vout.Diffuse = vin.Diffuse * Material.Diffuse * d * Light.Diffuse;
    vout.Diffuse += vin.Diffuse * Material.Ambient * Light.Ambient;
    vout.Diffuse += Material.Emission;
    vout.Diffuse.a = vin.Diffuse.a * Material.Diffuse.a;
    
    vout.Diffuse.xyz = vin.Diffuse.xyz * Material.Diffuse.xyz * d * Light.Diffuse.xyz;
    vout.Diffuse.xyz += vin.Diffuse.xyz * Material.Ambient.xyz * Light.Ambient.xyz;
    vout.Diffuse.xyz += Material.Emission.xyz;
    vout.Diffuse.a = vin.Diffuse.a * Material.Diffuse.a;
    
    vout.Position = mul(vin.Position, wvp);
    vout.TexCoord = vin.TexCoord;
    
    //vout.Diffuse = pos;
    
    return vout;
}