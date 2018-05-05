#include "CharactorMgr.h"
#include "Charactor.h"
#include "EnemyMgr.h"
#include "MapCtrl.h"

CharactorMgr* CharactorMgr::s_Instance;

void CharactorMgr::AddCharactor(Charactor * charactor)
{
	charactorList.push_back(std::shared_ptr<Charactor>(charactor));
}

void CharactorMgr::SetMoveFlg(bool moveFlg)
{
	for (auto chara : charactorList)
	{
		chara->SetMoveFlg(moveFlg);
	}
	state = moveFlg;
}

void CharactorMgr::SetOnDammyFlg(bool flg)
{
	for (auto chara : charactorList)
	{
		chara->SetDammyFlg(flg);
	}
}

void CharactorMgr::Update()
{
	bool moveFlg = false;

//	if(state)
	{
		for (auto chara = charactorList.begin(); chara != charactorList.end(); ++chara)
		{
			if (!(*chara))
			{
				charactorList.erase(chara);
			}
		}
		for (auto chara : charactorList)
		{
			chara->Update();
			if (!moveFlg&& chara->GetMoveFlg())
			{
				moveFlg = true;
			}
		}
	}

	if (state){
		if (!moveFlg)
		{
			//“G‚ÌˆÚ“®ƒtƒ‰ƒO‚ðOn‚É
			lpEnemyMgr->SetMoveFlg(true);
			state = false;
		}
	}
}

void CharactorMgr::Draw()
{
	for (auto chara: charactorList)
	{
		chara->UpdateAnim();
		chara->Draw();
	}
}

bool CharactorMgr::GetState() const
{
	return state;
}


void CharactorMgr::PushedWall(DRAW_DIR pushDir)
{
	for (auto chara : charactorList)
	{
		if (lpMapCtrl->GetMapData(chara->GetPos()) == MAP_SCREEN)
		{
			VECTOR2 tmp;
			switch(pushDir)
			{
			case DIR_DOWN:
				tmp.y = lpGameTask->chipSize.y;
				break;
			case DIR_UP:
				tmp.y = -lpGameTask->chipSize.y;
				break;
			case DIR_LEFT:
				tmp.x = -lpGameTask->chipSize.x;
				break;
			case DIR_RIGHT:
				tmp.x = lpGameTask->chipSize.x;
				break;
			}
			chara->SetPos(chara->GetPos() + tmp);
		}
	}
}

bool CharactorMgr::IsCharactor(const VECTOR2 & pos) const
{
	for (auto chara : charactorList)
	{
		if (chara->GetPos() == pos)
		{
			return true;
		}
	}
	return false;
}

CharactorMgr::CharactorMgr()
{
}


CharactorMgr::~CharactorMgr()
{
}
