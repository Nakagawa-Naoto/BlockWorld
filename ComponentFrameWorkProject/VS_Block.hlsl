
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


struct VS_IN
{
    float4 Position : POSITION0;
    float3 Normal : NORMAL0;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
    uint2 blockData : BlockData;
};

struct VS_OUT
{
    float4 Position : SV_Position;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
    int Block : BlockID;
};

uint3 GetPositionData(uint2 data)
{
    uint3 position = uint3(0, 0, 0);
    uint tmp = data.x;
    tmp >>= 8;
    position.x = tmp;
    
    tmp =data.x;
    tmp <<= 24;
    tmp >>= 8;
    position.z = tmp;
    tmp = data.y;
    tmp >>= 16;
    position.z |= tmp;
    
    tmp = data.y;
    tmp <<= 16;
    tmp >>= 24;
    position.y = tmp;
    
    return position;
}
int GetBlockID(uint2 data)
{
    int Id = 0;
    uint tmp = data.y;
    tmp <<= 24;
    tmp >>= 24;
    Id = tmp;
    return Id;
}

VS_OUT main(VS_IN In)
{
    VS_OUT Out;
    uint3 blockPos = GetPositionData(In.blockData);
    uint tmp = In.blockData.y;    
    tmp = In.blockData.y;
    tmp <<= 24;
    tmp >>= 24;
    
    matrix wvp;
    wvp = mul(world, view);
    wvp = mul(wvp, projection);
    
    float d = -dot(Light.Direction.xyz, In.Normal.xyz);
    d = saturate(d);

    //Out.Diffuse = In.Diffuse * Material.Diffuse * d * Light.Diffuse;
    //Out.Diffuse += In.Diffuse * Material.Ambient * Light.Ambient;
    Out.Diffuse = In.Diffuse * Material.Diffuse * d * Light.Diffuse;
    Out.Diffuse += In.Diffuse * Material.Ambient * Light.Ambient;
    Out.Diffuse += Material.Emission;
    Out.Diffuse.a = In.Diffuse.a * Material.Diffuse.a;

    //Out.Diffuse.xyz = In.Diffuse.xyz * Material.Diffuse.xyz * d * Light.Diffuse.xyz;
    //Out.Diffuse.xyz += In.Diffuse.xyz * Material.Ambient.xyz * Light.Ambient.xyz;
    Out.Diffuse.xyz = float3(1.0f, 1.0f, 1.0f) * d * Light.Diffuse.xyz;
    Out.Diffuse.xyz += float3(1.0f, 1.0f, 1.0f) * Material.Ambient.xyz * Light.Ambient.xyz;
    Out.Diffuse.xyz += Material.Emission.xyz;
    Out.Diffuse.a = In.Diffuse.a * Material.Diffuse.a;
    
    
    Out.Position = In.Position;
    Out.Position += float4(blockPos, 0);
    Out.Position = mul(Out.Position, world);
    Out.Position = mul(Out.Position, view);
    Out.Position = mul(Out.Position, projection);
    Out.TexCoord = In.TexCoord;
    Out.Block = GetBlockID(In.blockData);
    return Out;
}