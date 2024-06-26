//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "GameScene.h"
#include "Input.h"
#include "BlockFiled.h"
#include "Player.h"
#include "Block.h"
#include "AABB.h"

///////////////////////////////////////////////////////////////////////////////
// GameScene class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void GameScene::Init()
{
	m_pPlayer = AddGameObject<Player>();
	m_pBlock = AddGameObject<Block>();
	m_pFiled = AddGameObject<BlockFiled>();
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void GameScene::Update()
{ /* DO_NOTHING */}

//-----------------------------------------------------------------------------
// 更新前処理
//-----------------------------------------------------------------------------
void GameScene::PreUpdate()
{
	// あたり判定
	m_pBlock->GetCollision()->Reset();
	m_pPlayer->GetCollision()->Reset();
	static const int offset[36][3] =
	{
		{-1,-1,-1},{ 0,-1,-1},{ 1,-1,-1},{-1,-1, 0},{ 0,-1, 0},{ 1,-1, 0},{-1,-1, 1},{ 0,-1, 1},{ 1,-1, 1},
		{-1, 0,-1},{ 0, 0,-1},{ 1, 1,-1},{-1, 0, 0},{ 0, 0, 0},{ 1, 1, 0},{-1, 0, 1},{ 0, 0, 1},{ 1, 1, 1},
		{-1, 1,-1},{ 0, 1,-1},{ 1, 1,-1},{-1, 1, 0},{ 0, 1, 0},{ 1, 1, 0},{-1, 1, 1},{ 0, 1, 1},{ 1, 1, 1},
		{-1, 2,-1},{ 0, 2,-1},{ 1, 2,-1},{-1, 2, 0},{ 0, 2, 0},{ 1, 2, 0},{-1, 2, 1},{ 0, 2, 1},{ 1, 2, 1},
	};
	DirectX::SimpleMath::Vector3 playerPos = m_pPlayer->GetPosition();
	// プレイヤーとステージのあたり判定
	for (int i = 0; i < 36; i++)
	{
		int x = (int)playerPos.x + offset[i][0];
		int y = (int)playerPos.y + offset[i][1];
		int z = (int)playerPos.z + offset[i][2];
		m_pFiled->HitBlocks(x, y, z, m_pPlayer->GetCollision());
	}
	// 動的ブロックとステージのあたり判定
	playerPos = m_pBlock->GetPosition();
	for (int i = 0; i < 27; i++)
	{
		int x = (int)playerPos.x + offset[i][0];
		int y = (int)playerPos.y + offset[i][1];
		int z = (int)playerPos.z + offset[i][2];
		m_pFiled->HitBlocks(x, y, z, m_pBlock->GetCollision());
	}
}