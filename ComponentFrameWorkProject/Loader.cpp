//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Loader.h"
#include "utftosjisconv.h"
#include <stdio.h>
#include <shlwapi.h>
#include <WICTextureLoader.h>
#include <SimpleMath.h>

#pragma comment(lib, "shlwapi.lib")

using namespace DirectX::SimpleMath;

std::unordered_map<std::string, MODEL*> Loader::s_ModelPool;

///////////////////////////////////////////////////////////////////////////////
// Loader class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// OBJファイル読み込み処理
//-----------------------------------------------------------------------------
void Loader::LoadObj(const char* FileName, MODEL_OBJ* ModelObj)
{
	char dir[MAX_PATH];
	strcpy(dir, FileName);
	PathRemoveFileSpec(dir);

	Vector3* positionArray;
	Vector3* normalArray;
	Vector2* texcoordArray;

	unsigned int	positionNum = 0;
	unsigned int	normalNum = 0;
	unsigned int	texcoordNum = 0;
	unsigned int	vertexNum = 0;
	unsigned int	indexNum = 0;
	unsigned int	in = 0;
	unsigned int	subsetNum = 0;

	MODEL_MATERIAL* materialArray = nullptr;
	unsigned int	materialNum = 0;

	char str[256];
	char* s;
	char c;


	FILE* file;
	file = fopen(FileName, "rt");
	assert(file);



	//要素数カウント
	while (true)
	{
		fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;

		if (strcmp(str, "v") == 0)
		{
			positionNum++;
		}
		else if (strcmp(str, "vn") == 0)
		{
			normalNum++;
		}
		else if (strcmp(str, "vt") == 0)
		{
			texcoordNum++;
		}
		else if (strcmp(str, "usemtl") == 0)
		{
			subsetNum++;
		}
		else if (strcmp(str, "f") == 0)
		{
			in = 0;

			do
			{
				fscanf(file, "%s", str);
				vertexNum++;
				in++;
				c = fgetc(file);
			} while (c != '\n' && c != '\r');

			//四角は三角に分割
			if (in == 4)
				in = 6;

			indexNum += in;
		}
	}


	//メモリ確保
	positionArray = new Vector3[positionNum];
	normalArray = new Vector3[normalNum];
	texcoordArray = new Vector2[texcoordNum];


	ModelObj->VertexArray = new VERTEX_3D[vertexNum];
	ModelObj->VertexNum = vertexNum;

	ModelObj->IndexArray = new unsigned int[indexNum];
	ModelObj->IndexNum = indexNum;

	ModelObj->SubsetArray = new SUBSET[subsetNum];
	ModelObj->SubsetNum = subsetNum;




	//要素読込
	Vector3* position = positionArray;
	Vector3* normal = normalArray;
	Vector2* texcoord = texcoordArray;

	unsigned int vc = 0;
	unsigned int ic = 0;
	unsigned int sc = 0;


	fseek(file, 0, SEEK_SET);

	while (true)
	{
		fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;

		if (strcmp(str, "mtllib") == 0)
		{
			//マテリアルファイル
			fscanf(file, "%s", str);

			char path[256];
			strcpy(path, dir);
			strcat(path, "\\");
			strcat(path, str);

			LoadMaterial(path, &materialArray, &materialNum);
		}
		else if (strcmp(str, "o") == 0)
		{
			//オブジェクト名
			fscanf(file, "%s", str);
		}
		else if (strcmp(str, "v") == 0)
		{
			//頂点座標
			fscanf(file, "%f", &position->x);
			fscanf(file, "%f", &position->y);
			fscanf(file, "%f", &position->z);
			position++;
		}
		else if (strcmp(str, "vn") == 0)
		{
			//法線
			fscanf(file, "%f", &normal->x);
			fscanf(file, "%f", &normal->y);
			fscanf(file, "%f", &normal->z);
			normal++;
		}
		else if (strcmp(str, "vt") == 0)
		{
			//テクスチャ座標
			fscanf(file, "%f", &texcoord->x);
			fscanf(file, "%f", &texcoord->y);
			texcoord->x = 1.0f - texcoord->x;
			texcoord->y = 1.0f - texcoord->y;
			texcoord++;
		}
		else if (strcmp(str, "usemtl") == 0)
		{
			//マテリアル
			fscanf(file, "%s", str);

			if (sc != 0)
				ModelObj->SubsetArray[sc - 1].IndexNum = ic - ModelObj->SubsetArray[sc - 1].StartIndex;

			ModelObj->SubsetArray[sc].StartIndex = ic;


			for (unsigned int i = 0; i < materialNum; i++)
			{
				if (strcmp(str, materialArray[i].Name) == 0)
				{
					ModelObj->SubsetArray[sc].Material.Material = materialArray[i].Material;
					strcpy(ModelObj->SubsetArray[sc].Material.TextureName, materialArray[i].TextureName);
					strcpy(ModelObj->SubsetArray[sc].Material.Name, materialArray[i].Name);

					break;
				}
			}

			sc++;

		}
		else if (strcmp(str, "f") == 0)
		{
			//面
			in = 0;

			do
			{
				fscanf(file, "%s", str);

				s = strtok(str, "/");
				ModelObj->VertexArray[vc].Position = positionArray[atoi(s) - 1];
				if (s[strlen(s) + 1] != '/')
				{
					//テクスチャ座標が存在しない場合もある
					s = strtok(nullptr, "/");
					ModelObj->VertexArray[vc].TexCoord = texcoordArray[atoi(s) - 1];
				}
				s = strtok(nullptr, "/");
				ModelObj->VertexArray[vc].Normal = normalArray[atoi(s) - 1];

				ModelObj->VertexArray[vc].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);

				ModelObj->IndexArray[ic] = vc;
				ic++;
				vc++;

				in++;
				c = fgetc(file);
			} while (c != '\n' && c != '\r');

			//四角は三角に分割
			if (in == 4)
			{
				ModelObj->IndexArray[ic] = vc - 4;
				ic++;
				ModelObj->IndexArray[ic] = vc - 2;
				ic++;
			}
		}
	}


	if (sc != 0)
		ModelObj->SubsetArray[sc - 1].IndexNum = ic - ModelObj->SubsetArray[sc - 1].StartIndex;


	fclose(file);


	delete[] positionArray;
	delete[] normalArray;
	delete[] texcoordArray;
	delete[] materialArray;
}

//-----------------------------------------------------------------------------
// マテリアル読み込み処理
//-----------------------------------------------------------------------------
void Loader::LoadMaterial(const char* FileName, MODEL_MATERIAL** MaterialArray, unsigned int* MaterialNum)
{
	char dir[MAX_PATH];
	strcpy(dir, FileName);
	PathRemoveFileSpec(dir);



	char str[256];

	FILE* file;
	file = fopen(FileName, "rt");
	assert(file);

	MODEL_MATERIAL* materialArray;
	unsigned int materialNum = 0;

	//要素数カウント
	while (true)
	{
		fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;


		if (strcmp(str, "newmtl") == 0)
		{
			materialNum++;
		}
	}


	//メモリ確保
	materialArray = new MODEL_MATERIAL[materialNum];


	//要素読込
	int mc = -1;

	fseek(file, 0, SEEK_SET);

	while (true)
	{
		fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;


		if (strcmp(str, "newmtl") == 0)
		{
			//マテリアル名
			mc++;
			fscanf(file, "%s", materialArray[mc].Name);
			strcpy(materialArray[mc].TextureName, "");

			materialArray[mc].Material.Emission.x = 0.0f;
			materialArray[mc].Material.Emission.y = 0.0f;
			materialArray[mc].Material.Emission.z = 0.0f;
			materialArray[mc].Material.Emission.w = 0.0f;
		}
		else if (strcmp(str, "Ka") == 0)
		{
			//アンビエント
			fscanf(file, "%f", &materialArray[mc].Material.Ambient.x);
			fscanf(file, "%f", &materialArray[mc].Material.Ambient.y);
			fscanf(file, "%f", &materialArray[mc].Material.Ambient.z);
			materialArray[mc].Material.Ambient.w = 1.0f;
		}
		else if (strcmp(str, "Kd") == 0)
		{
			//ディフューズ
			fscanf(file, "%f", &materialArray[mc].Material.Diffuse.x);
			fscanf(file, "%f", &materialArray[mc].Material.Diffuse.y);
			fscanf(file, "%f", &materialArray[mc].Material.Diffuse.z);
			materialArray[mc].Material.Diffuse.w = 1.0f;
		}
		else if (strcmp(str, "Ks") == 0)
		{
			//スペキュラ
			fscanf(file, "%f", &materialArray[mc].Material.Specular.x);
			fscanf(file, "%f", &materialArray[mc].Material.Specular.y);
			fscanf(file, "%f", &materialArray[mc].Material.Specular.z);
			materialArray[mc].Material.Specular.w = 1.0f;
		}
		else if (strcmp(str, "Ns") == 0)
		{
			//スペキュラ強度
			fscanf(file, "%f", &materialArray[mc].Material.Shininess);
		}
		else if (strcmp(str, "d") == 0)
		{
			//アルファ
			fscanf(file, "%f", &materialArray[mc].Material.Diffuse.w);
		}
		else if (strcmp(str, "map_Kd") == 0)
		{
			//テクスチャ
			fscanf(file, "%s", str);

			char path[256];
			strcpy(path, dir);
			strcat(path, "\\");
			strcat(path, str);

			strcat(materialArray[mc].TextureName, path);
		}
	}

	fclose(file);

	*MaterialArray = materialArray;
	*MaterialNum = materialNum;
}
//-----------------------------------------------------------------------------
// モデル読み込み処理
//-----------------------------------------------------------------------------
void Loader::LoadModel(const char* FileName, MODEL* Model)
{
	MODEL_OBJ modelObj;
	LoadObj(FileName, &modelObj);



	// 頂点バッファ生成
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * modelObj.VertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = modelObj.VertexArray;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &Model->VertexBuffer);
	}


	// インデックスバッファ生成
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * modelObj.IndexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = modelObj.IndexArray;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &Model->IndexBuffer);
	}

	// サブセット設定
	{
		Model->SubsetArray = new SUBSET[modelObj.SubsetNum];
		Model->SubsetNum = modelObj.SubsetNum;

		for (unsigned int i = 0; i < modelObj.SubsetNum; i++)
		{
			Model->SubsetArray[i].StartIndex = modelObj.SubsetArray[i].StartIndex;
			Model->SubsetArray[i].IndexNum = modelObj.SubsetArray[i].IndexNum;

			Model->SubsetArray[i].Material.Material = modelObj.SubsetArray[i].Material.Material;

			Model->SubsetArray[i].Material.Texture = nullptr;

			// s-jisをワイド文字に
			std::wstring ws = sjis_to_wide_winapi(modelObj.SubsetArray[i].Material.TextureName);

			// テクスチャ読み込み
			DirectX::CreateWICTextureFromFile(
				Renderer::GetDevice(),
				//				modelObj.SubsetArray[i].Material.TextureName,
				ws.c_str(),
				nullptr,
				&Model->SubsetArray[i].Material.Texture);

			if (Model->SubsetArray[i].Material.Texture)
				Model->SubsetArray[i].Material.Material.TextureEnable = true;
			else
				Model->SubsetArray[i].Material.Material.TextureEnable = false;

		}
	}

	delete[] modelObj.VertexArray;
	delete[] modelObj.IndexArray;
	delete[] modelObj.SubsetArray;

}

//-----------------------------------------------------------------------------
// 先読み込み処理
//-----------------------------------------------------------------------------
void Loader::PreLoad3DModel(const char* FileName)
{
	// プールにあるなら読み込まない
	if (s_ModelPool.count(FileName) > 0)return;

	// モデルの読み込み
	MODEL* model = new MODEL;
	LoadModel(FileName, model);
	// プールに保存
	s_ModelPool[FileName] = model;
}

//-----------------------------------------------------------------------------
// 3Dモデル読み込み処理
//-----------------------------------------------------------------------------
void Loader::Load3DModel(const char* FileName, MODEL** Model)
{
	// プールにあるならそこからモデルを取得する
	if (s_ModelPool.count(FileName) > 0)
	{
		*Model = s_ModelPool[FileName];
		return;
	}

	// ないならモデル読み込み
	*Model = new MODEL;
	LoadModel(FileName, *Model);
	// プールに設定
	s_ModelPool[FileName] = *Model;
}


//-----------------------------------------------------------------------------
// 全データのアンロード処理
//-----------------------------------------------------------------------------
void Loader::UnloadAll()
{
	for (std::pair<const std::string, MODEL*> pair : s_ModelPool)
	{
		pair.second->VertexBuffer->Release();
		pair.second->IndexBuffer->Release();

		for (unsigned int i = 0; i < pair.second->SubsetNum; i++)
		{
			if (pair.second->SubsetArray[i].Material.Texture)
				pair.second->SubsetArray[i].Material.Texture->Release();
		}

		delete[] pair.second->SubsetArray;

		delete pair.second;
	}

	s_ModelPool.clear();
}
