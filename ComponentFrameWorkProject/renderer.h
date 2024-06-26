#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <d3d11.h>
#include <SimpleMath.h>
#include "Application.h"

#pragma comment(lib,"directxtk.lib")
#pragma comment(lib,"d3d11.lib")

// 3D頂点データ
struct VERTEX_3D
{
    DirectX::SimpleMath::Vector3 Position;
    DirectX::SimpleMath::Vector3 Normal;
    DirectX::SimpleMath::Color	Diffuse;
    DirectX::SimpleMath::Vector2 TexCoord;
    //int			BoneIndex[4];
    //float		BoneWeight[4];
};

// マテリアル
struct MATERIAL
{
    DirectX::SimpleMath::Color  Ambient;
    DirectX::SimpleMath::Color  Diffuse;
    DirectX::SimpleMath::Color  Specular;
    DirectX::SimpleMath::Color  Emission;
    float   Shininess;
    BOOL    TextureEnable;
    float    Dummy[2];
};

// 平行光源
struct LIGHT
{
    BOOL	Enable;
    BOOL	Dummy[3];
    DirectX::SimpleMath::Vector4	Direction;
    DirectX::SimpleMath::Color	    Diffuse;
    DirectX::SimpleMath::Color	    Ambient;

};

// マテリアル構造体
struct MODEL_MATERIAL
{
    char						Name[256];
    MATERIAL					Material;
    char						TextureName[256];
    ID3D11ShaderResourceView* Texture;

};

// 描画サブセット構造体
struct SUBSET
{
    unsigned int	StartIndex;
    unsigned int	IndexNum;
    MODEL_MATERIAL	Material;
};


// モデル構造体
struct MODEL_OBJ
{
    VERTEX_3D* VertexArray;
    unsigned int	VertexNum;

    unsigned int* IndexArray;
    unsigned int	IndexNum;

    SUBSET* SubsetArray;
    unsigned int	SubsetNum;
};

struct MODEL
{
    ID3D11Buffer* VertexBuffer;
    ID3D11Buffer* IndexBuffer;

    SUBSET* SubsetArray;
    unsigned int	SubsetNum;
    
};


///////////////////////////////////////////////////////////////////////////////
// Renderer class
///////////////////////////////////////////////////////////////////////////////
class Renderer
{
public:
    //=========================================================================
    // public variables.
    //=========================================================================
    /*NOTHING*/
    //=========================================================================
    // public methods.
    //=========================================================================

    static void Init(Application* ap);      // 初期化処理
    static void Uninit();                   // 終了処理
    static void Begin();                    // レンダリング前処理
    static void End();                      // レンダリング後処理

    // Setter
    static void SetDepthEnable(bool Enable);
    static void SetATCEnable(bool Enable);
    static void SetWorldViewProjection2D();
    static void SetWorldMatrix(DirectX::SimpleMath::Matrix* WorldMatrix);
    static void SetViewMatrix(DirectX::SimpleMath::Matrix* ViewMatrix);

    static void SetProjectionMatrix(DirectX::SimpleMath::Matrix* ProjectionMatrix);
    static void SetMaterial(MATERIAL Material);
    static void SetLight(LIGHT Light);

    // Getter
    static ID3D11Device* GetDevice(void) { return s_pDevice; }
    static ID3D11DeviceContext* GetDeviceContext(void) { return s_pDeviceContext; }

    // 頂点シェーダー
    static void CreateVertexShader(ID3D11VertexShader** VertexShader,
        ID3D11InputLayout** VertexLayout,
        D3D11_INPUT_ELEMENT_DESC* layout ,
        UINT elements,
        const char* FileName);

    // ピクセルシェーダー
    static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);

private:
    //=========================================================================
    // private variables.
    //=========================================================================
    
    static D3D_FEATURE_LEVEL s_FeatureLevel;

    static ID3D11Device*            s_pDevice;           // デバイス
    static ID3D11DeviceContext*     s_pDeviceContext;    // デバイスコンテキスト
    static IDXGISwapChain*          s_pSwapChain;        // スワップチェイン
    static ID3D11RenderTargetView*  s_pRenderTargetView; // レンダーターゲットビュー
    static ID3D11DepthStencilView*  s_pDepthStencilView; // デプスステンシルビュー
    
    static ID3D11Buffer*    s_pWorldBuffer;          // ワールドバッファ
    static ID3D11Buffer*    s_pViewBuffer;           // ビューバッファ
    static ID3D11Buffer*    s_pProjectionBuffer;    // プロジェクションバッファ
    static ID3D11Buffer*    s_pMaterialBuffer;       // マテリアルバッファ
    static ID3D11Buffer*    s_pLightBuffer;          // ライトバッファ
    
    static ID3D11DepthStencilState* s_pDepthStateEnable;
    static ID3D11DepthStencilState* s_pDepthStateDisable;

    static ID3D11BlendState* s_pBlendState;
    static ID3D11BlendState* s_pBlendStateATC;

    static Application* s_pApplication;

    //=========================================================================
    // private methods.
    //=========================================================================
    /*NOTHING*/
};