#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include"main.h"
#include <map>
#include <string>
#include <Xinput.h>
//------------------------------------
//		定数定義 
//------------------------------------
constexpr int KEY_MAX = 256;

///////////////////////////////////////////////////////////////////////////////
// Input class
///////////////////////////////////////////////////////////////////////////////
class Input
{
    //=========================================================================
    // list of friend classes and methods.
    //=========================================================================
    /* NOTHING */
private:
    //=========================================================================
    // private variables.
    //=========================================================================
    bool m_bKeyState[KEY_MAX];// 現在のキーボードの状態
    bool m_bOldKeyState[KEY_MAX];// 過去のキーボードの状態
    int m_nKeyCount;//気ボードの押された数
    int m_nMousex, m_nMousey;
    int m_nMouseOldx, m_nMouseOldy;
    static Input* s_pInput;


    //=========================================================================
    // private methods
    //=========================================================================
    Input() = default;
    ~Input() = default;
    
    static void Refresh(void) {
        memcpy_s(s_pInput->m_bOldKeyState, sizeof(s_pInput->m_bOldKeyState), s_pInput->m_bKeyState, sizeof(s_pInput->m_bKeyState));
        s_pInput->m_nKeyCount = 0;// リセット
        s_pInput->m_nMouseOldx = s_pInput->m_nMousex;
        s_pInput->m_nMouseOldy = s_pInput->m_nMousey;
        // 押された数を数える
        for (int i = 0; i < KEY_MAX; i++)s_pInput->m_nKeyCount += (s_pInput->m_bKeyState[i] == true ? 1 : 0);
        
    }

public:

    //=========================================================================
    // public variables.
    //=========================================================================
    /* NOTHING */

    //=========================================================================
    // public methods.
    //=========================================================================
    /* NOTHING */

    static void Init(void) { if (s_pInput == nullptr)s_pInput = new Input(); }
    static void Uninit(void) { if (s_pInput != nullptr)delete s_pInput; }
    static void Update(void) { Refresh(); }

    static void SetKeyDown(int key) { s_pInput->m_bKeyState[key] = true; }
    static void SetKeyUp(int key) { s_pInput->m_bKeyState[key] = false; }
    static void SetMousePos(int x, int y) { s_pInput->m_nMousex = x; s_pInput->m_nMousey = y; }

    static bool GetKeyDown(int key) { return s_pInput->m_bKeyState[key]; }
    static bool GetKeyTrigger(int key) { return s_pInput->m_bKeyState[key] && !s_pInput->m_bOldKeyState[key]; }
    static bool GetKeyUp(int key) {return !s_pInput->m_bKeyState[key]&& s_pInput->m_bOldKeyState[key];}
    static int GetMousePosX(void) { return s_pInput->m_nMousex; }
    static int GetMousePosY(void) { return s_pInput->m_nMousey; }
    static int GetMouseMoveX(void) { return s_pInput->m_nMousex - s_pInput->m_nMouseOldx; }
    static int GetMouseMoveY(void) { return s_pInput->m_nMousey - s_pInput->m_nMouseOldy; }

};

