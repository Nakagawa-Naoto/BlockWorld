// ピクセルシェーダへの入力
struct PS_IN
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};
// テクスチャのバインド
Texture2D tex : register(t0);
// サンプラーのバインド
SamplerState samp : register(s0);
float4 main(PS_IN pin) : SV_TARGET
{
    float4 color;
	// テクスチャをサンプリング
	// テクスチャから色を取得
    color = tex.Sample(samp, pin.uv);
    color *= pin.color;
    
    return color;
}