#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <list>
#include <typeinfo>
#include <SimpleMath.h>
#include "Component.h"
#include <iostream>
#include <typeindex>
#include <algorithm>
#include "renderer.h"
#include "GUI.h"
//#include "Log.h"

///////////////////////////////////////////////////////////////////////////////
// GameObject class
///////////////////////////////////////////////////////////////////////////////

class GameObject
{
    //=========================================================================
    // list of friend classes and methods.
    //=========================================================================
    /* NOTHING */
protected:
    //=========================================================================
    // protected variables.
    //=========================================================================
    bool m_bDestroy = false;
    bool m_bActive = true;
    DirectX::SimpleMath::Vector3 m_Position = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
    DirectX::SimpleMath::Vector3 m_Rotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
    DirectX::SimpleMath::Vector3 m_Scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);
    std::string m_sName = "GameObject";
    GameObject* m_ParentGameObject;
    std::list<Component* >m_pComponentList;
    std::list<GameObject*>m_pChildrenObjectList;

    class Scene* m_Scene;

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
    GameObject(){};// コンストラクタ
    virtual~GameObject(){}// デストラクタ（仮想関数）
    
    // Getter
    DirectX::SimpleMath::Vector3 GetPosition() { return m_Position; }
    DirectX::SimpleMath::Vector3 GetRotation() { return m_Rotation; }
    DirectX::SimpleMath::Vector3 GetScale() { return m_Scale; }
    DirectX::SimpleMath::Vector3 GetForward()//前方向ベクトル取得
    {
        DirectX::SimpleMath::Matrix rot;
        rot = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z);

        DirectX::SimpleMath::Vector3 forward;
        forward.x = rot._31;
        forward.y = rot._32;
        forward.z = rot._33;

        return forward;
    }
    GameObject* GetParent() { return m_ParentGameObject; }


    // Setter
    void SetPosition(DirectX::SimpleMath::Vector3 position) { m_Position = position; }
    void SetRotation(DirectX::SimpleMath::Vector3 rotation) { m_Rotation = rotation; }
    void SetScale(DirectX::SimpleMath::Vector3 scale) { m_Scale = scale; }
    void SetDestroy() { m_bDestroy = true; }
    void SetName(std::string name) { m_sName = name; }
//    void SetParent(GameObject* _gameObj)
//    {
//        auto result = std::find(m_ChildGameObject.begin(), m_ChildGameObject.end(), _gameObj);
//        bool NotChildflag = result == m_ChildGameObject.end();// 子オブジェではない時
//        bool HasParent = m_ParentGameObject != nullptr;
//
//
//        if (!NotChildflag)
//        {
//#if  defined(DEBUG) || defined(_DEBUG)
//            std::cout << "属している子オブジェクトを自身の親オブジェにセットできません" << std::endl;
//#endif
//            return;
//        }
//        
//        if (HasParent)
//        {
//            m_ParentGameObject->SubChild(this);// 親オブジェの子オブジェ配列から切り離す
//            std::cout << "親オブジェを上書きしました。" << std::endl;
//        }
//        m_ParentGameObject = _gameObj;        
//    }



    bool Destroy()
    {
        if (m_bDestroy)
        {
            UninitBase();
            delete this;
            return true;
        }
        else
        {
            return false;
        }
    }

    // 
    virtual void Init(){}
    virtual void Uninit(){}
    virtual void Update(){}
    virtual void Draw(){}
    virtual void PreDraw() {}
    virtual void GUIDraw() {}
    void SetScene(Scene* scene) { m_Scene = scene; }
    Scene* GetScene(void) { return m_Scene; }

    // コンポーネントの追加
    template<typename T>
    T* AddComponent()
    {
        T* component = new T(this);
        m_pComponentList.push_back(component);
        ((Component*)component)->Init();
        ((Component*)component)->SetName(typeid(T).name());
        return component;
    }
    // コンポーネントの取得
    template<typename T>
    T* GetComponent()
    {
        for(auto& mp:m_pComponentList)
        {
            if (typeid(*mp) == typeid(T))return (T*)mp;
        }
#if  defined(DEBUG) || defined(_DEBUG)
        std::string text = typeid(T).name();
        text += "のコンポーネントを取得できませんでした。";
        //Logger::Error(Logger::NameTag::GAME, text);
#endif
        return nullptr;
    }
    // 子オブジェクトの追加
    template <typename T>
    T* AddChild()
    {
        T* child = new T();
        m_pChildrenObjectList.push_back(child);
        child->InitBase();
        child->SetParent(this);

        return child;
    }

    // 配列のリストから削除
//    void SubChild(GameObject*_obj)
//    {
//        auto result = std::find(m_ChildGameObject.begin(), m_ChildGameObject.end(), _Obj);
//        bool NotChildflag = result == m_ChildGameObject.end();// 子オブジェではない時
//        
//        if (NotChildflag)
//        {
//#if  defined(DEBUG) || defined(_DEBUG)
//            std::cout << "このオブジェは子オブジェではありません。" << std::endl;
//#endif
//        }
//        //m_ChildGameObject.erase(result)
//            
//    }


    void InitBase()
    {
        Init();
    }

    void UninitBase()
    {
        Uninit();
        // ts 20230630（子供ゲームオブジェクトの終了処理忘れを追加）
        for (auto* childgo : m_pChildrenObjectList)
        {
            childgo->Uninit();

            for (Component* component : childgo->m_pComponentList)
            {
                component->Uninit();
                delete component;
            }
            childgo->m_pComponentList.clear();//リストのクリア

            delete childgo;
        }
        m_pChildrenObjectList.clear();//リストのクリア

        for (Component* component : m_pComponentList)
        {
            component->Uninit();
            delete component;
        }
        m_pComponentList.clear();//リストのクリア
    }

    void UpdateBase()
    {
        
        for (Component* component : m_pComponentList)
        {
            component->Update();
        }
        Update();
    }

    void DrawBase(DirectX::SimpleMath::Matrix ParentMatrix)
    {
        // マトリクス設定
        DirectX::SimpleMath::Matrix world, scale, rot, trans;
        scale = DirectX::SimpleMath::Matrix::CreateScale(m_Scale.x, m_Scale.y, m_Scale.z);
        rot = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z);
        trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_Position.x, m_Position.y, m_Position.z);
        world = scale * rot * trans * ParentMatrix;

        PreDraw();

        for (GameObject* child : m_pChildrenObjectList)
        {
            child->DrawBase(world);
        }


        Renderer::SetWorldMatrix(&world);

        for (Component* component : m_pComponentList)
        {
            component->Draw();
        }

        Draw();
    }
    
    
    void GUIDrawBase()
    {
        if (ImGui::TreeNode(m_sName.c_str()))
        {
            DrawInformation();
            ComponentsGUIDraw();
            ImGui::TreePop();
        }
    }


private:
    //=========================================================================
    // private variables.
    //=========================================================================
    /* NOTHING */

    //=========================================================================
    // private methods.
    //=========================================================================
    void DrawInformation()
    {
        if (ImGui::TreeNode("Information"))
        {
            ImGui::Checkbox("Active", &m_bActive);
            ImGui::Text("Position x:%0.1f y:%0.1f z:%0.1f", m_Position.x, m_Position.y, m_Position.z);
            ImGui::Text("Rotation x:%0.1f y:%0.1f z:%0.1f", m_Rotation.x, m_Rotation.y, m_Rotation.z);
            ImGui::Text("Scale    x:%0.1f y:%0.1f z:%0.1f", m_Scale.x, m_Scale.y, m_Scale.z);
            GUIDraw();
            ImGui::TreePop();
        }
    }
    void ComponentsGUIDraw()
    {
        if (ImGui::TreeNode("components"))
        {
            for (Component* component : m_pComponentList)ComponentGUI(component);
            ImGui::TreePop();
        }
    }
    void ComponentGUI(Component* component)
    {
        if (ImGui::TreeNode(component->GetName().c_str()))
        {
            component->GUIDraw();
            ImGui::TreePop();
        }
    }
};



