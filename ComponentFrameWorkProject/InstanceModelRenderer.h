#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <string>
#include <unordered_map>
#include "component.h"
#include "renderer.h"

///////////////////////////////////////////////////////////////////////////////
// InstancingModelRenderer class
///////////////////////////////////////////////////////////////////////////////
class InstancingModelRenderer : public Component
{
	//=========================================================================
	// list of friend classes and methods.
	//=========================================================================
	/* NOTHING */

private: 
	//=========================================================================
	// private variables.
	//=========================================================================
	int m_nInstanceCount;
	ID3D11Buffer* m_pInstanceBuffer;
	void* m_pData;
	MODEL* m_pModel{};
	ID3D11ShaderResourceView* m_pTexture[4];

	//=========================================================================
	// private methods
	//=========================================================================

public:
	//=========================================================================
	// public variables.
	//=========================================================================
	/* NOTHING */

	//=========================================================================
	// public methods.
	//=========================================================================
	void SetTexture(const int num,const wchar_t* FileName);
	using Component::Component;

	void Load(const char* FileName);
	void Init(const char* FileName,int num);
	void UpdateMatrix(DirectX::XMINT2 data[]);

	void Draw() override;
};


