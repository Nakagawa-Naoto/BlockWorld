//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Application.h"
#include "Audio.h"
#include "Time.h"
#include "Input.h"
#include "Manager.h"
#include "renderer.h"
#include "Loader.h"
#include <chrono>
#include <thread>

#include "imgui\imgui_impl_win32.h"

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);



//-----------------------------------------------------------------------------
// Constant Values.
//-----------------------------------------------------------------------------
const auto ClassName = TEXT("ActionGame3D");     //!< ウィンドウクラス名.
const auto WindowName = TEXT("BlockWorld");    //!< ウィンドウ名.

Time* Time::s_pTime;
Input* Input::s_pInput;
///////////////////////////////////////////////////////////////////////////////
// App class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// コンストラクタです.
//-----------------------------------------------------------------------------
Application::Application(uint32_t width, uint32_t height)
: m_hInst   (nullptr)
, m_hWnd    (nullptr)
, m_uWidth   (width)
, m_uHeight  (height)
{ /* DO_NOTHING */ }

//-----------------------------------------------------------------------------
// デストラクタです.
//-----------------------------------------------------------------------------
Application::~Application()
{ /* DO_NOTHING */ }

//-----------------------------------------------------------------------------
// 実行します.
//-----------------------------------------------------------------------------
void Application::Run()
{
    if (InitApp())MainLoop();
    
    TermApp();
}

//-----------------------------------------------------------------------------
// 初期化処理です.
//-----------------------------------------------------------------------------
bool Application::InitApp()
{
    // ウィンドウの初期化.
    if (!InitWnd())return false;
    Time::Init(60);
    Input::Init();
    Audio::InitMaster();
    Manager::Init(this);
    // 正常終了.
    return true;
}

//-----------------------------------------------------------------------------
// 終了処理です.
//-----------------------------------------------------------------------------
void Application::TermApp()
{
    Manager::Uninit();
    Loader::UnloadAll();
    Audio::UninitMaster();
    Input::Init();
    Time::Uninit();
    // ウィンドウの終了処理.
    TermWnd();
}

//-----------------------------------------------------------------------------
// ウィンドウの初期化処理です.
//-----------------------------------------------------------------------------
bool Application::InitWnd()
{
    // インスタンスハンドルを取得.
    auto hInst = GetModuleHandle(nullptr);
    if (hInst == nullptr)
    { 
        return false; 
    }

    // ウィンドウの設定.
    WNDCLASSEX wc = {};
    wc.cbSize           = sizeof(WNDCLASSEX);
    wc.style            = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc      = WndProc;
    wc.hIcon            = LoadIcon(hInst, IDI_APPLICATION);
    wc.hCursor          = LoadCursor(hInst, IDC_ARROW);
    wc.hbrBackground    = GetSysColorBrush(COLOR_BACKGROUND);
    wc.lpszMenuName     = nullptr;
    wc.lpszClassName    = ClassName;
    wc.hIconSm          = LoadIcon(hInst, IDI_APPLICATION);

    // ウィンドウの登録.
    if (!RegisterClassEx(&wc))
    { return false; }

    // インスタンスハンドル設定.
    m_hInst = hInst;

    // ウィンドウのサイズを設定.
    RECT rc = {};
    rc.right  = static_cast<LONG>(m_uWidth);
    rc.bottom = static_cast<LONG>(m_uHeight);

    // ウィンドウサイズを調整.
    auto style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
    AdjustWindowRect(&rc, style, FALSE);

    // ウィンドウを生成.
    m_hWnd = CreateWindowEx(
        0,
        ClassName,
        WindowName,
        style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rc.right  - rc.left,
        rc.bottom - rc.top,
        nullptr,
        nullptr,
        m_hInst,
        nullptr);

    if (m_hWnd == nullptr)
    { return false; }
 
    // ウィンドウを表示.
    ShowWindow(m_hWnd, SW_SHOWNORMAL);

    // ウィンドウを更新.
    UpdateWindow(m_hWnd);

    // ウィンドウにフォーカスを設定.
    SetFocus(m_hWnd);

    // 正常終了.
    return true;
}

//-----------------------------------------------------------------------------
// ウィンドウの終了処理です.
//-----------------------------------------------------------------------------
void Application::TermWnd()
{
    // ウィンドウの登録を解除.
    if (m_hInst != nullptr)
    { UnregisterClass(ClassName, m_hInst); }

    m_hInst = nullptr;
    m_hWnd  = nullptr;
}

//-----------------------------------------------------------------------------
// メインループです.
//-----------------------------------------------------------------------------
void Application::MainLoop()
{
    MSG msg = {};
    
    while(WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            
            uint64_t delta_time = 0;
            // デルタタイムを計算
            delta_time = Time::CalcDelta();
            
            // 更新処理
            Manager::Update(delta_time);

            // 描画処理
            Manager::Draw(delta_time);

            Input::Update();           

            // 規定時間まで時間調整
            Time::Wait();
        }
    }
}

//-----------------------------------------------------------------------------
// ウィンドウプロシージャです.
//-----------------------------------------------------------------------------
LRESULT CALLBACK Application::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wp, lp))return true;

    switch(msg)
    {
        case WM_DESTROY:
            { PostQuitMessage(0); }
            break;
        case WM_KEYDOWN:
            Input::SetKeyDown(LOWORD(wp));
            break;
        case WM_KEYUP:
            Input::SetKeyUp(LOWORD(wp));
            break;
        case WM_MOUSEMOVE:
            Input::SetMousePos(LOWORD(lp), HIWORD(lp));
        default:
            { /* DO_NOTHING */ }
            break;
    }

    return DefWindowProc(hWnd, msg, wp, lp);
}
