#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Component.h"
#include <SimpleMath.h>

///////////////////////////////////////////////////////////////////////////////
// AABB class
///////////////////////////////////////////////////////////////////////////////
class AABB : public Component
{
	//=========================================================================
	// list of friend classes and methods.
	//=========================================================================
	/* NOTHING */
private:
	//=========================================================================
	// private variables.
	//=========================================================================
	DirectX::SimpleMath::Vector3 m_Offset;
	DirectX::SimpleMath::Vector3 m_Sink;	// めり込んだベクトル
	DirectX::SimpleMath::Vector3 m_Max;	
	DirectX::SimpleMath::Vector3 m_Min;	
	DirectX::SimpleMath::Vector3 m_Velocity;	
	DirectX::SimpleMath::Vector3 m_MoveVec;
	float m_fGravityAcceler;
	bool m_bHit;
	bool m_bSimulation;

	//=========================================================================
	// private methods
	//=========================================================================
	void Gravity(void);

public:
	//=========================================================================
	// public variables.
	//=========================================================================
	/* NOTHING */

	//=========================================================================
	// public methods.
	//=========================================================================
	void Reset(void);
	using Component::Component;
	void Init(DirectX::SimpleMath::Vector3 max, DirectX::SimpleMath::Vector3 min);
	void Update()override;
	void GUIDraw()override;
	bool IsHit(AABB* aabb);

	void SetMaxVector(DirectX::SimpleMath::Vector3 max) { m_Max = max;}
	void SetMinVector(DirectX::SimpleMath::Vector3 min) { m_Min = min;}
	void SetOffset(DirectX::SimpleMath::Vector3 offset) { m_Offset = offset;}
	void ResetSinkVector(void) { m_Sink = DirectX::SimpleMath::Vector3::Zero;}
	void SetGravityAcceler(const float accleer) { m_fGravityAcceler = accleer; }
	void SetSimulationFlag(const bool sim) { m_bSimulation = sim; }
	void SetVelocity(DirectX::SimpleMath::Vector3 Velocity) { m_Velocity = Velocity; }
	void AddVelocity(DirectX::SimpleMath::Vector3 Velocity) { m_Velocity += Velocity; }
	void SetMoveVector(DirectX::SimpleMath::Vector3 moveVec) { m_MoveVec += moveVec; }
	DirectX::SimpleMath::Vector3 GetVelocity(void) { return m_Velocity; }

	// めり込んだベクトルを取得
	DirectX::SimpleMath::Vector3 GetSinkVector(void) { return m_Sink; }
	// 反発ベクトルを取得
	DirectX::SimpleMath::Vector3 GetRepulsionVector(void) { return m_Sink * -1; }
	bool GetHit(void) { return m_bHit; }
	
}; 

