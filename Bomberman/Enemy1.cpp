#include "Enemy1.h"
#include "Dxlib.h"
#include "MapCtrl.h"



Enemy1::Enemy1(const VECTOR2& offset, char(&keyData)[256], char(&keyDataOld)[256]) : Object(offset, keyData, keyDataOld)
{
	drawDir = DRAW_DIR::DIR_DOWN;
	moveWaitTime = 0;

}


Enemy1::~Enemy1()
{
}


void Enemy1::SetMove()
{

	if (pos.x % lpGameTask->chipSize.x == 0 && pos.y % lpGameTask->chipSize.y == 0)
	{
		

		moveWaitTime ++;
		moveKeyID = 0;
		if (moveWaitTime > MOVE_WAIT_TIME)
		{
			moveWaitTime = 0;
			drawDir = (DRAW_DIR)(rand() % DIR_DEATH);
			switch (drawDir)
			{
			case DIR_LEFT:
				moveKeyID = KEY_INPUT_NUMPAD4;
				break;
			case DIR_RIGHT:
				moveKeyID = KEY_INPUT_NUMPAD6;
				break;
			case DIR_UP:
				moveKeyID = KEY_INPUT_NUMPAD8;
				break;
			case DIR_DOWN:
				moveKeyID = KEY_INPUT_NUMPAD2;
				break;
			}
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
		case MAP_ITEM_BOMB:
		case MAP_ITEM_FIRE:
		case MAP_ITEM_SWITCH:
		case MAP_ITEM_SHOES:
			if (lpMapCtrl->GetItemHideFlag(this->pos, drawDir))
			{
				moveKeyID = 0;
			}
			break;
		default:
			break;
		}*/
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