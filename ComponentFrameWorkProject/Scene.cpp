//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include"Scene.h"
#include "main.h"
#include "GameScene.h"
#include "Time.h"

using namespace DirectX::SimpleMath;

///////////////////////////////////////////////////////////////////////////////
// Scene class
///////////////////////////////////////////////////////////////////////////////

Scene* Scene::s_pScene;
Scene* Scene::s_pNextScene;

//-----------------------------------------------------------------------------
// 生成処理
//-----------------------------------------------------------------------------
void Scene::CreateScene()
{
    // 初期化処理
	s_pScene = new GameScene();
	
	s_pScene->InitBase();
}

//-----------------------------------------------------------------------------
// 削除処理
//-----------------------------------------------------------------------------
void Scene::DestoryScene()
{
    // 終了処理
    s_pScene->UninitBase();
    delete s_pScene;
    s_pScene = nullptr;
    s_pNextScene = nullptr;
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void Scene::UpdateScene(void)
{
    SceneChange();
    s_pScene->UpdateBase();
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void Scene::DrawScene(void)
{
    s_pScene->DrawBase();
}

//-----------------------------------------------------------------------------
// ロードシーン
//-----------------------------------------------------------------------------
void Scene::LoadScene(Scene* scene)
{
    if (s_pNextScene == nullptr)s_pNextScene = scene;
}

//-----------------------------------------------------------------------------
// シーン変更
//-----------------------------------------------------------------------------
void Scene::SceneChange()
{
    if (s_pNextScene != nullptr)
    {
        s_pScene->UninitBase();
        delete s_pScene;
        s_pScene = s_pNextScene;
        s_pScene->InitBase();
        s_pNextScene = nullptr;
    }

}


