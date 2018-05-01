#include "Enemy.h"
#include "DxLib.h"
#include "MapCtrl.h"

Enemy::Enemy(const VECTOR2& drawOffset, char(&_keyData)[256], char(&_keyDataOld)[256]) : Object( drawOffset, _keyData, _keyDataOld)
{
}


Enemy::~Enemy()
{
}

void Enemy::SetMove()
{
	if (moveFlg)
	{
		if (pos.x % lpGameTask->chipSize.x == 0 && pos.y % lpGameTask->chipSize.y == 0)
		{

			auto moveDir = GetRightHandDir(drawDir);
			while (!lpMapCtrl->IsMove(pos, moveDir))
			{
				for (int i = 0; i < 3; ++i)
				{
					moveDir = GetRightHandDir(moveDir);
				}
			}
			drawDir = moveDir;
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
			moveFlg = false;
		}
	}
}

void Enemy::UpdateAnim()
{
}
