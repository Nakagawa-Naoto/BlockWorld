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
// �e�N�X�`���̐ݒ�
//-----------------------------------------------------------------------------
void InstancingModelRenderer::SetTexture(const int num, const wchar_t* FileName)
{
	DirectX::CreateWICTextureFromFile(Renderer::GetDevice(),FileName,nullptr,&m_pTexture[num]);	
}

//-----------------------------------------------------------------------------
// �ǂݍ��ݏ���
//-----------------------------------------------------------------------------
void InstancingModelRenderer::Load(const char* FileName)
{
	Loader::Load3DModel(FileName, &m_pModel);
}

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
void InstancingModelRenderer::Init(const char* FileName, int num)
{
	// ���f���̓Ǎ�
	Load(FileName);

	// �C���X�^���V���O
	m_nInstanceCount = num;
	m_pData = _mm_malloc(sizeof(DirectX::XMINT4) * num, 16);
	DirectX::XMINT2* pstart = static_cast<DirectX::XMINT2*>(m_pData);

	// ������
	for (int i = 0; i < m_nInstanceCount; i++) {
		*pstart = DirectX::XMINT2(0,0);
		pstart++;
	}
	

	{
		HRESULT hr;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;							// �o�b�t�@�g�p���@
		bd.ByteWidth = sizeof(DirectX::XMINT2) * num;			// �o�b�t�@�̑傫��
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;				// ���_�o�b�t�@
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;				// CPU�A�N�Z�X�\
		
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = m_pData;							// �o�b�t�@�̏����l

		hr = Renderer::GetDevice()->CreateBuffer(&bd, &InitData, &m_pInstanceBuffer);
		if (FAILED(hr)) {
			MessageBox(nullptr, "CreateBuffer(vertex buffer) error", "Error", MB_OK);
			return ;
		}
	}
}

//-----------------------------------------------------------------------------
// �u���b�N�̃f�[�^�}�g���b�N�X���X�V
//-----------------------------------------------------------------------------
void InstancingModelRenderer::UpdateMatrix(DirectX::XMINT2 data[])
{
	D3D11_MAPPED_SUBRESOURCE pData;

	DirectX::XMINT2* pstart = static_cast<DirectX::XMINT2*>(m_pData);

	// �C���X�^���X�o�b�t�@�̏����s����Z�b�g
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
// �`�揈��
//-----------------------------------------------------------------------------
void InstancingModelRenderer::Draw()
{
	int indexnum = 0;
	int indexstart = 0;

	// ���͂��钸�_�o�b�t�@�ƃC���X�^���X�o�b�t�@
	ID3D11Buffer* vbuffer[2] = {
		m_pModel->VertexBuffer ,
		m_pInstanceBuffer };
	// ���ꂼ��̃X�g���C�h���Z�b�g
	unsigned int stride[2] = { sizeof(VERTEX_3D),sizeof(DirectX::XMINT2) };
	// �I�t�Z�b�g���Z�b�g
	unsigned  offset[2] = { 0,0 };

	// ���_�o�b�t�@���Z�b�g	
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 2, vbuffer, stride, offset);

	// �C���f�b�N�X�o�b�t�@���Z�b�g
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_pModel->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// �g�|���W�[���Z�b�g�i���v���~�e�B�u�^�C�v�j
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// �u���b�N�̃e�N�X�`���𑗂�
	for (int i = 0; i < 4; i++)
	{
		Renderer::GetDeviceContext()->PSSetShaderResources(i + 1, 1, &m_pTexture[i]);
	}
	
	for (unsigned int i = 0; i < m_pModel->SubsetNum; i++)
	{
		// �}�e���A���ݒ�
		Renderer::SetMaterial(m_pModel->SubsetArray[i].Material.Material);

		// �e�N�X�`���ݒ�
		if (m_pModel->SubsetArray[i].Material.Texture)
			Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_pModel->SubsetArray[i].Material.Texture);

		// �|���S���`��
		//Renderer::GetDeviceContext()->DrawIndexed(m_Model->SubsetArray[i].IndexNum, m_Model->SubsetArray[i].StartIndex, 0);
		Renderer::GetDeviceContext()->DrawIndexedInstanced(
			m_pModel->SubsetArray[i].IndexNum,
			m_nInstanceCount, 
			m_pModel->SubsetArray[i].StartIndex,
			0, 0);
		
	}

}
