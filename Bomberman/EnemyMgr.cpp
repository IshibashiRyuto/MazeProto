#include "EnemyMgr.h"
#include "Enemy.h"
#include"CameraMgr.h"
#include"MapCtrl.h"

EnemyMgr* EnemyMgr::s_Instance;


EnemyMgr::EnemyMgr()
{
}


EnemyMgr::~EnemyMgr()
{
}

void EnemyMgr::AddEnemy(Enemy * enemy)
{
	enemyList.push_back(std::shared_ptr<Enemy>(enemy));
}

void EnemyMgr::SetMoveFlg(bool moveFlg)
{
	for (auto enemy : enemyList)
	{
		enemy->SetMoveFlg(moveFlg);
	}
	state = moveFlg;
}

void EnemyMgr::Update()
{
	if (state)
	{
		lpMapCtrl->ClearObjMap(MAP_ENEMY);
		for (auto enemy = enemyList.begin(); enemy != enemyList.end(); ++enemy)
		{
			if (!(*enemy))
			{
				enemyList.erase(enemy);
			}
		}
		bool moveFlg = false;
		for (auto enemy : enemyList)
		{
			enemy->Update();
			lpMapCtrl->SetObjMap(enemy->GetPos(), MAP_ENEMY);
			if (!moveFlg && enemy->GetMoveFlg())
			{
				moveFlg = true;
			}
		}
		if (!moveFlg)
		{
			//ƒJƒƒ‰‚ÌˆÚ“®ƒtƒ‰ƒO‚ðOn‚É
			lpCameraMgr->SetMoveFlg(true);
			state = false;
		}
	}
}

void EnemyMgr::Draw()
{
	for (auto enemy : enemyList)
	{
		enemy->UpdateAnim();
		enemy->Draw();
	}
}

bool EnemyMgr::GetState() const
{
	return state;
}

bool EnemyMgr::IsEnemy(const VECTOR2& pos) const
{
	for (auto enemy : enemyList)
	{
		if (enemy->GetPos() == pos)
		{
			return true;
		}
	}
	return false;
}
