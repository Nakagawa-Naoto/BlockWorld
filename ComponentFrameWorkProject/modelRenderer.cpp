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
// �`�揈��
//-----------------------------------------------------------------------------
void ModelRenderer::Draw()
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pModel->VertexBuffer, &stride, &offset);

	// �C���f�b�N�X�o�b�t�@�ݒ�
	if(m_bIndex)Renderer::GetDeviceContext()->IASetIndexBuffer(m_pModel->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	if (!m_bIndex) { Renderer::GetDeviceContext()->Draw(m_uSize, 0); return; }

	for( unsigned int i = 0; i < m_pModel->SubsetNum; i++ )
	{
		// �}�e���A���ݒ�
		Renderer::SetMaterial(m_pModel->SubsetArray[i].Material.Material );

		// �e�N�X�`���ݒ�
		if(m_pModel->SubsetArray[i].Material.Texture)
			Renderer::GetDeviceContext()->PSSetShaderResources( 0, 1, &m_pModel->SubsetArray[i].Material.Texture );

		// �|���S���`��
		Renderer::GetDeviceContext()->DrawIndexed(m_pModel->SubsetArray[i].IndexNum, m_pModel->SubsetArray[i].StartIndex, 0 );
		
	}

}

//-----------------------------------------------------------------------------
// �ǂݍ��ݏ���
//-----------------------------------------------------------------------------
void ModelRenderer::Load(const char* FileName)
{
	Loader::Load3DModel(FileName, &m_pModel);
}

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
void ModelRenderer::Init(VERTEX_3D* vertex,int num)
{
	m_bIndex = false;
	m_pData = _mm_malloc(sizeof(VERTEX_3D)*num, 16);
	m_pModel = new MODEL();
	m_uSize = num;
	// ���_�o�b�t�@����
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
