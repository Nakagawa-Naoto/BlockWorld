//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Shader.h"
#include "modelRenderer.h"
#include "Box.h"



///////////////////////////////////////////////////////////////////////////////
// Box class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      èâä˙âªèàóù
//-----------------------------------------------------------------------------

void Box::Init()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0,		D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	4 * 3,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	4 * 6,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0,	4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	AddComponent<Shader>()->Load("asset/shader/vertexLightingVS.cso", "asset/shader/vertexLightingPS.cso", layout, ARRAYSIZE(layout));
	AddComponent <ModelRenderer>()->Load("asset\\model\\cube.obj");
}
