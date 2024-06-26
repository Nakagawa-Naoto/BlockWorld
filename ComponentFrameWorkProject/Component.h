#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <iostream>

///////////////////////////////////////////////////////////////////////////////
// Component class
///////////////////////////////////////////////////////////////////////////////
class Component
{
    //=========================================================================
    // list of friend classes and methods.
    //=========================================================================
    /* NOTHING */

protected:
    //=========================================================================
    // protected variables.
    //=========================================================================
    class GameObject* m_pGameObject;
    bool m_bActive;
    std::string m_sName;
    //=========================================================================
    // protected methods
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

    Component() = delete;
    Component(GameObject* object) { m_pGameObject = object; m_bActive = true; }
    virtual ~Component(){}

    
    virtual void Init() {}
    virtual void Uninit() {}
    virtual void Update() {}
    virtual void Draw() {}
    virtual void GUIDraw(){}
    void SetActive(bool active) { m_bActive = active; }
    void SetName(std::string name) { m_sName = name; }
    std::string GetName(void) { return m_sName; }
};



