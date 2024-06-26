#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <list>
#include "GameObject.h"
#include"Sprite.h"
#include"polygon2D.h"

///////////////////////////////////////////////////////////////////////////////
// Scene class
///////////////////////////////////////////////////////////////////////////////
class Scene
{
    //=========================================================================
    // list of friend classes and methods.
    //=========================================================================
    /* NOTHING */

private:
    //=========================================================================
    // private variables.
    //=========================================================================
    static Scene* s_pScene;
    static Scene* s_pNextScene;
    class Camera* m_pCamera;
    //=========================================================================
    // private methods
    //=========================================================================
    /* NOTHING */
    
protected:
    //=========================================================================
    // protected variables.
    //=========================================================================
    /* NOTHING */
    
    //=========================================================================
    // protected methods
    //=========================================================================
    // コンストラクタ
    Scene(){}
    // デストラクタ
    virtual ~Scene(){}
    std::list<GameObject* > m_GameObjectList;
    void InitBase()
    {
        m_sName = typeid(this).name();
        Init();
    }
    void UninitBase()
    {
        for (auto& go : m_GameObjectList)
        {
            go->UninitBase();
            delete go;
        }
        // 要素をすべてクリアする
        m_GameObjectList.clear();
        Uninit();
    }
    void UpdateBase()
    {
        PreUpdate();
        for (auto go = m_GameObjectList.begin(); go != m_GameObjectList.end();)
        {
            if ((*go)->Destroy())
            {
                go = m_GameObjectList.erase(go);
                continue;
            }

            (*go)->UpdateBase();
            go++;
        }
        Update();
    }
    void DrawBase()
    {
        DirectX::SimpleMath::Matrix matrix;
        matrix = DirectX::SimpleMath::Matrix::Identity;
        for (auto& go : m_GameObjectList)go->DrawBase(matrix);
        Draw();
        ImGui::Begin(m_sName.c_str());
        for (auto& go : m_GameObjectList)go->GUIDrawBase();
        ImGui::LabelText("", "");
        ImGui::LabelText("", "%d Objects ", m_GameObjectList.size());
        ImGui::End();
    }

    virtual void Init() {}      // 初期化処理
    virtual void Uninit() {}    // 終了処理
    virtual void Update() {}    // 更新処理
    virtual void PreUpdate() {}    // 更新処理
    virtual void Draw() {}      // 描画処理

    template<typename T>
    T* AddGameObject()
    {
        T* gameObject = new T();
        m_GameObjectList.push_back(gameObject);
       ((GameObject*)gameObject)->SetScene(this);
       ((GameObject*)gameObject)->Init();
       ((GameObject*)gameObject)->SetName(typeid(T).name());
       return  gameObject;
    }

    

    
public:
    enum class State
    {
        NONE,
        MAIN,
    };
    //=========================================================================
    // public variables.
    //=========================================================================
    std::string m_sName;

    //=========================================================================
    // public methods.
    //=========================================================================
    
    static void CreateScene(void);// 生成
    
    static void DestoryScene(void);// 削除
    
    static void UpdateScene(void);// 更新
   
    static void DrawScene(void);// 描画
    
    static void LoadScene(Scene* scene);// ロード
    static void SceneChange();
    void SetCamera(Camera* camera) { m_pCamera = camera; }
    Camera* GetCamera(void) { return m_pCamera; }

};

