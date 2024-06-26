#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Component.h"
#include "SimpleMath.h"

///////////////////////////////////////////////////////////////////////////////
// Controller class
///////////////////////////////////////////////////////////////////////////////
class Controller : public Component
{
	//=========================================================================
	// list of friend classes and methods.
	//=========================================================================
	/* NOTHING */

private:
	//=========================================================================
	// private variables.
	//=========================================================================
	int m_sForward,m_sBack,m_sRight,m_sLeft;
	DirectX::SimpleMath::Vector2 m_MoveVector;
	DirectX::SimpleMath::Vector2 m_RotateCamera; 
	DirectX::SimpleMath::Vector2 m_Rotate;
	bool m_bDashkey;
	bool m_bJumpkey;
	bool m_bSquatkey;
	float m_fSensitive = 40.0f;

	//=========================================================================
	// private methods
	//=========================================================================
	// MoveVectorÇê›íËÇ∑ÇÈ
	void InputMoveVector(void);
	void InputRotateCamera(void);
public:
	//=========================================================================
	// public variables.
	//=========================================================================
	/* NOTHING */

	//=========================================================================
	// public methods.
	//=========================================================================
	using Component::Component;
	void Init()override;
	void Update()override;
	void GUIDraw()override;

	bool GetMoveFlag(void) { return m_MoveVector.Length() != 0; }
	bool GetDashKeyFlag(void) { return m_bDashkey; }
	bool GetMoveMouseFlag(void) { return m_RotateCamera.Length() != 0; }
	bool GetJumpKeyFlag(void) { return m_bJumpkey; }
	bool GetSquatKeyFlag(void) { return m_bSquatkey; }
	DirectX::SimpleMath::Vector2 GetMoveVector(void) { return m_MoveVector; }
	DirectX::SimpleMath::Vector2 GetMoveMouse(void) { return m_RotateCamera; }
	DirectX::SimpleMath::Vector2 GetMove(void) { return m_Rotate; }


};

