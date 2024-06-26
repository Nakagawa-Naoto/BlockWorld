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
    // �R���X�g���N�^
    Scene(){}
    // �f�X�g���N�^
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
        // �v�f�����ׂăN���A����
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

    virtual void Init() {}      // ����������
    virtual void Uninit() {}    // �I������
    virtual void Update() {}    // �X�V����
    virtual void PreUpdate() {}    // �X�V����
    virtual void Draw() {}      // �`�揈��

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
    
    static void CreateScene(void);// ����
    
    static void DestoryScene(void);// �폜
    
    static void UpdateScene(void);// �X�V
   
    static void DrawScene(void);// �`��
    
    static void LoadScene(Scene* scene);// ���[�h
    static void SceneChange();
    void SetCamera(Camera* camera) { m_pCamera = camera; }
    Camera* GetCamera(void) { return m_pCamera; }

};

