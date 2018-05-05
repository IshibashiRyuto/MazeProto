#include "EnemyMgr.h"
#include "Enemy.h"
#include"CameraMgr.h"
#include "CharactorMgr.h"
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
	bool moveFlg = false;
	//if (state)
	{
		lpMapCtrl->ClearObjMap(MAP_ENEMY);
		for (auto enemy = enemyList.begin(); enemy != enemyList.end(); ++enemy)
		{
			if (!(*enemy))
			{
				enemyList.erase(enemy);
			}
		}
		for (auto enemy : enemyList)
		{
			enemy->Update();
			lpMapCtrl->SetObjMap(enemy->GetPos(), MAP_ENEMY);
			if (!moveFlg && enemy->GetMoveFlg())
			{
				moveFlg = true;
			}
		}
	}
	if (state)
	{
		if (!moveFlg)
		{
			//カメラの移動フラグをOnに
			lpCameraMgr->SetMoveFlg(true);
			//キャラクタの移動ダミーのフラグをOnに
			lpCharactorMgr->SetOnDammyFlg(true);
			this->SetOnDammyFlg(true);
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

void EnemyMgr::StopEnemy(const VECTOR2 & pos, int stopCnt) const
{
	for (auto enemy : enemyList)
	{
		if (enemy->GetPos() == pos)
		{
			enemy->SetStopCnt(stopCnt);
		}
	}
}

void EnemyMgr::SetOnDammyFlg(bool flg)
{
	for (auto enemy : enemyList)
	{
		enemy->SetDammyFlg(flg);
	}
}
