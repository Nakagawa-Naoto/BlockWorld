
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main.h"
#include "Manager.h"
#include "renderer.h"
#include "cameraObj.h"
#include "Frame.h"
#include "Input.h"
#include "GUI.h"

using namespace DirectX::SimpleMath;

///////////////////////////////////////////////////////////////////////////////
// Manager class
///////////////////////////////////////////////////////////////////////////////

void CameraObj::FirstPerson(void)
{
	{
		float y = 0.0f;
		float z = 0.0f;
		float x = 0.0f;
		if (Input::GetKeyDown('W')) { z = 1.0f; }
		if (Input::GetKeyDown('S')) { z = -1.0f; }
		if (Input::GetKeyDown('D')) { x = 1.0f; }
		if (Input::GetKeyDown('A')) { x = -1.0f; }


		if (Input::GetKeyDown(VK_SPACE))y = 1.0f;
		if (Input::GetKeyDown(VK_SHIFT))y = -1.0f;
		m_Target = Vector3(m_CamMat._31, m_CamMat._32, m_CamMat._33) + m_Position;

		// 
		Vector3 m = Vector3(m_CamMat._33, 0, -m_CamMat._31);
		Vector3 foward = Vector3(m_CamMat._31, 0, m_CamMat._33);

		foward.Normalize();
		m.Normalize();

		m_velocty = foward * z;
		m_velocty += m * x;
		m_velocty.y = y;

		m_velocty *= 10.0f * Frame::GetDeltaTime();

		m_Position += m_velocty;

	}
	{
		//int x = Input::GetMouseMoveX();
		//int y = Input::GetMouseMoveY();
		int x = 0;
		int y = 0;
		if (Input::GetKeyDown(VK_UP))y -= 5;
		if (Input::GetKeyDown(VK_DOWN))y += 5;
		if (Input::GetKeyDown(VK_LEFT))x -= 5;
		if (Input::GetKeyDown(VK_RIGHT))x += 5;

		float yaw = y * 0.5f * Frame::GetDeltaTime();
		float pitch = x * 0.5f * Frame::GetDeltaTime();
		m_Rotation.x += yaw;
		m_Rotation.y += pitch;
		m_Rotation.x = (m_Rotation.x > 1.5f ? 1.5f : (m_Rotation.x < -1 ? -1 : m_Rotation.x));
		m_Rotation.y = fmod(m_Rotation.y, 6.2831f);
		m_CamMat = Matrix::CreateFromYawPitchRoll(m_Rotation);
		m_CamMat *= Matrix::CreateTranslation(m_Position);
	}
}

void CameraObj::ThirdtPerson(void)
{
	{
	
		m_Position.Normalize();
		
		m_velocty.x += 0.005f;
		float len = 20;
		m_Target = Vector3(8, 8, 8);

		m_Position = m_Target + Vector3(sin(m_velocty.x) * len, -4, cos(m_velocty.x) * len);
	}
}

//-----------------------------------------------------------------------------
//      初期化処理
//-----------------------------------------------------------------------------
void CameraObj::Init()
{
	m_velocty = Vector3(0.0f, 0.0f, 0.0f);
	m_Position = Vector3(0.0f,4.0f, 0.0f);
	m_Rotation = Vector3(0.0f, 0.0f, 0.0f);
	m_Target = Vector3(0.0f, 0.0f, 0.0f);
	m_CamMat = DirectX::XMMatrixIdentity();
	m_CamMat._11 = 1;
	m_CamMat._22 = 1;
	m_CamMat._33 = 1;
}

//-----------------------------------------------------------------------------
//      終了処理
//-----------------------------------------------------------------------------
void CameraObj::Uninit()
{
}

//-----------------------------------------------------------------------------
//      更新処理
//-----------------------------------------------------------------------------
void CameraObj::Update()
{
	if(m_flag)
	{
		ThirdtPerson();
	}
	else
	{
		FirstPerson();
	}
	

	/*ImGui::BeginChild("Camera",ImVec2(300,100),true,0);
	ImGui::Text("Position X:%.1f Y:%.1f Z:%.1f", m_Position.x, m_Position.y, m_Position.z);
	ImGui::Text("Rotate X:%.2f Y:%.2f Z:%.2f", m_Rotation.x, m_Rotation.y, m_Rotation.z);
	ImGui::Text("Target X:%.1f Y:%.1f Z:%.1f", m_Target.x, m_Target.y, m_Target.z);
	ImGui::EndChild();*/
}

//-----------------------------------------------------------------------------
//      描画処理
//-----------------------------------------------------------------------------
void CameraObj::Draw()
{
	// ビュー変換行列後作成
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	m_ViewMatrix = DirectX::XMMatrixLookAtLH(m_Position, m_Target, up);// 左系
	if (!m_flag)m_CamMat.Invert(m_ViewMatrix);
	
	
	// シェーダに送る
	Renderer::SetViewMatrix(&m_ViewMatrix);

	// プロジェクション行列の生成
	constexpr float fieldOfView = DirectX::XMConvertToRadians(60.0f);// 視野角設定
	float aspectRatio = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);// アスペクト比
	float nearPlane = 1.0f;		// ニアクリップ
	float farPlane = 1000.0f;	// ファークリップ

	// プロジェクション行列の生成
	Matrix projectionMatrix;
	projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearPlane, farPlane);// 左手系

	Renderer::SetProjectionMatrix(&projectionMatrix);
}

void GetPlanefromProjectionmatrix(Vector4* p,const Matrix& mat)
{
	//left
	p[0].x = mat._14 + mat._11;
	p[0].y = mat._24 + mat._21;
	p[0].z = mat._34 + mat._31;
	p[0].w = mat._44 + mat._41;

	// right
	p[1].x = mat._14 - mat._11;
	p[1].y = mat._24 - mat._21;
	p[1].z = mat._34 - mat._31;
	p[1].w = mat._44 - mat._41;

	// bottom
	p[2].x = mat._14 + mat._12;
	p[2].y = mat._24 + mat._22;
	p[2].z = mat._34 + mat._32;
	p[2].w = mat._44 + mat._42;

	// top
	p[3].x = mat._14 - mat._12;
	p[3].y = mat._24 - mat._22;
	p[3].z = mat._34 - mat._32;
	p[3].w = mat._44 - mat._42;

	// near
	p[4].x = mat._13;
	p[4].y = mat._23;
	p[4].z = mat._33;
	p[4].w = mat._43;

	// far
	p[5].x = mat._14 - mat._13;
	p[5].y = mat._24 - mat._23;
	p[5].z = mat._34 - mat._33;
	p[5].w = mat._44 - mat._43;
}
void GetPlanefromProjectionmatrixWithNormalize(Vector4* p, const Matrix& mat)
{
	//left
	p[0].x = mat._14 + mat._11;
	p[0].y = mat._24 + mat._21;
	p[0].z = mat._34 + mat._31;
	p[0].w = mat._44 + mat._41;

	// right
	p[1].x = mat._14 - mat._11;
	p[1].y = mat._24 - mat._21;
	p[1].z = mat._34 - mat._31;
	p[1].w = mat._44 - mat._41;

	// bottom
	p[2].x = mat._14 + mat._12;
	p[2].y = mat._24 + mat._22;
	p[2].z = mat._34 + mat._32;
	p[2].w = mat._44 + mat._42;

	// top
	p[3].x = mat._14 - mat._12;
	p[3].y = mat._24 - mat._22;
	p[3].z = mat._34 - mat._32;
	p[3].w = mat._44 - mat._42;

	// near
	p[4].x = mat._13;
	p[4].y = mat._23;
	p[4].z = mat._33;
	p[4].w = mat._43;

	// far
	p[5].x = mat._14 - mat._13;
	p[5].y = mat._24 - mat._23;
	p[5].z = mat._34 - mat._33;
	p[5].w = mat._44 - mat._43;

	// Normalize
	for (int i = 0; i < 6; ++i)p[i].Normalize();
	
}

// 点とフラスタム
bool IsInFrustum(const Vector3& pos,const Matrix& matrix)
{
	Vector4 plane[6];
	GetPlanefromProjectionmatrix(plane, matrix);

	for (int i = 0; i < _countof(plane); ++i) 
	{
		float ans = plane[i].x * pos.x + plane[i].y * pos.y + plane[i].z * pos.z + plane[i].w;
		if (ans < 0)return false;
	}
	return true;
}

// 球とフラスタム
bool IsInFrustum(const Vector3& pos, const float& radius,const Matrix& matrix)
{
	Vector4 plane[6];
	GetPlanefromProjectionmatrix(plane, matrix);
	for (int i = 0; i < _countof(plane); ++i)
	{
		// 正規化
		plane[i].Normalize();
		// 内積
		float ans = plane[i].x * pos.x + plane[i].y * pos.y + plane[i].z * pos.z + plane[i].w;
		if (ans + radius < 0)return false;
	}

	return true;
}