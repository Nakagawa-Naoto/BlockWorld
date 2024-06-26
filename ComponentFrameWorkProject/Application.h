﻿#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <Windows.h>
#include <cstdint>
#include "Manager.h"


///////////////////////////////////////////////////////////////////////////////
// App class
///////////////////////////////////////////////////////////////////////////////
class Application
{
    //=========================================================================
    // list of friend classes and methods.
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
    Application(uint32_t width, uint32_t height);
    ~Application();
    void Run();

    // 幅を取得
    uint32_t GetWidth() {
        return m_uWidth;
    }

    // 高さを取得
    uint32_t GetHeight() {
        return m_uHeight;
    }

    // ウインドウハンドルを返す
    HWND GetWindow() {
        return m_hWnd;
    }
    HWND* GetWindowH()
    {
        return &m_hWnd;
    }

private:
    //=========================================================================
    // private variables.
    //=========================================================================
    HINSTANCE   m_hInst;        // インスタンスハンドルです.
    HWND        m_hWnd;         // ウィンドウハンドルです.
    uint32_t    m_uWidth;        // ウィンドウの横幅です.
    uint32_t    m_uHeight;       // ウィンドウの縦幅です.
    
    //=========================================================================
    // private methods.
    //=========================================================================
    bool InitApp();
    void TermApp();
    bool InitWnd();
    void TermWnd();
    void MainLoop();

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
};