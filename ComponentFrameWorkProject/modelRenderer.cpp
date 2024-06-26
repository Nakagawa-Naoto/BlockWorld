#define _CRT_SECURE_NO_WARNINGS
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main.h"
#include "renderer.h"
#include "modelRenderer.h"
#include "Loader.h"

using namespace DirectX::SimpleMath;

///////////////////////////////////////////////////////////////////////////////
// ModelRenderer class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void ModelRenderer::Draw()
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pModel->VertexBuffer, &stride, &offset);

	// インデックスバッファ設定
	if(m_bIndex)Renderer::GetDeviceContext()->IASetIndexBuffer(m_pModel->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	if (!m_bIndex) { Renderer::GetDeviceContext()->Draw(m_uSize, 0); return; }

	for( unsigned int i = 0; i < m_pModel->SubsetNum; i++ )
	{
		// マテリアル設定
		Renderer::SetMaterial(m_pModel->SubsetArray[i].Material.Material );

		// テクスチャ設定
		if(m_pModel->SubsetArray[i].Material.Texture)
			Renderer::GetDeviceContext()->PSSetShaderResources( 0, 1, &m_pModel->SubsetArray[i].Material.Texture );

		// ポリゴン描画
		Renderer::GetDeviceContext()->DrawIndexed(m_pModel->SubsetArray[i].IndexNum, m_pModel->SubsetArray[i].StartIndex, 0 );
		
	}

}

//-----------------------------------------------------------------------------
// 読み込み処理
//-----------------------------------------------------------------------------
void ModelRenderer::Load(const char* FileName)
{
	Loader::Load3DModel(FileName, &m_pModel);
}

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void ModelRenderer::Init(VERTEX_3D* vertex,int num)
{
	m_bIndex = false;
	m_pData = _mm_malloc(sizeof(VERTEX_3D)*num, 16);
	m_pModel = new MODEL();
	m_uSize = num;
	// 頂点バッファ生成
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * num;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		//bd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_pModel->VertexBuffer);
	}
	
}
