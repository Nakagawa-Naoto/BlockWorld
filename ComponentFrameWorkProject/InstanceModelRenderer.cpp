//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "InstanceModelRenderer.h"
#include <WICTextureLoader.h>
#include "main.h"
#include "renderer.h"
#include "Loader.h"

using namespace DirectX::SimpleMath;

///////////////////////////////////////////////////////////////////////////////
// InstancingModelRenderer class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// テクスチャの設定
//-----------------------------------------------------------------------------
void InstancingModelRenderer::SetTexture(const int num, const wchar_t* FileName)
{
	DirectX::CreateWICTextureFromFile(Renderer::GetDevice(),FileName,nullptr,&m_pTexture[num]);	
}

//-----------------------------------------------------------------------------
// 読み込み処理
//-----------------------------------------------------------------------------
void InstancingModelRenderer::Load(const char* FileName)
{
	Loader::Load3DModel(FileName, &m_pModel);
}

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void InstancingModelRenderer::Init(const char* FileName, int num)
{
	// モデルの読込
	Load(FileName);

	// インスタンシング
	m_nInstanceCount = num;
	m_pData = _mm_malloc(sizeof(DirectX::XMINT4) * num, 16);
	DirectX::XMINT2* pstart = static_cast<DirectX::XMINT2*>(m_pData);

	// 初期化
	for (int i = 0; i < m_nInstanceCount; i++) {
		*pstart = DirectX::XMINT2(0,0);
		pstart++;
	}
	

	{
		HRESULT hr;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;							// バッファ使用方法
		bd.ByteWidth = sizeof(DirectX::XMINT2) * num;			// バッファの大きさ
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;				// 頂点バッファ
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;				// CPUアクセス可能
		
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = m_pData;							// バッファの初期値

		hr = Renderer::GetDevice()->CreateBuffer(&bd, &InitData, &m_pInstanceBuffer);
		if (FAILED(hr)) {
			MessageBox(nullptr, "CreateBuffer(vertex buffer) error", "Error", MB_OK);
			return ;
		}
	}
}

//-----------------------------------------------------------------------------
// ブロックのデータマトリックスを更新
//-----------------------------------------------------------------------------
void InstancingModelRenderer::UpdateMatrix(DirectX::XMINT2 data[])
{
	D3D11_MAPPED_SUBRESOURCE pData;

	DirectX::XMINT2* pstart = static_cast<DirectX::XMINT2*>(m_pData);

	// インスタンスバッファの初期行列をセット
	for (int i = 0; i < m_nInstanceCount; i++) {
		*pstart = data[i];
		pstart++;
	}

	HRESULT hr = Renderer::GetDeviceContext()->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	
	if (!SUCCEEDED(hr))return;
	memcpy_s(pData.pData, pData.RowPitch, (void*)(m_pData), sizeof(DirectX::XMINT2) * m_nInstanceCount);
	Renderer::GetDeviceContext()->Unmap(m_pInstanceBuffer, 0);
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void InstancingModelRenderer::Draw()
{
	int indexnum = 0;
	int indexstart = 0;

	// 入力する頂点バッファとインスタンスバッファ
	ID3D11Buffer* vbuffer[2] = {
		m_pModel->VertexBuffer ,
		m_pInstanceBuffer };
	// それぞれのストライドをセット
	unsigned int stride[2] = { sizeof(VERTEX_3D),sizeof(DirectX::XMINT2) };
	// オフセットをセット
	unsigned  offset[2] = { 0,0 };

	// 頂点バッファをセット	
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 2, vbuffer, stride, offset);

	// インデックスバッファをセット
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_pModel->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// トポロジーをセット（旧プリミティブタイプ）
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// ブロックのテクスチャを送る
	for (int i = 0; i < 4; i++)
	{
		Renderer::GetDeviceContext()->PSSetShaderResources(i + 1, 1, &m_pTexture[i]);
	}
	
	for (unsigned int i = 0; i < m_pModel->SubsetNum; i++)
	{
		// マテリアル設定
		Renderer::SetMaterial(m_pModel->SubsetArray[i].Material.Material);

		// テクスチャ設定
		if (m_pModel->SubsetArray[i].Material.Texture)
			Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_pModel->SubsetArray[i].Material.Texture);

		// ポリゴン描画
		//Renderer::GetDeviceContext()->DrawIndexed(m_Model->SubsetArray[i].IndexNum, m_Model->SubsetArray[i].StartIndex, 0);
		Renderer::GetDeviceContext()->DrawIndexedInstanced(
			m_pModel->SubsetArray[i].IndexNum,
			m_nInstanceCount, 
			m_pModel->SubsetArray[i].StartIndex,
			0, 0);
		
	}

}
