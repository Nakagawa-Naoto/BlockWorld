
// ピクセルシェーダへの入力
struct PS_IN
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

float4 main(PS_IN In) : SV_TARGET
{
	return In.color;
}