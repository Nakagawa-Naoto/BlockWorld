// �s�N�Z���V�F�[�_�ւ̓���
struct PS_IN
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};
// �e�N�X�`���̃o�C���h
Texture2D tex : register(t0);
// �T���v���[�̃o�C���h
SamplerState samp : register(s0);
float4 main(PS_IN pin) : SV_TARGET
{
    float4 color;
	// �e�N�X�`�����T���v�����O
	// �e�N�X�`������F���擾
    color = tex.Sample(samp, pin.uv);
    color *= pin.color;
    
    return color;
}