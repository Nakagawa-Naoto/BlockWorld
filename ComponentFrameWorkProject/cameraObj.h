#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SimpleMath.h>
#include "GameObject.h"

///////////////////////////////////////////////////////////////////////////////
// CameraObj class
///////////////////////////////////////////////////////////////////////////////
class CameraObj : public GameObject
{
    //=========================================================================
    // list of friend classes and methods.
    //=========================================================================
    /* NOTHING */
private:
    //=========================================================================
    // private variables.
    //=========================================================================
    DirectX::SimpleMath::Vector3    m_Target{};     // íçéãì_
    DirectX::SimpleMath::Matrix     m_ViewMatrix{}; // 
    DirectX::SimpleMath::Matrix     m_CamMat{};
    DirectX::SimpleMath::Vector3    m_MovePos{};
    DirectX::SimpleMath::Vector3    m_velocty{};
    //DirectX::SimpleMath::Quaternion m_quat{};
    
    bool m_flag = false;
    //=========================================================================
    // private methods
    //=========================================================================
    /* NOTHING */

    void FirstPerson(void);
    void ThirdtPerson(void);
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
    void Draw()override;

    void MovePos(DirectX::SimpleMath::Vector3 movePos) { m_MovePos = movePos; }
    void SetTarget(DirectX::SimpleMath::Vector3 target) { m_Target = target; }
    DirectX::SimpleMath::Vector3 GetTarget(void) { return m_Target; }
    DirectX::SimpleMath::Vector3 GetVeloctyt(void) { return m_velocty; }
    void SetFlag(const bool _flag) { m_flag = _flag; }
};

bool IsInFrustum(const DirectX::SimpleMath::Vector3& pos, const float& radius, const DirectX::SimpleMath::Matrix& matrix);