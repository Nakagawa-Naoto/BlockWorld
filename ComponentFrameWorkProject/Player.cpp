//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Player.h"
#include "Camera.h"
#include "Controller.h"
#include "Time.h"
#include "GUI.h"
#include "Scene.h"
#include "AABB.h"
using namespace DirectX::SimpleMath;

///////////////////////////////////////////////////////////////////////////////
// Player class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// �l�ԃ��[�h�̍X�V����
//-----------------------------------------------------------------------------
void Player::HumanModeUpdate(void)
{	
	Jump();
	WalkAndRun();
	
}

//-----------------------------------------------------------------------------
// �_���[�h�̍X�V����
//-----------------------------------------------------------------------------
void Player::GodModeUpdate(void)
{
	Jump();
	WalkAndRun();
	//Fly();
}

//-----------------------------------------------------------------------------
// �샂�[�h�̍X�V����
//-----------------------------------------------------------------------------
void Player::PhantomModeUpdate(void)
{
	Fly();
}

//-----------------------------------------------------------------------------
// �l�ԃ��[�h�̏���������
//-----------------------------------------------------------------------------
void Player::HumanModeInit(void) 
{ 
	m_pCollision->SetSimulationFlag(true);
	m_pCollision->SetGravityAcceler(9.8f);
	m_pCollision->SetVelocity(Vector3::Zero);
}
//-----------------------------------------------------------------------------
// �_���[�h�̏���������
//-----------------------------------------------------------------------------
void Player::GodModeInit(void) 
{
	m_pCollision->SetSimulationFlag(true);
	m_pCollision->SetGravityAcceler(9.8f);
	m_pCollision->SetVelocity(Vector3::Zero);
}
//-----------------------------------------------------------------------------
// �샂�[�h�̏���������
//-----------------------------------------------------------------------------
void Player::PhantomModeInit(void) 
{
	m_pCollision->SetSimulationFlag(false);
	m_pCollision->SetGravityAcceler(0.0f);
	m_pCollision->SetVelocity(Vector3::Zero);
}

//-----------------------------------------------------------------------------
// �W�����v����
//-----------------------------------------------------------------------------
void Player::Jump(void)
{
	// �����Ă��Ȃ��Ȃ炷��������
	if (!m_pCollision->GetSinkVector().y > 0) return;
	// �W�����v�L�[�������Ă��Ȃ��Ȃ甲����
	if (!m_pController->GetJumpKeyFlag())
	{
		m_fJumpKeyPushTime = 0;
		return;
	}
	if (m_fJumpKeyPushTime == 0.0f && m_Velocity.y <=c_fJumpPower)
	{
		m_pCollision->AddVelocity(Vector3::Up * c_fJumpPower);
		return;
	}

	float deltatime = Time::GetDeltaTime();	
	m_fJumpKeyPushTime += deltatime;
	if (m_fJumpKeyPushTime > 1.5f)m_fJumpKeyPushTime = 0;
	
}

//-----------------------------------------------------------------------------
// ��s����
//-----------------------------------------------------------------------------
void Player::Fly(void)
{
	// �����������擾����
	Vector3 MoveVector = HorizontalMove(c_fFrySpeed);
	// �㉺�ړ��̓���
	MoveVector.y += c_fFrySpeed * static_cast<int>(m_pController->GetJumpKeyFlag());
	MoveVector.y -= c_fFrySpeed * static_cast<int>(m_pController->GetSquatKeyFlag());
	
	m_InputVector = MoveVector;
}

//-----------------------------------------------------------------------------
// �����Ƒ��菈��
//-----------------------------------------------------------------------------
void Player::WalkAndRun(void)
{
	Vector3 MoveVec = Vector3::Zero;
	// �����L�[����������
	bool isMove = m_pController->GetMoveFlag();
	bool isDashKeyDown = m_pController->GetDashKeyFlag();
	bool isWalk = isMove && !isDashKeyDown;
	bool isRun	= isMove && isDashKeyDown;
	
	if (isWalk) MoveVec = HorizontalMove(c_fWalkSpeed);
	if (isRun) MoveVec = HorizontalMove(c_fRunSpeed);
	
	m_InputVector = MoveVec;
}

//-----------------------------------------------------------------------------
// ���������̈ړ�����
//-----------------------------------------------------------------------------
DirectX::SimpleMath::Vector3 Player::HorizontalMove(const float moveSpeed)
{
	// ���͒l�����o��
	Vector2 inputVector = m_pController->GetMoveVector();
	Vector3 MoveDirection = Vector3::Zero;
	float forwardAngle = m_pController->GetMove().x;
	const double Deg2Rad = DirectX::XM_PI / 180.0f;
	// �O����������߂�
	MoveDirection.x = cos(forwardAngle * Deg2Rad);
	MoveDirection.z = sin(forwardAngle * Deg2Rad);
	// �O������̈ړ��ʂ����߂�
	Vector3 ForwardVector = MoveDirection * inputVector.y;

	// ���E���������߂�
	float tmp = MoveDirection.x;
	MoveDirection.x = -MoveDirection.z;
	MoveDirection.z = tmp;
	// ���E�����̈ړ��ʂ����߂�
	Vector3 BesideVector = MoveDirection * inputVector.x;

	// �����𑫂����킹��
	Vector3 MoveVector = ForwardVector + BesideVector;

	return MoveVector * moveSpeed;
}

//-----------------------------------------------------------------------------
// ���_�̈ړ�����
//-----------------------------------------------------------------------------
void Player::MovePointView(void)
{
	Vector3 target = m_pCamera->GetTarget();
	Vector2 mouse = m_pController->GetMove();
	Vector3 Rotate = Vector3::Zero;
	const double Deg2Rad = DirectX::XM_PI / 180.0f;
	Rotate.x = cos(mouse.x * Deg2Rad) * cos(mouse.y * Deg2Rad);
	Rotate.y = sin(mouse.y * Deg2Rad);
	Rotate.z = sin(mouse.x * Deg2Rad) * cos(mouse.y * Deg2Rad);
	Rotate.Normalize();
	
	target = Rotate * 10.0f;

	m_pCamera->SetTarget(target);
}

//-----------------------------------------------------------------------------
// �ړ��ʂ��X�V
//-----------------------------------------------------------------------------
void Player::MoveUpdate(void)
{
	m_pCollision->SetMoveVector(m_InputVector);
}

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
void Player::Init()
{
	// �����ʒu�ݒ�
	m_Position = Vector3(16, 50, 16);
	// �R���|�[�l���g��ǉ�
	m_pCamera = AddComponent<Camera>();
	m_pController = AddComponent<Controller>();
	m_pCollision = AddComponent<AABB>();
	
	// �J��������ɂ��炷
	m_pCamera->SetOffsetPosition(Vector3(0, 1, 0));
	
	// �R���W�����̃T�C�Y�ݒ�
	m_pCollision->Init(Vector3(0.5f, 1.0f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	m_pCollision->SetSimulationFlag(true);
	m_pCollision->SetGravityAcceler(9.8f);

	// �e���[�h��ݒ肷��
	// ������
	m_PlayModeInit[(int)PlayMode::Human] = [this](void) {HumanModeInit(); };
	m_PlayModeInit[(int)PlayMode::God] = [this](void) {GodModeInit(); };
	m_PlayModeInit[(int)PlayMode::Phantom] = [this](void) {PhantomModeInit(); };
	// �X�V
	m_PlayModeUpdate[(int)PlayMode::Human] = [this](void) {HumanModeUpdate(); };
	m_PlayModeUpdate[(int)PlayMode::God] = [this](void) {GodModeUpdate(); };
	m_PlayModeUpdate[(int)PlayMode::Phantom] = [this](void) {PhantomModeUpdate(); };

	m_nMode = (int)PlayMode::Human;
}

//-----------------------------------------------------------------------------
// �I������
//-----------------------------------------------------------------------------
void Player::Uninit()
{
	m_pCamera = nullptr;
	m_pController = nullptr;
	m_pCollision = nullptr;
}

//-----------------------------------------------------------------------------
// �X�V����
//-----------------------------------------------------------------------------
void Player::Update()
{	
	// ���[�h�ύX���ɏ���������
	if(m_nMode != m_nOldMode)m_PlayModeInit[(int)m_nMode]();
	// ���[�h�̍X�V����
	m_PlayModeUpdate[(int)m_nMode]();
	// �J�����̍X�V����
	MovePointView();
	MoveUpdate();
	m_nOldMode = m_nMode;
}

//-----------------------------------------------------------------------------
// GUI�`�揈��
//-----------------------------------------------------------------------------
void Player::GUIDraw()
{
	ImGui::Text("Velocity x:%.1f y:%.1f z:%.1f",m_Velocity.x, m_Velocity.y, m_Velocity.z);
	ImGui::Text("InputVec x:%.1f y:%.1f z:%.1f",m_InputVector.x, m_InputVector.y, m_InputVector.z);
	
	if(ImGui::TreeNode("PlayMode"))
	{
		ImGui::RadioButton("Human", &m_nMode, (int)PlayMode::Human);
		ImGui::SameLine();
		ImGui::RadioButton("Phantom", &m_nMode, (int)PlayMode::Phantom);
		ImGui::TreePop();
	}
	
	
	if(ImGui::Button("ResetPosition",ImVec2(100,25)))m_Position = Vector3(16, 50, 16);
	if(ImGui::Button("UP",ImVec2(100,25)))m_Position += Vector3(0, 1, 0);
	
}
