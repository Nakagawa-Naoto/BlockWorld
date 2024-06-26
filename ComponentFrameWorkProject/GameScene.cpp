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
// ����������
//-----------------------------------------------------------------------------
void GameScene::Init()
{
	m_pPlayer = AddGameObject<Player>();
	m_pBlock = AddGameObject<Block>();
	m_pFiled = AddGameObject<BlockFiled>();
}

//-----------------------------------------------------------------------------
// �X�V����
//-----------------------------------------------------------------------------
void GameScene::Update()
{ /* DO_NOTHING */}

//-----------------------------------------------------------------------------
// �X�V�O����
//-----------------------------------------------------------------------------
void GameScene::PreUpdate()
{
	// �����蔻��
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
	// �v���C���[�ƃX�e�[�W�̂����蔻��
	for (int i = 0; i < 36; i++)
	{
		int x = (int)playerPos.x + offset[i][0];
		int y = (int)playerPos.y + offset[i][1];
		int z = (int)playerPos.z + offset[i][2];
		m_pFiled->HitBlocks(x, y, z, m_pPlayer->GetCollision());
	}
	// ���I�u���b�N�ƃX�e�[�W�̂����蔻��
	playerPos = m_pBlock->GetPosition();
	for (int i = 0; i < 27; i++)
	{
		int x = (int)playerPos.x + offset[i][0];
		int y = (int)playerPos.y + offset[i][1];
		int z = (int)playerPos.z + offset[i][2];
		m_pFiled->HitBlocks(x, y, z, m_pBlock->GetCollision());
	}
}