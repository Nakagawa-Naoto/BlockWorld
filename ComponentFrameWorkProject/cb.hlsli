
cbuffer Time : register(b0)
{
    
}

cbuffer Light : register(b1)
{
    float4 color;
    float4 ambient;
}

cbuffer Matrixs : register(b2)
{
    float4x4 World;
    float4x4 View;
    float4x4 Projection;
}

cbuffer Matrial : register(b3)
{
    
}