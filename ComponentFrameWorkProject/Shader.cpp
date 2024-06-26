//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "Shader.h"

///////////////////////////////////////////////////////////////////////////////
// Shader class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// 読み込み処理
//-----------------------------------------------------------------------------
void Shader::Load(const char* VertexShader, const char* PixelShader, D3D11_INPUT_ELEMENT_DESC* layout,const UINT elements)
{
	Renderer::CreateVertexShader(&m_pVertexShader, &m_pVertexLayout, layout, elements, VertexShader);
	Renderer::CreatePixelShader(&m_pPixelShader, PixelShader);
}

//-----------------------------------------------------------------------------     
// 終了処理
//-----------------------------------------------------------------------------
void Shader::Uninit()
{
	m_pVertexLayout->Release();
	m_pVertexShader->Release();
	m_pPixelShader->Release();
}

//-----------------------------------------------------------------------------     
// 描画処理
//-----------------------------------------------------------------------------
void Shader::Draw()
{

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_pVertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_pVertexShader, nullptr, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_pPixelShader, nullptr, 0);
}
