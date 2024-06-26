//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "AABB.h"
#include "GameObject.h"
#include "Time.h"

using namespace DirectX::SimpleMath;

///////////////////////////////////////////////////////////////////////////////
// AABB class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// 重力処理
//-----------------------------------------------------------------------------
void AABB::Gravity(void)
{
	float deltatime = Time::GetDeltaTime();
	// 重力
	Vector3 gravityVec = Vector3(0, -1, 0);
	m_Velocity += gravityVec * m_fGravityAcceler * deltatime;
}
//-----------------------------------------------------------------------------
// リセット
//-----------------------------------------------------------------------------
void AABB::Reset(void)
{
	m_Sink = Vector3::Zero;
	m_bHit = false;
}
//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void AABB::Init(DirectX::SimpleMath::Vector3 max, DirectX::SimpleMath::Vector3 min)
{
	m_Max = max; m_Min = min; 
	m_Velocity =Vector3::Zero;
	m_MoveVec =Vector3::Zero;
	m_bSimulation = false;
	m_fGravityAcceler = 9.8f;
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void AABB::Update()
{
	float deltatime = Time::GetDeltaTime();
	m_pGameObject->SetPosition(m_pGameObject->GetPosition() + (m_Velocity + m_MoveVec) * deltatime);
	if(m_bSimulation)Gravity();
	m_MoveVec = Vector3::Zero;
}

//-----------------------------------------------------------------------------
// GUI描画処理
//-----------------------------------------------------------------------------
void AABB::GUIDraw()
{
	ImGui::Text("Velocity x:%.1f y:%.1f z:%.1f", m_Velocity.x, m_Velocity.y, m_Velocity.z);
	ImGui::Checkbox("hit", &m_bHit);
}
//-----------------------------------------------------------------------------
// あたり判定AABB同士
//-----------------------------------------------------------------------------
bool AABB::IsHit(AABB * aabb)
{
	bool result = false;

	Vector3 thisMax = m_pGameObject->GetPosition() + m_Offset + m_Max;
	Vector3 thisMin = m_pGameObject->GetPosition() + m_Offset + m_Min;

	Vector3 otherMax = aabb->m_pGameObject->GetPosition() + aabb->m_Offset + aabb->m_Max;
	Vector3 otherMin = aabb->m_pGameObject->GetPosition() + aabb->m_Offset + aabb->m_Min;
	
	bool x_axis = thisMax.x > otherMin.x && thisMin.x < otherMax.x;
	bool y_axis = thisMax.y > otherMin.y && thisMin.y < otherMax.y;
	bool z_axis = thisMax.z > otherMin.z && thisMin.z < otherMax.z;
	result = x_axis && y_axis && z_axis;
	// 当たっていないなら終わる
	if (!result)return result;
	// めり込んだベクトルを求める
	Vector3 MaxMin = thisMax - otherMin;
	Vector3 MinMax = thisMin - otherMax;
	Vector3 Sink = (MaxMin.Length() > MinMax.Length() ? MinMax : MaxMin);
	float x = (Sink.x * Sink.x < Sink.y * Sink.y && Sink.x * Sink.x < Sink.z * Sink.z ? Sink.x : 0);
	float y = (Sink.y * Sink.y < Sink.x * Sink.x && Sink.y * Sink.y < Sink.z * Sink.z ? Sink.y : 0);
	float z = (Sink.z * Sink.z < Sink.x * Sink.x && Sink.z * Sink.z < Sink.y * Sink.y ? Sink.z : 0);
	
	
	Sink = Vector3(x, y, z);
	// 価の小さい方ベクトルを返す
	m_Sink += Sink;
	aabb->m_Sink += Sink * -1;
	
	if(m_bSimulation)
	{
		// めり込みを防ぐ
		Vector3 pos = m_pGameObject->GetPosition();
		pos -= Sink;
		
		m_pGameObject->SetPosition(pos);
		// 速度をリセットする
		m_Velocity.x *= (x == 0 ? 1 : 0);
		m_Velocity.y *= (y == 0 ? 1 : 0);
		m_Velocity.z *= (z == 0 ? 1 : 0);
		m_MoveVec.x *= (x == 0 ? 1 : 0);
		m_MoveVec.y *= (y == 0 ? 1 : 0);
		m_MoveVec.z *= (z == 0 ? 1 : 0);
		m_Velocity -= Sink;
	}
	 if(aabb->m_bSimulation)
	 {
		 // めり込みを防ぐ
		 Vector3 pos = aabb->m_pGameObject->GetPosition();
		 pos += Sink ;
		 aabb->m_pGameObject->SetPosition(pos);
		 // 速度をリセットする
		 aabb->m_Velocity.x *= (x == 0 ? 1 : 0);
		 aabb->m_Velocity.y *= (y == 0 ? 1 : 0);
		 aabb->m_Velocity.z *= (z == 0 ? 1 : 0);
		 aabb->m_MoveVec.x *= (x == 0 ? 1 : 0);
		 aabb->m_MoveVec.y *= (y == 0 ? 1 : 0);
		 aabb->m_MoveVec.z *= (z == 0 ? 1 : 0);
	 }

	 
	 
	// 結果を設定する
	m_bHit = result;
	aabb->m_bHit = result;
	// 終わる
	return result;
}
