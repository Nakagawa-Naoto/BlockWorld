#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <d3d11.h>
#include <SimpleMath.h>
#include "Application.h"

#pragma comment(lib,"directxtk.lib")
#pragma comment(lib,"d3d11.lib")

// 3D���_�f�[�^
struct VERTEX_3D
{
    DirectX::SimpleMath::Vector3 Position;
    DirectX::SimpleMath::Vector3 Normal;
    DirectX::SimpleMath::Color	Diffuse;
    DirectX::SimpleMath::Vector2 TexCoord;
    //int			BoneIndex[4];
    //float		BoneWeight[4];
};

// �}�e���A��
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

// ���s����
struct LIGHT
{
    BOOL	Enable;
    BOOL	Dummy[3];
    DirectX::SimpleMath::Vector4	Direction;
    DirectX::SimpleMath::Color	    Diffuse;
    DirectX::SimpleMath::Color	    Ambient;

};

// �}�e���A���\����
struct MODEL_MATERIAL
{
    char						Name[256];
    MATERIAL					Material;
    char						TextureName[256];
    ID3D11ShaderResourceView* Texture;

};

// �`��T�u�Z�b�g�\����
struct SUBSET
{
    unsigned int	StartIndex;
    unsigned int	IndexNum;
    MODEL_MATERIAL	Material;
};


// ���f���\����
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

    static void Init(Application* ap);      // ����������
    static void Uninit();                   // �I������
    static void Begin();                    // �����_�����O�O����
    static void End();                      // �����_�����O�㏈��

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

    // ���_�V�F�[�_�[
    static void CreateVertexShader(ID3D11VertexShader** VertexShader,
        ID3D11InputLayout** VertexLayout,
        D3D11_INPUT_ELEMENT_DESC* layout ,
        UINT elements,
        const char* FileName);

    // �s�N�Z���V�F�[�_�[
    static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);

private:
    //=========================================================================
    // private variables.
    //=========================================================================
    
    static D3D_FEATURE_LEVEL s_FeatureLevel;

    static ID3D11Device*            s_pDevice;           // �f�o�C�X
    static ID3D11DeviceContext*     s_pDeviceContext;    // �f�o�C�X�R���e�L�X�g
    static IDXGISwapChain*          s_pSwapChain;        // �X���b�v�`�F�C��
    static ID3D11RenderTargetView*  s_pRenderTargetView; // �����_�[�^�[�Q�b�g�r���[
    static ID3D11DepthStencilView*  s_pDepthStencilView; // �f�v�X�X�e���V���r���[
    
    static ID3D11Buffer*    s_pWorldBuffer;          // ���[���h�o�b�t�@
    static ID3D11Buffer*    s_pViewBuffer;           // �r���[�o�b�t�@
    static ID3D11Buffer*    s_pProjectionBuffer;    // �v���W�F�N�V�����o�b�t�@
    static ID3D11Buffer*    s_pMaterialBuffer;       // �}�e���A���o�b�t�@
    static ID3D11Buffer*    s_pLightBuffer;          // ���C�g�o�b�t�@
    
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