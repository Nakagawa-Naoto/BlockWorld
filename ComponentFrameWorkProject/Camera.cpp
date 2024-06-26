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
// 初期化処理
//-----------------------------------------------------------------------------
void Camera::Init() 
{
	m_Target = Vector3(0, 0, 1);
	m_Up = Vector3(0, 1, 0);
	m_aspect.x = SCREEN_WIDTH;
	m_aspect.y = SCREEN_HEIGHT;
	m_fFieldOfView = DirectX::XMConvertToRadians(60.0f);// 視野角設定
	m_fNearPlane = 0.1f;		// ニアクリップ
	m_fFarPlane = 120.0f;	// ファークリップ

	//カメラの登録
	m_pGameObject->GetScene()->SetCamera(this);
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void Camera::Draw()
{
	Vector3 pos = m_pGameObject->GetPosition() + m_OffsetPos;
	// ビュー変換行列後作成
	m_ViewMatrix = DirectX::XMMatrixLookAtLH(pos, pos + m_Target, m_Up);// 左系
	// シェーダに送る
	Renderer::SetViewMatrix(&m_ViewMatrix);

	// プロジェクション行列の生成
	float aspectRatio = m_aspect.x / m_aspect.y;// アスペクト比

	// プロジェクション行列の生成
	
	m_ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(m_fFieldOfView, aspectRatio, m_fNearPlane, m_fFarPlane);// 左手系
	
	Renderer::SetProjectionMatrix(&m_ProjectionMatrix);	
}

//-----------------------------------------------------------------------------
// GUI描画処理
//-----------------------------------------------------------------------------
void Camera::GUIDraw()
{
	Vector3 pos = m_pGameObject->GetPosition() + m_OffsetPos;
	float aspectRatio = m_aspect.x / m_aspect.y;// アスペクト比
	ImGui::Text("Position X:%.1f Y:%.1f Z:%.1f", pos.x, pos.y, pos.z);
	ImGui::Text("Target X:%.1f Y:%.1f Z:%.1f", m_Target.x + pos.x, m_Target.y + pos.y, m_Target.z + pos.z);
	ImGui::Text("Aspect:%f", aspectRatio);
	ImGui::Text("Fov:%f", m_fFieldOfView);
	ImGui::Text("Near:%f", m_fNearPlane);
	ImGui::Text("Far:%f", m_fFarPlane);
}

//-----------------------------------------------------------------------------
// カメラの位置(EyePosition)を取得
//-----------------------------------------------------------------------------
DirectX::SimpleMath::Vector3 Camera::GetCameraPosition(void) {return m_pGameObject->GetPosition() + m_OffsetPos;}

//-----------------------------------------------------------------------------
// フラスタムマトリックスの取得
//-----------------------------------------------------------------------------
DirectX::SimpleMath::Matrix Camera::GetFrustumMatrix(void)
{
	Vector3 pos = m_pGameObject->GetPosition() + m_OffsetPos;
	// ビュー変換行列後作成
	m_ViewMatrix = DirectX::XMMatrixLookAtLH(pos, pos + m_Target, m_Up);// 左系

	// プロジェクション行列の生成
	float aspectRatio = m_aspect.x / m_aspect.y;// アスペクト比

	// プロジェクション行列の生成

	m_ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(m_fFieldOfView, aspectRatio, m_fNearPlane, m_fFarPlane);// 左手系
	return  m_ViewMatrix * m_ProjectionMatrix;
}
