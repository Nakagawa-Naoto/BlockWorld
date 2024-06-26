//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main.h"
#include "Application.h"

//-----------------------------------------------------------------------------
// メインエントリーポイント
//-----------------------------------------------------------------------------
int wmain(int argc, wchar_t** argv, wchar_t** evnp)
{

#if defined(DEBUG) || defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif//defined(DEBUG) || defined(_DEBUG)
  
    // アプリケーション実行
    Application app(SCREEN_WIDTH, SCREEN_HEIGHT);
    app.Run();
    return 0;
}