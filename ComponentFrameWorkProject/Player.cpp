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
// 人間モードの更新処理
//-----------------------------------------------------------------------------
void Player::HumanModeUpdate(void)
{	
	Jump();
	WalkAndRun();
	
}

//-----------------------------------------------------------------------------
// 神モードの更新処理
//-----------------------------------------------------------------------------
void Player::GodModeUpdate(void)
{
	Jump();
	WalkAndRun();
	//Fly();
}

//-----------------------------------------------------------------------------
// 霊モードの更新処理
//-----------------------------------------------------------------------------
void Player::PhantomModeUpdate(void)
{
	Fly();
}

//-----------------------------------------------------------------------------
// 人間モードの初期化処理
//-----------------------------------------------------------------------------
void Player::HumanModeInit(void) 
{ 
	m_pCollision->SetSimulationFlag(true);
	m_pCollision->SetGravityAcceler(9.8f);
	m_pCollision->SetVelocity(Vector3::Zero);
}
//-----------------------------------------------------------------------------
// 神モードの初期化処理
//-----------------------------------------------------------------------------
void Player::GodModeInit(void) 
{
	m_pCollision->SetSimulationFlag(true);
	m_pCollision->SetGravityAcceler(9.8f);
	m_pCollision->SetVelocity(Vector3::Zero);
}
//-----------------------------------------------------------------------------
// 霊モードの初期化処理
//-----------------------------------------------------------------------------
void Player::PhantomModeInit(void) 
{
	m_pCollision->SetSimulationFlag(false);
	m_pCollision->SetGravityAcceler(0.0f);
	m_pCollision->SetVelocity(Vector3::Zero);
}

//-----------------------------------------------------------------------------
// ジャンプ処理
//-----------------------------------------------------------------------------
void Player::Jump(void)
{
	// 立っていないならすぐ抜ける
	if (!m_pCollision->GetSinkVector().y > 0) return;
	// ジャンプキーを押していないなら抜ける
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
// 飛行処理
//-----------------------------------------------------------------------------
void Player::Fly(void)
{
	// 水平方向を取得する
	Vector3 MoveVector = HorizontalMove(c_fFrySpeed);
	// 上下移動の入力
	MoveVector.y += c_fFrySpeed * static_cast<int>(m_pController->GetJumpKeyFlag());
	MoveVector.y -= c_fFrySpeed * static_cast<int>(m_pController->GetSquatKeyFlag());
	
	m_InputVector = MoveVector;
}

//-----------------------------------------------------------------------------
// 歩きと走り処理
//-----------------------------------------------------------------------------
void Player::WalkAndRun(void)
{
	Vector3 MoveVec = Vector3::Zero;
	// 動くキーを押したか
	bool isMove = m_pController->GetMoveFlag();
	bool isDashKeyDown = m_pController->GetDashKeyFlag();
	bool isWalk = isMove && !isDashKeyDown;
	bool isRun	= isMove && isDashKeyDown;
	
	if (isWalk) MoveVec = HorizontalMove(c_fWalkSpeed);
	if (isRun) MoveVec = HorizontalMove(c_fRunSpeed);
	
	m_InputVector = MoveVec;
}

//-----------------------------------------------------------------------------
// 水平方向の移動処理
//-----------------------------------------------------------------------------
DirectX::SimpleMath::Vector3 Player::HorizontalMove(const float moveSpeed)
{
	// 入力値を取り出す
	Vector2 inputVector = m_pController->GetMoveVector();
	Vector3 MoveDirection = Vector3::Zero;
	float forwardAngle = m_pController->GetMove().x;
	const double Deg2Rad = DirectX::XM_PI / 180.0f;
	// 前後方向を求める
	MoveDirection.x = cos(forwardAngle * Deg2Rad);
	MoveDirection.z = sin(forwardAngle * Deg2Rad);
	// 前後方向の移動量を求める
	Vector3 ForwardVector = MoveDirection * inputVector.y;

	// 左右方向を求める
	float tmp = MoveDirection.x;
	MoveDirection.x = -MoveDirection.z;
	MoveDirection.z = tmp;
	// 左右方向の移動量を求める
	Vector3 BesideVector = MoveDirection * inputVector.x;

	// 方向を足し合わせる
	Vector3 MoveVector = ForwardVector + BesideVector;

	return MoveVector * moveSpeed;
}

//-----------------------------------------------------------------------------
// 視点の移動処理
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
// 移動量を更新
//-----------------------------------------------------------------------------
void Player::MoveUpdate(void)
{
	m_pCollision->SetMoveVector(m_InputVector);
}

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void Player::Init()
{
	// 初期位置設定
	m_Position = Vector3(16, 50, 16);
	// コンポーネントを追加
	m_pCamera = AddComponent<Camera>();
	m_pController = AddComponent<Controller>();
	m_pCollision = AddComponent<AABB>();
	
	// カメラを上にずらす
	m_pCamera->SetOffsetPosition(Vector3(0, 1, 0));
	
	// コリジョンのサイズ設定
	m_pCollision->Init(Vector3(0.5f, 1.0f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	m_pCollision->SetSimulationFlag(true);
	m_pCollision->SetGravityAcceler(9.8f);

	// 各モードを設定する
	// 初期化
	m_PlayModeInit[(int)PlayMode::Human] = [this](void) {HumanModeInit(); };
	m_PlayModeInit[(int)PlayMode::God] = [this](void) {GodModeInit(); };
	m_PlayModeInit[(int)PlayMode::Phantom] = [this](void) {PhantomModeInit(); };
	// 更新
	m_PlayModeUpdate[(int)PlayMode::Human] = [this](void) {HumanModeUpdate(); };
	m_PlayModeUpdate[(int)PlayMode::God] = [this](void) {GodModeUpdate(); };
	m_PlayModeUpdate[(int)PlayMode::Phantom] = [this](void) {PhantomModeUpdate(); };

	m_nMode = (int)PlayMode::Human;
}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void Player::Uninit()
{
	m_pCamera = nullptr;
	m_pController = nullptr;
	m_pCollision = nullptr;
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void Player::Update()
{	
	// モード変更時に初期化する
	if(m_nMode != m_nOldMode)m_PlayModeInit[(int)m_nMode]();
	// モードの更新処理
	m_PlayModeUpdate[(int)m_nMode]();
	// カメラの更新処理
	MovePointView();
	MoveUpdate();
	m_nOldMode = m_nMode;
}

//-----------------------------------------------------------------------------
// GUI描画処理
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
