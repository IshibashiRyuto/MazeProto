#include "Enemy.h"
#include "DxLib.h"
#include "MapCtrl.h"
#include "ImageMgr.h"

Enemy::Enemy(const VECTOR2& drawOffset, char(&_keyData)[256], char(&_keyDataOld)[256], bool useDammy) : Object( drawOffset, _keyData, _keyDataOld)
{
	dammy = nullptr;
	if (useDammy)
	{
		dammy.reset(new Enemy(drawOffset, keyData, keyDataOld, false));
	}
}


Enemy::~Enemy()
{
}

void Enemy::SetMove()
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
			//“GƒT[ƒ`


			//Œˆ’è‚µ‚½ˆÚ“®•ûŒü‚ÉŠî‚Ã‚¢‚ÄˆÚ“®“ü—Í
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
				if (dammyMoveCnt < NUM_READ_MOVE)
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

void Enemy::UpdateAnim()
{
}


void Enemy::Draw() const
{
	if (imageName.empty())
	{
		return;
	}
	DrawRotaGraph(this->pos.x + drawOffset.x + lpGameTask->chipSize.x / 2, this->pos.y + drawOffset.y + lpGameTask->chipSize.y / 2, 1.0f, angle[drawDir], IMAGE_ID(imageName)[chipOffset.y * divCnt.x + chipOffset.x], true);


	if (dammyOn)
	{
		if (dammy)
		{
			DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			dammy->Draw();
			DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 128);
		}
	}
}

void Enemy::SetDammyFlg(bool flg)
{
	dammyOn = flg;
	if (dammy)
	{
		dammy->SetMoveFlg(flg);
	}
}

void Enemy::Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCnt, VECTOR2 chipOffset, int speed, int animSpeed, int stateAnimDiv)
{
	Object::Init(fileName, divSize, divCnt, chipOffset, speed, animSpeed, stateAnimDiv);
	if (dammy)
	{
		dammy->Init(fileName, divSize, divCnt, chipOffset, speed, animSpeed, stateAnimDiv);
	}
}