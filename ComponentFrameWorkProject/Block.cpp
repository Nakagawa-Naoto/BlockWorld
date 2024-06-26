//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Shader.h"
#include "modelRenderer.h"
#include "Block.h"
#include "AABB.h"
#include "Time.h"

using namespace DirectX::SimpleMath;

///////////////////////////////////////////////////////////////////////////////
// Block class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// èâä˙âªèàóù
//-----------------------------------------------------------------------------
void Block::Init()
{
	m_pShader = AddComponent<Shader>();
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0,		D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	4 * 3,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	4 * 6,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0,	4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	m_pShader->Load("asset/shader/vertexLightingVS.cso", "asset/shader/vertexLightingPS.cso",layout,ARRAYSIZE(layout));
	m_pModel = AddComponent <ModelRenderer>();
	m_pModel->Load("asset\\model\\block\\cube.obj");
	m_pCollision = AddComponent<AABB>();
	m_pCollision->Init(DirectX::SimpleMath::Vector3(0.5f, 0.5f, 0.5f), DirectX::SimpleMath::Vector3(-0.5f, -0.5f, -0.5f));
	m_pCollision->SetSimulationFlag(true);
	m_Position = Vector3(16, 50, 16);
	
}

//-----------------------------------------------------------------------------
// çXêVèàóù
//-----------------------------------------------------------------------------
void Block::Update()
{
	if (m_Position.y < -1)  m_Position = Vector3(16, 50, 16);
}

//-----------------------------------------------------------------------------
// èIóπèàóù
//-----------------------------------------------------------------------------
void Block::Uninit()
{
	
	m_pShader = nullptr;
	m_pModel=nullptr;
	m_pCollision = nullptr;
}

//-----------------------------------------------------------------------------
// GUIï`âÊèàóù
//-----------------------------------------------------------------------------
void Block::GUIDraw()
{
	if (ImGui::Button("ResetPosition", ImVec2(100, 25)))m_Position = Vector3(16, 50, 16);
	if (ImGui::Button("UP", ImVec2(100, 25)))m_Position += Vector3(0, 2, 0);
}
