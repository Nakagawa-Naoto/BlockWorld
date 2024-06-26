#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Perlinnoise.h"
#include "GameObject.h"
#include "InstanceModelRenderer.h"
#include "ModelRenderer.h"
#include <SimpleMath.h>
#include <vector>

constexpr int MaxX = 16;
constexpr int MaxZ = 16;
constexpr int MaxY = 16;
constexpr int MaxBlock = MaxX * MaxZ * MaxY;
constexpr int ChunkMaxX = 16;
constexpr int ChunkMaxY = 8;
 constexpr int ChunkMaxZ = 16;
constexpr int ChunkMaxSize = ChunkMaxX * ChunkMaxY * ChunkMaxZ;
constexpr int ActiveaMapNum = ChunkMaxSize;
constexpr int MaxDrawBlocks = MaxBlock * 30;

//-----------------------------------------------------------------------------
// enum class
//-----------------------------------------------------------------------------
enum class BlockID : int8_t
{
	Air = 0,
	Grass,
	Dirt,
	Stone,
	Sand,
};

///////////////////////////////////////////////////////////////////////////////
// Chunk struct
///////////////////////////////////////////////////////////////////////////////
struct Chunk
{
	int32_t posx,posz,posy;
	int8_t blockID[MaxX][MaxY][MaxZ];
	bool isCreated;
};

///////////////////////////////////////////////////////////////////////////////
// BlockFiled class
///////////////////////////////////////////////////////////////////////////////
class BlockFiled :public GameObject
{
	//=========================================================================
	// list of friend classes and methods.
	//=========================================================================
	/* NOTHING */

private:
	//=========================================================================
	// private variables.
	//=========================================================================

#if defined(DEBUG) || defined(_DEBUG)
	// デバッグ用
	DirectX::SimpleMath::Vector3 CamPos;
	DirectX::SimpleMath::Matrix fmat;
	float CameraFar;
	bool m_bCameraFixPos = true;// カリング用カメラ位置固定
#endif // DEBUG

	
	Chunk m_stage[ActiveaMapNum];
	InstancingModelRenderer* m_pRenderer;
	ModelRenderer* m_pGroundRender;
	int m_nDrawBlockNum;
	class AABB* m_pCollision;
	
	//=========================================================================
	// private methods
	//=========================================================================
	inline bool IsInArea(const DirectX::SimpleMath::Vector3 PointPosition, const DirectX::SimpleMath::Vector3 AreaPosition,const float areaRange);
	
	inline BlockID GetBlockDataID(const double spaceDensity,const int groundHeight,const int y);
	inline double GetSurfaceHeight(const float x, const float z);
	inline double GetMountainHeight(const float x, const float z);
	inline int GetGroundHeight(const float x, const float z);
	inline double GetSpaceDensity(const float x, const float y, const float z);
	int GetChunkNum(const float x, const float y, const float z);

	inline bool IsBlockSurround(const float x, const float y, const float z);
	void CreateStage(void);
	void CreateGround(Chunk* stage, const int x, const int z);
	void FiledGenerator(Chunk* stage);
	void FiledGenerator(const int num);
	void BlockDisplaySetting(Chunk* stage);
	inline void ChunkPositionSetting(void);
	inline void ChunksCulling(std::vector<int>* drawNum);
	inline void DrawBlocks(const std::vector<int> drawNum);
	void DisplayBlock(const int _x, const int _y, const int _z);

	DirectX::XMINT2 ConversionBlockData(int32_t x, int32_t y, int32_t z, int8_t blockID);

public:
	//=========================================================================
	// public variables.
	//=========================================================================
	/* NOTHING */

	//=========================================================================
	// public methods.
	//=========================================================================
	void Init()override;
	void Uninit()override;
	void Update()override;
	void PreDraw()override;
	void Draw()override;
	void GUIDraw()override;
	void HitBlocks(const int _x, const int _y, const int _z, AABB* aabb);
	void HitBlocks(const DirectX::SimpleMath::Vector3 BlockPos, AABB* aabb);
	int8_t GetBlockID(const int _x, const int _y, const int _z);
	int8_t GetBlockID(const int _stageX, const int _stageY, const int _stageZ ,const int _blockX,const int _blockY, const int _blockZ);
	void SetBlockID(const int _x, const int _y, const int _z,const int8_t blockId);
};
