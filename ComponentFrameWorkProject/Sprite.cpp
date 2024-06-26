//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <WICTextureLoader.h>
#include <SimpleMath.h>
#include "GameObject.h"
#include "Sprite.h"

using namespace DirectX::SimpleMath;

///////////////////////////////////////////////////////////////////////////////
// Sprite class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
void Sprite::Init(int x, int y, int Width, int Height, const wchar_t* TextureName)
{
	VERTEX_3D vertex[4];

	vertex[0].Position = Vector3((float)x, (float)y, 0.0f);
	vertex[0].Normal = Vector3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = Vector2(0.0f, 0.0f);

	vertex[1].Position = Vector3((float)x + Width, (float)y, 0.0f);
	vertex[1].Normal = Vector3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = Vector2(1.0f, 0.0f);

	vertex[2].Position = Vector3((float)x, (float)y + Height, 0.0f);
	vertex[2].Normal = Vector3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = Vector2(0.0f, 1.0f);

	vertex[3].Position = Vector3((float)x + Width, (float)y + Height, 0.0f);
	vertex[3].Normal = Vector3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = Vector2(1.0f, 1.0f);

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_pVertexBuffer);
	
	// �e�N�X�`���ǂݍ���
	DirectX::CreateWICTextureFromFile(
		Renderer::GetDevice(),
		TextureName,
		nullptr,
		&m_pTexture);
	assert(m_pTexture);
	
	m_Material.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Material.TextureEnable = true;

}

//-----------------------------------------------------------------------------
// �I������
//-----------------------------------------------------------------------------
void Sprite::Uninit()
{
	m_pVertexBuffer->Release();
	m_pTexture->Release();
}

//-----------------------------------------------------------------------------
// �`�揈��
//-----------------------------------------------------------------------------
void Sprite::Draw()
{
	// �}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	// ���[���h�}�g���N�X�ݒ�
	Matrix world, scale, rot, trans;
	scale = Matrix::CreateScale(m_pGameObject->GetScale());
	rot = Matrix::CreateFromYawPitchRoll(m_pGameObject->GetRotation());
	trans = Matrix::CreateTranslation(m_pGameObject->GetPosition());
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	
	// �}�e���A���ݒ�
	Renderer::SetMaterial(m_Material);

	// �e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_pTexture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}
