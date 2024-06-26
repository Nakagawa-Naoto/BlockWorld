//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Controller.h"
#include "Input.h"
#include "GUI.h"
#include "Time.h"
using namespace DirectX::SimpleMath;

///////////////////////////////////////////////////////////////////////////////
// Controller class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// MoveVector�ɓ��͒l��ݒ肷��
//-----------------------------------------------------------------------------
void Controller::InputMoveVector(void)
{
	m_MoveVector = Vector2::Zero;
	
	m_MoveVector.y += static_cast<int>(Input::GetKeyDown(m_sForward));
	m_MoveVector.y -= static_cast<int>(Input::GetKeyDown(m_sBack));
	m_MoveVector.x += static_cast<int>(Input::GetKeyDown(m_sLeft));
	m_MoveVector.x -= static_cast<int>(Input::GetKeyDown(m_sRight));
	m_MoveVector.Normalize();
}

//-----------------------------------------------------------------------------
// RotateCamera�ɓ��͒l��ݒ肷��
//-----------------------------------------------------------------------------
void Controller::InputRotateCamera(void)
{
	float deltaTime = Time::GetDeltaTime();
	m_RotateCamera.x = Input::GetMouseMoveX();
	m_RotateCamera.y = Input::GetMouseMoveY();
	m_Rotate += m_RotateCamera * m_fSensitive * deltaTime *-1;

	m_Rotate.x = (int)m_Rotate.x % 360 + (float)m_Rotate.x - (int)m_Rotate.x;
	m_Rotate.y = min(max(m_Rotate.y, -89.99f),89.99f);
}

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
void Controller::Init()
{
	// �L�[�ݒ�
	m_sForward = 'W';
	m_sBack = 'S';
	m_sLeft = 'A';
	m_sRight = 'D';

	m_MoveVector = Vector2::Zero;
	m_RotateCamera = Vector2::Zero;
	m_Rotate = Vector2::Zero;
}

//-----------------------------------------------------------------------------
// �X�V����
//-----------------------------------------------------------------------------
void Controller::Update()
{
	// MoveVector�ɓ��͒l��ݒ肷��
	InputMoveVector();
	// RotateCamera�ɓ��͒l��ݒ肷��
	InputRotateCamera();
	
	m_bJumpkey = Input::GetKeyDown(VK_SPACE);
	m_bSquatkey = Input::GetKeyDown(VK_SHIFT);
	m_bDashkey = Input::GetKeyDown(VK_CONTROL);
}

//-----------------------------------------------------------------------------
// GUI�`�揈��
//-----------------------------------------------------------------------------
void Controller::GUIDraw()
{
	ImGui::Text("MoveVector X:%.1f Y:%.1f", m_MoveVector.x, m_MoveVector.y);
	ImGui::Text("RotateCamera X:%.1f Y:%.1f", m_RotateCamera.x, m_RotateCamera.y);
	ImGui::Checkbox("Dash",&m_bDashkey);
	ImGui::Checkbox("Squat",&m_bSquatkey);
	ImGui::Checkbox("Jump",&m_bJumpkey);
	ImGui::SliderFloat("Sensitive", &m_fSensitive, 0.0f, 100.0f);
}