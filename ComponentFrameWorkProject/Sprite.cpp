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
// 初期化処理
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

	// 頂点バッファ生成
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
	
	// テクスチャ読み込み
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
// 終了処理
//-----------------------------------------------------------------------------
void Sprite::Uninit()
{
	m_pVertexBuffer->Release();
	m_pTexture->Release();
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void Sprite::Draw()
{
	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

	// ワールドマトリクス設定
	Matrix world, scale, rot, trans;
	scale = Matrix::CreateScale(m_pGameObject->GetScale());
	rot = Matrix::CreateFromYawPitchRoll(m_pGameObject->GetRotation());
	trans = Matrix::CreateTranslation(m_pGameObject->GetPosition());
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	
	// マテリアル設定
	Renderer::SetMaterial(m_Material);

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_pTexture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}
