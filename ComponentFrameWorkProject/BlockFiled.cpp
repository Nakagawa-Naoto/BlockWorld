//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "BlockFiled.h"
#include "Shader.h"
#include "renderer.h"
#include "Frustum.h"
#include "Scene.h"
#include "Camera.h"
#include "GUI.h"
#include <iostream>
#include "AABB.h"
#include <thread>
#include <timeapi.h>


#if defined(DEBUG) || defined(_DEBUG)
#include "Input.h"
#endif // DEBUG


using namespace DirectX::SimpleMath;

///////////////////////////////////////////////////////////////////////////////
// BlockFiled class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// エリアの中にポイントがあるか判定する
//-----------------------------------------------------------------------------
inline bool BlockFiled::IsInArea(const DirectX::SimpleMath::Vector3 PointPosition, const DirectX::SimpleMath::Vector3 AreaPosition, const float areaRange)
{
	Vector3 distance = PointPosition - AreaPosition;
	float length = (distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z);
	return length < areaRange * areaRange;
}

//-----------------------------------------------------------------------------
// どのブロックを置くか判定する
//-----------------------------------------------------------------------------
inline BlockID BlockFiled::GetBlockDataID(const double spaceDensity, const int groundHeight, const int y)
{
	const float limit = 0.65f;
	bool IsCavity = spaceDensity >= limit;	// 空洞か
	bool IsSky = y > groundHeight;			// 上空か
	bool IsAir = IsSky || IsCavity;			// 空気か

	// 地表なら、芝生ブロック
	bool IsGrass = groundHeight == y;
	
	// 地表から近ければ、土ブロック
	const int dirtH = 4;
	bool IsDirt = groundHeight - y < dirtH;
	
	// 地表から近くて標高が低いなら、砂ブロック
	const int sandHeight = 43;
	bool IsUnder = groundHeight <= sandHeight;
	bool IsSand = IsUnder && IsDirt;
	
	if (IsAir)return BlockID::Air;
	if (IsSand)return BlockID::Sand;
	if (IsGrass)return BlockID::Grass;
	if (IsDirt)return BlockID::Dirt;

	// 石か
	return BlockID::Stone;
}

//-----------------------------------------------------------------------------
// 地表までの地下部分の生成
//-----------------------------------------------------------------------------
void BlockFiled::CreateGround(Chunk* stage,const int x,const int z)
{
	// 地面の高さを求める
	int groundHeight = GetGroundHeight((float)(x + stage->posx * MaxX), (float)(z + stage->posz * MaxZ));
	// 地表までの地下部分の生成
	for (int y = 0; y < MaxY; y++)
	{
		// 空間の密度を求める
		double spaceDensity = GetSpaceDensity((float)(x + stage->posx * MaxX), (float)(y + stage->posy * MaxY), (float)(z + stage->posz * MaxZ));
		stage->blockID[x][y][z] = (int8_t)GetBlockDataID(spaceDensity, groundHeight, (int)(y + stage->posy * MaxY));
	}

	// 一番下を埋める
	if (stage->posy == 0)stage->blockID[x][0][z] = (int8_t)BlockID::Stone;
	stage->isCreated = true;
}

//-----------------------------------------------------------------------------
// 地形生成
//-----------------------------------------------------------------------------
void BlockFiled::FiledGenerator(Chunk* stage)
{
	for (int x = 0; x < MaxX; x++)
	{
		for (int z = 0; z < MaxZ; z++)
		{
			CreateGround(stage, x, z);
		}
	}
}

//-----------------------------------------------------------------------------
// 地形生成
//-----------------------------------------------------------------------------
void BlockFiled::FiledGenerator(const int num)
{
	if (num >= ActiveaMapNum)return;
	for (int x = 0; x < MaxX; x++)
	{
		for (int z = 0; z < MaxZ; z++)
		{
			CreateGround(&m_stage[num], x, z);
		}
	}
	
}

//-----------------------------------------------------------------------------
// 地表の高さを求める
//-----------------------------------------------------------------------------
inline double BlockFiled::GetSurfaceHeight(const float x, const float z)
{
	PerlinNoise perlin;
	const float sizeMag = 0.00004f;	// 拡大率
	const int heightMag = 57;		// 高さの倍率
	const int offset = 16;			// オフセット
	const int octave = 10;			// 複雑さ
	const double persistence = 0.98f;
	return  perlin.octaveNoise(x * sizeMag, 0, z * sizeMag, octave, persistence) * heightMag + offset;
}

//-----------------------------------------------------------------------------
// 山の高さを求める
//-----------------------------------------------------------------------------
inline double  BlockFiled::GetMountainHeight(const float x, const float z)
{
	PerlinNoise perlin;
	const float sizeMag = 0.00001f;	// 拡大率
	const int heightMag = 126;		// 高さの倍率
	const int offset = -30;			// オフセット
	const int octave = 10;			// 複雑さ
	const double persistence = 5.0f;
	return perlin.octaveNoise(x * sizeMag, 0, z * sizeMag, octave, persistence) * heightMag + offset;
}

//-----------------------------------------------------------------------------
// 地面の高さを求める
//-----------------------------------------------------------------------------
inline int BlockFiled::GetGroundHeight(const float x, const float z)
{
	// 地表の高さを求める
	double surfaceHeight = GetSurfaceHeight(x, z);

	// 山の高さを求める
	double MountainHeight = GetMountainHeight(x, z);

	// 高い方を設定する
	double height = std::max<double>(surfaceHeight, MountainHeight);
	
	const int HeightOffset = 0;
	return  (int)height + HeightOffset;
}

//-----------------------------------------------------------------------------
// ブロックが完全に囲まれてはいない
//-----------------------------------------------------------------------------
inline bool BlockFiled::IsBlockSurround(const float x, const float y, const float z)
{
	if (GetBlockID((int)x - 1, (int)y, (int)z) == (int8_t)BlockID::Air) return false;
	if (GetBlockID((int)x + 1, (int)y, (int)z) == (int8_t)BlockID::Air) return false;
	if (GetBlockID((int)x, (int)y - 1, (int)z) == (int8_t)BlockID::Air) return false;
	if (GetBlockID((int)x, (int)y + 1, (int)z) == (int8_t)BlockID::Air) return false;
	if (GetBlockID((int)x, (int)y, (int)z - 1) == (int8_t)BlockID::Air) return false;
	if (GetBlockID((int)x, (int)y, (int)z + 1) == (int8_t)BlockID::Air) return false;
	return true;
}

//-----------------------------------------------------------------------------
// ステージを作る
//-----------------------------------------------------------------------------
void BlockFiled::CreateStage(void)
{
	printf("地形生成開始\n");
	printf("MapSize x:%d z:%d y:%d \n", ChunkMaxX * MaxX + MaxX, ChunkMaxY * MaxY + MaxY, ChunkMaxZ * MaxZ + MaxZ);
	printf("ChunkNum :%d \n", ChunkMaxSize);

	uint64_t m_startTime = timeGetTime();

	std::vector<std::thread> threads;
	
	const int threadNum = 10;
	for (int i = 0; i < ActiveaMapNum; i += threadNum)
	{ 
		// 実行
		for (int j = 0; j < threadNum; j++)threads.push_back(std::thread([this, i, j] { this->FiledGenerator(i + j); }));
		// 実行完了待ち
		for (int j = 0; j < threadNum; j++)threads[i + j].join();	
	}

	m_startTime = timeGetTime() - m_startTime;
	printf("地形生成完了:%d\n", m_startTime);

	
}

//-----------------------------------------------------------------------------
// 空間の密度を求める
//-----------------------------------------------------------------------------
inline double BlockFiled::GetSpaceDensity(const float x, const float y, const float z)
{
	PerlinNoise perlin;
	const float sizeMag = 0.0001f;	// 拡大率
	const int octave = 9;			// 複雑さ
	const double persistence = 10.0f;
	return  perlin.octaveNoise(x * sizeMag, y * sizeMag, z * sizeMag, octave, persistence);
}

//-----------------------------------------------------------------------------
// ブロックの表示設定
//-----------------------------------------------------------------------------
void BlockFiled::BlockDisplaySetting(Chunk* stage)
{
	for (int x = 0; x < MaxX; x++)
	{
		for (int z = 0; z < MaxZ; z++)
		{
			for (int y = 0; y < MaxY; y++)
			{
				// ブロックが空なら
				if (stage->blockID[x][y][z] == (int8_t)BlockID::Air) continue;

				// 囲われていないなら次へ
				if (!IsBlockSurround((float)(stage->posx * MaxX + x), (float)(stage->posy * MaxY + y), (float)(stage->posz * MaxZ + z))) continue;
				
				// 非表示設定
				stage->blockID[x][y][z] *= -1;
			}

		}
	}
}

//-----------------------------------------------------------------------------
// 位置情報から配列番号を取得する
//-----------------------------------------------------------------------------
int BlockFiled::GetChunkNum(const float x, const float y, const float z)
{
	int stageX = (int)(x / MaxX);
	int stageY = (int)(y / MaxY);
	int stageZ = (int)(z / MaxZ);

	int stageNum = (stageX * ChunkMaxY * ChunkMaxZ) + stageY + (stageZ * ChunkMaxY);
	return stageNum;
}

//-----------------------------------------------------------------------------
//塊ごとの位置の設定
//-----------------------------------------------------------------------------
inline void BlockFiled::ChunkPositionSetting(void)
{
	for (int i = 0; i < ActiveaMapNum; i++)
	{
		m_stage[i].posx = (int32_t)(i / (ChunkMaxY * ChunkMaxZ));
		m_stage[i].posy = (int32_t)(i % ChunkMaxY);
		m_stage[i].posz = (int32_t)((i / ChunkMaxY) % ChunkMaxZ);
	}
}

//-----------------------------------------------------------------------------
// チャンクのカリングをする
//-----------------------------------------------------------------------------
inline void BlockFiled::ChunksCulling(std::vector<int>* drawNum)
{
	Camera* Camera = m_Scene->GetCamera();
	
#if defined(DEBUG) || defined(_DEBUG)
	if (Input::GetKeyTrigger('O'))m_bCameraFixPos = !m_bCameraFixPos;
	// 更新
	if (m_bCameraFixPos)
	{
		CamPos = Camera->GetCameraPosition();
		fmat = Camera->GetFrustumMatrix();
		Camera->SetFarPlane(120.0f);
		CameraFar = Camera->GetFarPlane();
	}
	if (!m_bCameraFixPos)
	{
		Camera->SetFarPlane(1000.0f);
	}
	
#else
	DirectX::SimpleMath::Vector3 CamPos = Camera->GetCameraPosition();
	DirectX::SimpleMath::Matrix fmat = Camera->GetFrustumMatrix();
	float CameraFar =  Camera->GetFarPlane();
#endif // DEBUG
	
	
	Vector3 Offset = Vector3(MaxX, MaxY, MaxZ) / 2;	// チャンクのオフセット
	float chunkRadius = Offset.Length();			// チャンクの半径
	float radius = CameraFar + chunkRadius;			// カメラのファーとチャンクの半径を足した物

	PLANE plane[6];
	bool  canseeflag[6];
	float ans[6];

	for (int i = 0; i < ActiveaMapNum; i++)
	{
		// チャンクの位置
		Vector3 chancPos = Vector3((float)(m_stage[i].posx * MaxX), (float)(m_stage[i].posy * MaxY),(float)(m_stage[i].posz * MaxZ));
		// チャンクがプレイヤーから遠いなら処理を実行しない
		if (!IsInArea(chancPos + Offset, CamPos, radius))continue;
		// フラスタムカリング
		if (!IsInFrustum(chancPos + Offset, chunkRadius, fmat, ans, canseeflag, plane))continue;
		// 番号を設定
		drawNum->push_back(i);
	}
}

//-----------------------------------------------------------------------------
// 変換
//-----------------------------------------------------------------------------
DirectX::XMINT2 BlockFiled::ConversionBlockData(int32_t x, int32_t y, int32_t z, int8_t blockID)
{
	DirectX::XMINT2 data;

	// 24bit分の位置情報をdata.xに代入
	int32_t tmp = x;
	for (int i = 1; i <= 3; i++)
	{
		tmp = x;
		tmp >>= 8 * (3 - i);
		data.x <<= 8;
		data.x |= (unsigned char)tmp;
	}

	// 24bit分の位置情報をdata.xとdata.yに代入
	tmp = z;
	tmp >>= 8 * 2;
	data.x <<= 8;
	data.x |= (unsigned char)tmp;
	tmp = z;
	for (int i = 1; i <= 2; i++)
	{
		tmp = z;
		tmp >>= 8 * (2 - i);
		data.y <<= 8;
		data.y |= (unsigned char)tmp;
	}

	// 8bit分の位置情報をdata.yに代入
	tmp = y;
	data.y <<= 8;
	data.y |= (int32_t)tmp;

	// 8bit分のブロックID情報をdata.yに代入
	data.y <<= 8;
	data.y |= (int32_t)blockID;

	return data;
}

//-----------------------------------------------------------------------------
// ブロックの描画設定をする
//-----------------------------------------------------------------------------
inline void BlockFiled::DrawBlocks(const std::vector<int> drawNum)
{
	int DrawNum = 0;
	DirectX::XMINT2 pos[MaxDrawBlocks];
	for (int i = 0; i < drawNum.size(); i++)
	{
		// 未生成なら生成する
		if (!m_stage[i].isCreated)FiledGenerator(i);
		// チャンクの位置
		Vector3 chunkPos = Vector3((float)(m_stage[drawNum.at(i)].posx * MaxX), (float)(m_stage[drawNum.at(i)].posy * MaxY), (float)(m_stage[drawNum.at(i)].posz * MaxZ));
		for (int j = 0; j < MaxBlock; j++)
		{
			bool maxNum = MaxDrawBlocks <= DrawNum;
			// 描画数がマックスなら終わる
			if (maxNum)break;

			int x = j / (MaxY * MaxZ);
			int y = j % MaxY;
			int z = (j / MaxY) % MaxZ;
			int8_t blockId = m_stage[drawNum.at(i)].blockID[x][y][z];

			// ブロックがないなら飛ばす
			bool OnBlock = blockId > (int8_t)BlockID::Air;
			if (!OnBlock)continue;
			pos[DrawNum] = ConversionBlockData((int32_t)(chunkPos.x + x), (int32_t)(chunkPos.y + y), (int32_t)(chunkPos.z + z), blockId);
			DrawNum++;// 描画数
		}
	}
	// ブロックの情報を送る
	m_pRenderer->UpdateMatrix(pos);
	// 描画したぶろっく数を保管する
	m_nDrawBlockNum = DrawNum;
}

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void BlockFiled::Init(void)
{	
	//塊ごとの位置の設定
	ChunkPositionSetting();
	
	// ステージを作る
	CreateStage();
	

	// 表示設定
	printf("地形表示設定開始\n");
	for (int i = 0; i < ActiveaMapNum; i++)BlockDisplaySetting(&m_stage[i]);
	printf("地形表示設定完了\n");
	
	// レイアウト設定
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",		0,		DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",			0,		DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",			0,		DXGI_FORMAT_R32G32B32A32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",		0,		DXGI_FORMAT_R32G32_FLOAT,			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BlockData",		0,		DXGI_FORMAT_R32G32_UINT,			1, 0,							 D3D11_INPUT_PER_INSTANCE_DATA,	1},		
	};
		
	AddComponent<Shader>()->Load("asset/shader/VS_Block.cso", "asset/shader/PS_Block.cso",layout,ARRAYSIZE(layout));

	m_pRenderer = AddComponent<InstancingModelRenderer>();
	m_pRenderer->Init("asset\\model\\block\\cube.obj", MaxDrawBlocks);
	m_pRenderer->SetTexture(0, L"asset\\texture\\Blocks\\grass.png");
	m_pRenderer->SetTexture(1, L"asset\\texture\\Blocks\\dirt.png");
	m_pRenderer->SetTexture(2, L"asset\\texture\\Blocks\\stone.png");
	m_pRenderer->SetTexture(3, L"asset\\texture\\Blocks\\sand.png");
		

	m_pCollision = AddComponent<AABB>();
	m_pCollision->Init(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	m_pCollision->SetSimulationFlag(false);
	
	printf("初期化完了\n");
}
//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void BlockFiled::Uninit(void){/*DoNothing*/}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void BlockFiled::Update(void){/*DoNothing*/}

//-----------------------------------------------------------------------------
// 描画前処理
//-----------------------------------------------------------------------------
void BlockFiled::PreDraw()
{
	std::vector<int> drawNum;
	ChunksCulling(&drawNum);
	DrawBlocks(drawNum);
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void BlockFiled::Draw(void){/*DoNothing*/}

//-----------------------------------------------------------------------------
// GUI描画
//-----------------------------------------------------------------------------
void BlockFiled::GUIDraw()
{
	ImGui::Text("DrawBlocks:%d", m_nDrawBlockNum);
}

//-----------------------------------------------------------------------------
// ブロックのあたり判定
//-----------------------------------------------------------------------------
void  BlockFiled::HitBlocks(const int _x, const int _y, const int _z, AABB* aabb)
{
	
	// ブロックがなければあたり判定をしない
	if (GetBlockID(_x, _y, _z) == (int8_t)BlockID::Air)return;
	
	m_pCollision->SetOffset(Vector3((float)_x, (float)_y, (float)_z));
	m_pCollision->IsHit(aabb);
}

//-----------------------------------------------------------------------------
// ブロックのあたり判定
//-----------------------------------------------------------------------------
void BlockFiled::HitBlocks(const DirectX::SimpleMath::Vector3 BlockPos, AABB* aabb)
{
	HitBlocks((int)BlockPos.x, (int)BlockPos.y, (int)BlockPos.z, aabb);
}

//-----------------------------------------------------------------------------
// ブロックIDを取得
//-----------------------------------------------------------------------------
int8_t  BlockFiled::GetBlockID(const int _x, const int _y, const int _z)
{
	int BlockX = _x % MaxX;
	int BlockY = _y % MaxY;
	int BlockZ = _z % MaxZ;
	int stageX = _x / MaxX;
	int stageY = _y / MaxY;
	int stageZ = _z / MaxZ;
	
	int stageNum = (stageX * ChunkMaxY * ChunkMaxZ) + stageY + (stageZ * ChunkMaxY);
	
	// 範囲外
	if (BlockX < 0)return (int8_t)BlockID::Air;
	if (BlockY < 0)return (int8_t)BlockID::Air;
	if (BlockZ < 0)return (int8_t)BlockID::Air;
	if (stageNum < 0)return (int8_t)BlockID::Air;
	if (stageNum >= ActiveaMapNum)return (int8_t)BlockID::Air;
	
	return m_stage[stageNum].blockID[BlockX][BlockY][BlockZ];
}

//-----------------------------------------------------------------------------
// ブロックIDを取得
//-----------------------------------------------------------------------------
int8_t BlockFiled::GetBlockID(const int _stageX, const int _stageY, const int _stageZ, const int _blockX, const int _blockY, const int _blockZ)
{
	int stageNum = (_stageX * ChunkMaxY * ChunkMaxZ) + _stageY + (_stageZ * ChunkMaxY);
	// 範囲外
	if (stageNum < 0)return (int8_t)BlockID::Air;
	if (stageNum > ActiveaMapNum)return (int8_t)BlockID::Air;
	return m_stage[stageNum].blockID[_blockX][_blockY][_blockZ];
}

//-----------------------------------------------------------------------------
// ブロックの設定
//-----------------------------------------------------------------------------
void BlockFiled::SetBlockID(const int _x, const int _y, const int _z, const int8_t blockId)
{
	int BlockX = _x % MaxX;
	int BlockY = _y % MaxY;
	int BlockZ = _z % MaxZ;
	int stageX = _x / MaxX;
	int stageY = _y / MaxY;
	int stageZ = _z / MaxZ;

	int stageNum = (stageX * ChunkMaxY * ChunkMaxZ) + stageY + (stageZ * ChunkMaxY);

	// 範囲外
	bool IsOutX = BlockX < 0;
	bool IsOutY = BlockY < 0;
	bool IsOutZ = BlockZ < 0;
	bool IsOutNum = stageNum < 0 || stageNum >= ActiveaMapNum;
	bool IsOutChunk = IsOutX || IsOutY|| IsOutZ || IsOutNum;
	if (IsOutChunk)return;
	

	m_stage[stageNum].blockID[BlockX][BlockY][BlockZ] = blockId;
	if (blockId != (int8_t)BlockID::Air)return;
	const int offset[6][3] =
	{
		{ 1, 0, 0},
		{-1, 0, 0},
		{ 0, 1, 0},
		{ 0,-1, 0},
		{ 0, 0, 1},
		{ 0, 0,-1},
	};
	for (int i = 0; i < 6; i++)
	{
		// ブロックが空なら
		if (GetBlockID(_x +offset[i][0], _y + offset[i][1], _z + offset[i][2]) == (int8_t)BlockID::Air) continue;

		if (IsBlockSurround((float)(_x + offset[i][0]), (float)(_y + offset[i][1]), (float)(_z + offset[i][2]))) continue;

		DisplayBlock(_x + offset[i][0], _y + offset[i][1], _z + offset[i][2]);
	}
}

//-----------------------------------------------------------------------------
// 非表示ブロックを表示
//-----------------------------------------------------------------------------
void BlockFiled::DisplayBlock(const int _x, const int _y, const int _z)
{
	int BlockX = _x % MaxX;
	int BlockY = _y % MaxY;
	int BlockZ = _z % MaxZ;
	int stageX = _x / MaxX;
	int stageY = _y / MaxY;
	int stageZ = _z / MaxZ;

	int stageNum = (stageX * ChunkMaxY * ChunkMaxZ) + stageY + (stageZ * ChunkMaxY);

	// 範囲外
	bool IsOutX = BlockX < 0;
	bool IsOutY = BlockY < 0;
	bool IsOutZ = BlockZ < 0;
	bool IsOutNum = stageNum < 0 || stageNum >= ActiveaMapNum;
	bool IsOutChunk = IsOutX || IsOutY || IsOutZ || IsOutNum;
	if (IsOutChunk)return;

	if (m_stage[stageNum].blockID[BlockX][BlockY][BlockZ] < 0)m_stage[stageNum].blockID[BlockX][BlockY][BlockZ] *= -1;
}
