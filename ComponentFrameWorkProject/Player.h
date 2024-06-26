#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "GameObject.h"
#include <functional>

//-----------------------------------------------------------------------------
// enum class
//-----------------------------------------------------------------------------
enum class PlayMode : int
{
	Human = 0,
	God,
	Phantom,
	MAX,
};
///////////////////////////////////////////////////////////////////////////////
// Player class
///////////////////////////////////////////////////////////////////////////////
class Player : public GameObject
{	
	//=========================================================================
	// list of friend classes and methods.
	//=========================================================================
	/* NOTHING */
private:
	//=========================================================================
	// private variables.
	//=========================================================================
	class Camera* m_pCamera;
	class Controller* m_pController;
	class AABB* m_pCollision;
	
	int m_nMode;
	int m_nOldMode;
	DirectX::SimpleMath::Vector3 m_Velocity;
	DirectX::SimpleMath::Vector3 m_InputVector;
	float m_fJumpKeyPushTime = 1.0f;
	

	// 各モードの処理を持つ,functionクラス
	std::function<void(void)> m_PlayModeInit[(int)PlayMode::MAX];
	std::function<void(void)> m_PlayModeUpdate[(int)PlayMode::MAX];
	const float c_fWalkSpeed = 3.0f;
	const float c_fRunSpeed = 5.0f;
	const float c_fFrySpeed = 10.0f;
	const float c_fJumpPower = 5.0f;
	

	//=========================================================================
	// private methods
	//=========================================================================
	
	void HumanModeUpdate(void);
	void GodModeUpdate(void);
	void PhantomModeUpdate(void);

	void HumanModeInit(void);
	void GodModeInit(void);
	void PhantomModeInit(void);

	void Jump(void);
	void Fly(void);
	void WalkAndRun(void);
	void Repulsion(void);
	DirectX::SimpleMath::Vector3 HorizontalMove(const float moveSpeed);
	// カメラの向きを変える
	void MovePointView(void);
	void MoveUpdate(void);
	
	void ModeChange(PlayMode mode);
public:
	
	//=========================================================================
	// public variables.
	//=========================================================================
	/* NOTHING */

	//=========================================================================
	// public methods.
	//=========================================================================
	void Init()override;
	void Uninit()override;
	void Update()override;
	void GUIDraw()override;
	AABB* GetCollision(void) { return m_pCollision; }
	
	
};

