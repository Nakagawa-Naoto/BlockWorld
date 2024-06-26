//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "Shader.h"

///////////////////////////////////////////////////////////////////////////////
// Shader class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// �ǂݍ��ݏ���
//-----------------------------------------------------------------------------
void Shader::Load(const char* VertexShader, const char* PixelShader, D3D11_INPUT_ELEMENT_DESC* layout,const UINT elements)
{
	Renderer::CreateVertexShader(&m_pVertexShader, &m_pVertexLayout, layout, elements, VertexShader);
	Renderer::CreatePixelShader(&m_pPixelShader, PixelShader);
}

//-----------------------------------------------------------------------------     
// �I������
//-----------------------------------------------------------------------------
void Shader::Uninit()
{
	m_pVertexLayout->Release();
	m_pVertexShader->Release();
	m_pPixelShader->Release();
}

//-----------------------------------------------------------------------------     
// �`�揈��
//-----------------------------------------------------------------------------
void Shader::Draw()
{

	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_pVertexLayout);

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_pVertexShader, nullptr, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_pPixelShader, nullptr, 0);
}
