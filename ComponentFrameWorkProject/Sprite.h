#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "renderer.h"
#include "Component.h"


///////////////////////////////////////////////////////////////////////////////
// Component class
///////////////////////////////////////////////////////////////////////////////
class Sprite : public Component
{
    //=========================================================================
    // list of friend classes and methods.
    //=========================================================================
    /* NOTHING */

private:
    //=========================================================================
    // private variables.
    //=========================================================================
    ID3D11Buffer* m_pVertexBuffer{};
    ID3D11ShaderResourceView* m_pTexture{};
    ID3D11Resource* m_pTextureResource{};

    MATERIAL m_Material{};
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

    void Init(int x, int y, int Width,int Height, const wchar_t* TextureName);
    void Uninit()override;
    void Draw()override;

    void SetMaterial(MATERIAL Material) { m_Material = Material; }

};