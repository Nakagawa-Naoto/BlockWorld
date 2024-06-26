#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Component.h"
#include <SimpleMath.h>

///////////////////////////////////////////////////////////////////////////////
// Camera class
///////////////////////////////////////////////////////////////////////////////
class Camera : public Component
{
	//=========================================================================
	// list of friend classes and methods.
	//=========================================================================
	/* NOTHING */

private:
	//=========================================================================
	// private variables.
	//=========================================================================
	DirectX::SimpleMath::Vector2    m_aspect;		// アスペクト
	DirectX::SimpleMath::Vector3	m_OffsetPos;	// 
	DirectX::SimpleMath::Vector3	m_Target;		// 注視点 
	DirectX::SimpleMath::Vector3    m_Up{};			// 上方向
	DirectX::SimpleMath::Quaternion m_Quaternion;
	DirectX::SimpleMath::Matrix     m_ViewMatrix{}; // 
	DirectX::SimpleMath::Matrix		m_ProjectionMatrix;
	
	float m_fFieldOfView;		// 視野角設定
	float m_fNearPlane;			// ニアクリップ
	float m_fFarPlane;			// ファークリップ

	//=========================================================================
	// private methods
	//=========================================================================
	/* NOTHING */

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
	void Draw()override;
	void GUIDraw()override;
	void SetTarget(const DirectX::SimpleMath::Vector3 target) { m_Target = target; }
	void SetOffsetPosition(const DirectX::SimpleMath::Vector3 pos) { m_OffsetPos = pos; }
	void SetUp(const DirectX::SimpleMath::Vector3 up) { m_Up = up; }

	void SetAspect(const DirectX::SimpleMath::Vector2 aspect) { m_aspect = aspect; }
	void SetAspect(const float width, const float height) { m_aspect.x = width; m_aspect.y = height; }
	void SetFiledOfView(const float fov) { m_fFieldOfView = fov; }
	void SetNearPlane(const float nearp) { m_fNearPlane = nearp; }
	void SetFarPlane(const float farp) { m_fFarPlane = farp; }


	
	DirectX::SimpleMath::Vector3 GetTarget(void) {return m_Target; }
	DirectX::SimpleMath::Vector3 GetOffsetPosition(void) { return m_OffsetPos; }
	DirectX::SimpleMath::Vector3 GetCameraPosition(void);
	DirectX::SimpleMath::Vector3 GetUpVec(void) { return m_Up; }
	DirectX::SimpleMath::Vector3 GetForwardVec(void) 
	{ 
		DirectX::SimpleMath::Matrix mat = m_ViewMatrix;
		mat = mat.Invert();
		return  DirectX::SimpleMath::Vector3(mat._31, mat._32, mat._33); 
	}
	DirectX::SimpleMath::Matrix GetViewMatrix(void) { return  m_ViewMatrix; }
	DirectX::SimpleMath::Quaternion GetQuaternion(void) { return  m_Quaternion; }
	
	DirectX::SimpleMath::Vector2 GetAspect(void) { return m_aspect; }
	float GetFiledOfView(void) { return m_fFieldOfView; }
	float GetNearPlane(void) { return m_fNearPlane; }
	float GetFarPlane(void) { return m_fFarPlane; }
	
	DirectX::SimpleMath::Matrix GetFrustumMatrix(void);
	
};

