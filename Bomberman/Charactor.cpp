#include "Charactor.h"
#include"DxLib.h"
#include"MapCtrl.h"
#include"EnemyMgr.h"
#include "Player.h"
#include "ImageMgr.h"

int Charactor::GetMoveCntNextScreen()
{
	int cnt = 0;
	int debugCnt = 0;
	if (dammy)
	{
		dammy->SetMoveFlg(true);
		if (lpMapCtrl->GetMapData(dammy->GetPos()) != MAP_SCREEN)
		{
			dammy->SetDammyFlg(true);
			while (1)
			{
				debugCnt++;
				dammy->Update();
				if (!dammy->GetMoveFlg())
				{
					dammy->SetMoveFlg(true);
					++cnt;
					if (lpMapCtrl->GetMapData(dammy->GetPos()) == MAP_SCREEN)
					{
						nextScreenPos = dammy->GetPos();
						break;
					}
				}
			}
			dammy->SetDammyFlg(false);
		}
		dammy->SetDir(drawDir);
		dammy->SetPos(pos);
	}
	return cnt;
}

Charactor::Charactor(const VECTOR2& drawOffset, char(&keyData)[256], char(&keyDataOld)[256], bool useDammy) : Object(drawOffset,keyData, keyDataOld)
{
	dammy = nullptr;
	if (useDammy)
	{
		dammy.reset(new Charactor(drawOffset, keyData, keyDataOld, false));
	}
}


Charactor::~Charactor()
{
	dammy = nullptr;
}

void Charactor::SetMove()
{
	if (moveFlg)
	{
		if (dammy)
		{
			dammyOn = false;
		}
		if (pos.x % lpGameTask->chipSize.x == 0 && pos.y % lpGameTask->chipSize.y == 0)
		{
			auto moveDir = GetRightHandDir(drawDir);
			int check[4];

			int cnt = 0;
			while (1)
			{
				if (dammy)
				{
					if (lpMapCtrl->IsMove(pos, moveDir))
					{
						break;
					}
				}
				else
				{
					if (lpMapCtrl->IsMove(pos, moveDir, dammyOn))
					{
						break;
					}
				}
				check[moveDir] = 1;
				for (int i = 0; i < 3; ++i)
				{
					moveDir = GetRightHandDir(moveDir);
				}
				cnt++;
				if (cnt >= 4)
				{
					moveDir = DIR_DEATH;
					break;
				}
			}
			//敵サーチ
			

			//決定した移動方向に基づいて移動入力
			if (moveDir != DIR_DEATH)
			{
				drawDir = moveDir;
			}
			switch (moveDir)
			{
			case DIR_DOWN:
				moveKeyID = KEY_INPUT_NUMPAD2;
				break;
			case DIR_LEFT:
				moveKeyID = KEY_INPUT_NUMPAD4;
				break;
			case DIR_UP:
				moveKeyID = KEY_INPUT_NUMPAD8;
				break;
			case DIR_RIGHT:
				moveKeyID = KEY_INPUT_NUMPAD6;
				break;
			default:
				moveKeyID = KEY_INPUT_NUMPAD5;
			}
		
		}
		switch (moveKeyID)
		{
		case KEY_INPUT_NUMPAD2:
			pos.y += speed;
			break;
		case KEY_INPUT_NUMPAD4:
			pos.x -= speed;
			break;
		case KEY_INPUT_NUMPAD6:
			pos.x += speed;
			break;
		case KEY_INPUT_NUMPAD8:
			pos.y -= speed;
			break;
		}
		if (pos.x % lpGameTask->chipSize.x == 0 && pos.y % lpGameTask->chipSize.y == 0 && moveKeyID != 0)
		{
			if (dammy)
			{
				dammy->SetPos(pos);
				dammy->SetDir(drawDir);
				--nextScreenCnt;
				
				if (nextScreenCnt <= 0)
				{
					if (lpMapCtrl->GetMapData(pos) != MAP_SCREEN)
					{
						nextScreenCnt = GetMoveCntNextScreen();
					}
				}
				lpMapCtrl->SetNextMap(nextScreenCnt);
				
				
			}
			dammyMoveCnt = 0;
			moveFlg = false;
		}
	}
	if (dammyOn)
	{
		if (dammy)
		{
			dammy->Update();
			if (!dammy->GetMoveFlg())
			{
				auto isMoveCnt = (NUM_READ_MOVE < nextScreenCnt ? NUM_READ_MOVE : nextScreenCnt);
				if (dammyMoveCnt < isMoveCnt)
				{
					dammy->SetMoveFlg(true);
				}
				dammyMoveCnt++;
			}
			if (dammyMoveCnt > NUM_READ_MOVE + MOVE_WAIT_CNT)
			{
				dammyMoveCnt = 0;
				dammy->SetPos(pos);
				dammy->SetDir(drawDir);
				dammy->SetMoveFlg(true);
			}
		}
	}
}

void Charactor::Draw() const
{
	if (imageName.empty())
	{
		return;
	}
	DrawRotaGraph(this->pos.x + drawOffset.x + lpGameTask->chipSize.x/2, this->pos.y + drawOffset.y + lpGameTask->chipSize.y/2, 1.0f,angle[drawDir],IMAGE_ID(imageName)[chipOffset.y * divCnt.x + chipOffset.x], true);

	if (dammyOn)
	{
		if (dammy)
		{
			DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			dammy->Draw();
			DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 128);	DrawFormatString(0, 20, 0xffffff, "スクリーンまであと %d マス", nextScreenCnt);
			DrawFormatString(0, 20, 0xffffff, "スクリーンまであと %d マス", nextScreenCnt);
		}
		DxLib::SetDrawBlendMode(DX_BLENDMODE_ADD, 128);
		DrawBox(nextScreenPos.x + drawOffset.x, nextScreenPos.y + drawOffset.y,
			nextScreenPos.x + drawOffset.x + lpGameTask->chipSize.x, nextScreenPos.y + drawOffset.y + lpGameTask->chipSize.y,
			0x0000ff, true);
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 128);
	}
}

void Charactor::SetDammyFlg(bool flg)
{
	dammyOn = flg;
	if (dammy)
	{
		dammy->SetMoveFlg(flg);
	}
}

void Charactor::Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCnt, VECTOR2 chipOffset, int speed, int animSpeed, int stateAnimDiv)
{
	Object::Init(fileName, divSize, divCnt, chipOffset, speed, animSpeed, stateAnimDiv);
	if (dammy)
	{
		dammy->Init(fileName, divSize, divCnt, chipOffset, speed, animSpeed, stateAnimDiv);
	}
}
