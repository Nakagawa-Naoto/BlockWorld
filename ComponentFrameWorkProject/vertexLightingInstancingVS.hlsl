
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
    //int3 BlockPos : BlockPosition;
    float4x4 mat : LocalToWorld;
};

struct VS_OUT
{
    float4 Position : SV_Position;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
};


VS_OUT main(VS_IN In)
{
    VS_OUT Out;
    
    matrix wvp;
    wvp = mul(In.mat, view);
    wvp = mul(wvp, projection);

    float4 worldNormal, normal;
    normal = float4(In.Normal.xyz, 0.0);
    worldNormal = mul(normal, In.mat);
    worldNormal = normalize(worldNormal);

    float d = -dot(Light.Direction.xyz, worldNormal.xyz);
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
    
    
    Out.Position = mul(In.Position, wvp);
    Out.Position = mul(In.Position, In.mat);
    Out.Position = mul(Out.Position, view);
    Out.Position = mul(Out.Position, projection);
    Out.TexCoord = In.TexCoord;

    return Out;
}