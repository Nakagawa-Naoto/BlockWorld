//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Camera.h"
#include "GameObject.h"
#include "main.h"
#include "Scene.h"
#include "GUI.h"

using namespace DirectX::SimpleMath;
///////////////////////////////////////////////////////////////////////////////
// Camera class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
void Camera::Init() 
{
	m_Target = Vector3(0, 0, 1);
	m_Up = Vector3(0, 1, 0);
	m_aspect.x = SCREEN_WIDTH;
	m_aspect.y = SCREEN_HEIGHT;
	m_fFieldOfView = DirectX::XMConvertToRadians(60.0f);// ����p�ݒ�
	m_fNearPlane = 0.1f;		// �j�A�N���b�v
	m_fFarPlane = 120.0f;	// �t�@�[�N���b�v

	//�J�����̓o�^
	m_pGameObject->GetScene()->SetCamera(this);
}

//-----------------------------------------------------------------------------
// �`�揈��
//-----------------------------------------------------------------------------
void Camera::Draw()
{
	Vector3 pos = m_pGameObject->GetPosition() + m_OffsetPos;
	// �r���[�ϊ��s���쐬
	m_ViewMatrix = DirectX::XMMatrixLookAtLH(pos, pos + m_Target, m_Up);// ���n
	// �V�F�[�_�ɑ���
	Renderer::SetViewMatrix(&m_ViewMatrix);

	// �v���W�F�N�V�����s��̐���
	float aspectRatio = m_aspect.x / m_aspect.y;// �A�X�y�N�g��

	// �v���W�F�N�V�����s��̐���
	
	m_ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(m_fFieldOfView, aspectRatio, m_fNearPlane, m_fFarPlane);// ����n
	
	Renderer::SetProjectionMatrix(&m_ProjectionMatrix);	
}

//-----------------------------------------------------------------------------
// GUI�`�揈��
//-----------------------------------------------------------------------------
void Camera::GUIDraw()
{
	Vector3 pos = m_pGameObject->GetPosition() + m_OffsetPos;
	float aspectRatio = m_aspect.x / m_aspect.y;// �A�X�y�N�g��
	ImGui::Text("Position X:%.1f Y:%.1f Z:%.1f", pos.x, pos.y, pos.z);
	ImGui::Text("Target X:%.1f Y:%.1f Z:%.1f", m_Target.x + pos.x, m_Target.y + pos.y, m_Target.z + pos.z);
	ImGui::Text("Aspect:%f", aspectRatio);
	ImGui::Text("Fov:%f", m_fFieldOfView);
	ImGui::Text("Near:%f", m_fNearPlane);
	ImGui::Text("Far:%f", m_fFarPlane);
}

//-----------------------------------------------------------------------------
// �J�����̈ʒu(EyePosition)���擾
//-----------------------------------------------------------------------------
DirectX::SimpleMath::Vector3 Camera::GetCameraPosition(void) {return m_pGameObject->GetPosition() + m_OffsetPos;}

//-----------------------------------------------------------------------------
// �t���X�^���}�g���b�N�X�̎擾
//-----------------------------------------------------------------------------
DirectX::SimpleMath::Matrix Camera::GetFrustumMatrix(void)
{
	Vector3 pos = m_pGameObject->GetPosition() + m_OffsetPos;
	// �r���[�ϊ��s���쐬
	m_ViewMatrix = DirectX::XMMatrixLookAtLH(pos, pos + m_Target, m_Up);// ���n

	// �v���W�F�N�V�����s��̐���
	float aspectRatio = m_aspect.x / m_aspect.y;// �A�X�y�N�g��

	// �v���W�F�N�V�����s��̐���

	m_ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(m_fFieldOfView, aspectRatio, m_fNearPlane, m_fFarPlane);// ����n
	return  m_ViewMatrix * m_ProjectionMatrix;
}
