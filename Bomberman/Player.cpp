#include "Player.h"
#include "MapCtrl.h"
#include "DxLib.h"


Player::Player(const VECTOR2& offset, char(&keyData)[256], char(&keyDataOld)[256]) : Object(offset,keyData,keyDataOld)
{
	fireRange = 1;
}


Player::~Player()
{
}

void Player::SetMove()
{

	if (pos.x % lpGameTask->chipSize.x == 0 && pos.y % lpGameTask->chipSize.y == 0)
	{
		if (keyData[KEY_INPUT_NUMPAD4])
		{
			drawDir = DIR_LEFT;
			moveKeyID = KEY_INPUT_NUMPAD4;
		}
		else if (keyData[KEY_INPUT_NUMPAD6])
		{
			drawDir = DIR_RIGHT;
			moveKeyID = KEY_INPUT_NUMPAD6;
		}
		else if (keyData[KEY_INPUT_NUMPAD2])
		{
			drawDir = DIR_DOWN;
			moveKeyID = KEY_INPUT_NUMPAD2;
		}
		else if (keyData[KEY_INPUT_NUMPAD8])
		{
			drawDir = DIR_UP;
			moveKeyID = KEY_INPUT_NUMPAD8;
		}
		else if (keyData[KEY_INPUT_Z] ^ keyDataOld[KEY_INPUT_Z] & keyData[KEY_INPUT_Z])
		{
		}
		else
		{
			moveKeyID = 0;
		}
	}
	if (moveKeyID)
	{
		state = ST_WALK;
		/*
		switch (lpMapCtrl->GetMapData(this->pos, drawDir))
		{
		case MAP_WALL1:
		case MAP_WALL2:
		case MAP_BLOCK:
		case MAP_BOMB:
			moveKeyID = 0;
			break;
		case MAP_ITEM_BOMB:
			if (lpMapCtrl->GetItemHideFlag(this->pos, drawDir))
			{
				moveKeyID = 0;
			}
			else
			{
				lpBombCtrl->SetBombMaxNum(lpBombCtrl->GetBombMaxNum() + 1);
				lpMapCtrl->SetMapData(MAP_NON, this->pos, drawDir);
			}
			break;
		case MAP_ITEM_FIRE:
			if (lpMapCtrl->GetItemHideFlag(this->pos, drawDir))
			{
				moveKeyID = 0;
			}
			else
			{
				if (fireRange < MAX_FIRE_RANGE)
				{
					fireRange++;
				}
				lpMapCtrl->SetMapData(MAP_NON, this->pos, drawDir);
			}
			break;
		case MAP_ITEM_SHOES:
			if (lpMapCtrl->GetItemHideFlag(this->pos, drawDir))
			{
				moveKeyID = 0;
			}
			else
			{
				lpMapCtrl->SetMapData(MAP_NON, this->pos, drawDir);
			}
			break;
		case MAP_ITEM_SWITCH:
			if (lpMapCtrl->GetItemHideFlag(this->pos, drawDir))
			{
				moveKeyID = 0;
			}
			else
			{
				lpMapCtrl->SetMapData(MAP_NON, this->pos, drawDir);
			}
			break;
		default:
			break;
		}
		*/
	}
	else
	{
		state = ST_STAND;
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
}


