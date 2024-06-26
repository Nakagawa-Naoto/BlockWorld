#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <d3d11.h>
#include "Component.h"
#include <string>
#include <unordered_map>
#include "renderer.h"

///////////////////////////////////////////////////////////////////////////////
// Shader  class
///////////////////////////////////////////////////////////////////////////////
class Shader : public Component
{
    //=========================================================================
    // list of friend classes and methods.
    //=========================================================================
    /* NOTHING */

private:
    //=========================================================================
    // private variables.
    //=========================================================================
    
    ID3D11VertexShader*     m_pVertexShader{};
    ID3D11PixelShader*      m_pPixelShader{};
    ID3D11InputLayout*      m_pVertexLayout{};

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
    
    void Load(const char* VertexShader, const char* PixelShader, D3D11_INPUT_ELEMENT_DESC* layout,const UINT elements);
    void Uninit()override;
    void Draw()override;

};



