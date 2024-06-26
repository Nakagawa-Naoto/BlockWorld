#include "Title.h"
#include "Input.h"
#include "TestScene.h"
#include "BlockFiled.h"
#include "Sprite.h"

using namespace DirectX::SimpleMath;

void Title::Shoot(void)
{
	Sphere* sphere = new Sphere();
	sphere->InitBase();


	// 初期位置設定
	sphere->SetPosition(cam->GetPosition());

	// 投げる方向
	float speed = 10.0f+rand()%10;
	Vector3 shrow = cam->GetTarget();
	shrow.y += 20;
	shrow -= cam->GetPosition();
	shrow.Normalize();
	shrow *= speed;
	sphere->SetVelocity(shrow);

	// 加速度設定
	sphere->SetAcceleration(Vector3(0.0f, -9.8f, 0.0f));
	shperes.push_back(sphere);
}

void Title::CreateParticle(const float x, const float y, const float z)
{
	Particle* par = new Particle();
	par->Init(15 + rand() % 20, Vector3(x, y, z));
	particle.push_back(par);
}

void Title::Init()
{
	
	cam = AddGameObject<CameraObj>();
	cam->SetFlag(true);
	field = AddGameObject<TilteField>();
	//AddGameObject<BlockFiled>();
	Scene::Init();
}

void Title::Uninit()
{
	for (auto tmp = shperes.begin(); tmp != shperes.end(); tmp++)
	{
		(*tmp)->UninitBase();
		tmp = shperes.erase(tmp);
	}
	shperes.clear();
	Scene::Uninit();
}

void Title::Update()
{
	for (auto tmp = shperes.begin(); tmp != shperes.end(); tmp++)
	{
		(*tmp)->UpdateBase();
		for (int j = 0; j < MaxBlock2; j++)
		{
			int x = j / (MaxY2 * MaxZ2);
			int y = j % MaxY2;
			int z = (j / MaxY2) % MaxZ2;

			// ブロックがないなら次のループへ
			if (field->ChackBlock(x, y, z) == 0)continue;
			// 当たっていないなら次のループへ
			if (!(*tmp)->HitCheack((float)x, (float)y, (float)z))continue;
			
			// ブロックを消す	
			field->ChangeBlock(x, y, z, 0);

			// パーティクルを出す
			CreateParticle((float)x, (float)y, (float)z);
		}
		
	}
	for (auto tmp = shperes.begin(); tmp != shperes.end();)
	{
		if ((*tmp)->m_hit) 
		{ 
			
			tmp = shperes.erase(tmp); 
		}
		else
		{
			tmp++;
		}
	}
	for (auto tmp = particle.begin(); tmp != particle.end();)
	{
		(*tmp)->UpdateBase();
		if ((*tmp)->end)
		{
			(*tmp)->UninitBase();
			tmp = particle.erase(tmp);
		}
		else
		{
			tmp++;
		}
	}
	static int ot = 0;
	ot++;
	if (Input::GetKeyTrigger(VK_SPACE)||ot%200 == 199)Shoot();

	if (Input::GetKeyTrigger(VK_RETURN))
	{
		//mainScene
		Scene::LoadScene(new TestScene);
	}


	Scene::Update();
}

void Title::Draw()
{
	for (auto tmp = shperes.begin(); tmp != shperes.end(); tmp++)
	{
		(*tmp)->DrawBase(Matrix::Identity);
	}
	for (auto tmp = particle.begin(); tmp != particle.end(); tmp++)
	{
		(*tmp)->DrawBase(Matrix::Identity);
	}
	Scene::Draw();
}
