#pragma once
#include	<functional>
#include	<thread>
#include    <d3d11.h>
#include    <string>
#include    <locale.h>
#include    <d3dcompiler.h>

// 指定時間後にスレッドを起動
void Invoke(std::function<void()> Function, int Time);

//--------------------------------------------------------------------------------------
// シェーダーファイルを読み込む
//--------------------------------------------------------------------------------------
bool readShader(const char* csoName, std::vector<unsigned char>& byteArray);
//--------------------------------------------------------------------------------------
// ファイルの拡張子を取得する
//--------------------------------------------------------------------------------------
std::string GetFileExt(const char* filename);
//--------------------------------------------------------------------------------------
// シェーダーをコンパイル
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
/*------------------------
 Structuredバッファを作成
--------------------------*/
bool CreateStructuredBuffer(
    ID3D11Device* device,
    unsigned int stride,				// ストライドバイト数
    unsigned int num,					// 個数
    void* data,							// データ格納メモリ先頭アドレス
    ID3D11Buffer** pStructuredBuffer	// RWStructuredBuffer
);
/*------------------------
 Structuredバッファを作成
--------------------------*/
bool CreateStructuredBufferWrite(
    ID3D11Device* device,
    unsigned int stride,				// ストライドバイト数
    unsigned int num,					// 個数
    void* data,							// データ格納メモリ先頭アドレス
    ID3D11Buffer** pStructuredBuffer	// RWStructuredBuffer
);
/*------------------------
    ShaderResourceViewを作成
--------------------------*/
bool CreateShaderResourceView(
    ID3D11Device* device,
    ID3D11Buffer* pBuffer,	// Buffer
    ID3D11ShaderResourceView** ppSRV);
/*------------------------
UnOrderedAccessViewを作成
--------------------------*/
bool CreateUnOrderAccessView(
    ID3D11Device* device,
    ID3D11Buffer* pBuffer,	                    // Buffer
    ID3D11UnorderedAccessView** ppUAV);
/*---------------------------------
STAGINGバッファを作成しコピーする
----------------------------------*/
ID3D11Buffer* CreateAndCopyToBuffer(
    ID3D11Device* device,
    ID3D11DeviceContext* devicecontext,
    ID3D11Buffer* pBuffer	// RWStructuredBuffer
);
/*------------------------
頂点バッファを作成
--------------------------*/
bool CreateVertexBuffer(
    ID3D11Device* device,
    unsigned int stride,				// １頂点当たりバイト数
    unsigned int vertexnum,				// 頂点数
    void* vertexdata,					// 頂点データ格納メモリ先頭アドレス
    ID3D11Buffer** pVertexBuffer		// 頂点バッファ
);
/*------------------------
頂点バッファを作成(ＣＰＵ書き込み可能)
--------------------------*/
bool CreateVertexBufferWrite(
    ID3D11Device* device,
    unsigned int stride,				// １頂点当たりバイト数
    unsigned int vertexnum,				// 頂点数
    void* vertexdata,					// 頂点データ格納メモリ先頭アドレス
    ID3D11Buffer** pVertexBuffer		// 頂点バッファ
);
/*------------------------
頂点バッファ(UAV)を作成
--------------------------*/
bool CreateVertexBufferUAV(
    ID3D11Device* device,
    unsigned int stride,				// １頂点当たりバイト数
    unsigned int vertexnum,				// 頂点数
    void* vertexdata,					// 頂点データ格納メモリ先頭アドレス
    ID3D11Buffer** pVertexBuffer		// 頂点バッファ
);
/*----------------------------
コンスタントバッファを作成(MAPで書き換え可能)
------------------------------*/
bool CreateConstantBufferWrite(
    ID3D11Device* device,					// デバイスオブジェクト
    unsigned int bytesize,					// コンスタントバッファサイズ
    ID3D11Buffer** pConstantBuffer			// コンスタントバッファ
);


void Invoke(std::function<void()> Function, int Time);