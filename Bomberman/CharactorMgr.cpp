#include "CharactorMgr.h"
#include "Charactor.h"
#include "EnemyMgr.h"

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

void CharactorMgr::Update()
{
	if(state)
	{
		for (auto chara = charactorList.begin(); chara != charactorList.end(); ++chara)
		{
			if (!(*chara))
			{
				charactorList.erase(chara);
			}
		}
		bool moveFlg = false;
		for (auto chara : charactorList)
		{
			chara->Update();
			if (!moveFlg&& chara->GetMoveFlg())
			{
				moveFlg = true;
			}
		}
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
