#include "Camera.h"
#include"DxLib.h"
#include "MapCtrl.h"
#include "CharactorMgr.h"
#include"EnemyMgr.h"

Camera::Camera(const VECTOR2& offset, char(&keyData)[256], char(&keyDataOld)[256]) : Object(offset, keyData, keyDataOld)
{
	MapCtrl::GetInstance()->SetCamera(this);
	moveFlg = true;
}


Camera::~Camera()
{
}

void Camera::SetMove()
{

	if (pos.x % lpGameTask->chipSize.x == 0 && pos.y % lpGameTask->chipSize.y == 0)
	{
		auto mapData = lpMapCtrl->GetMapData(pos);
		bool pushFlg = false;
		if (keyData[KEY_INPUT_NUMPAD4] & ~keyDataOld[KEY_INPUT_NUMPAD4] )
		{
			drawDir = DIR_LEFT;
			moveKeyID = KEY_INPUT_NUMPAD4;
			if ( (mapData & MOVABLE_LEFT) == 0)
			{
				pushFlg = true;
			}
		}
		else if (keyData[KEY_INPUT_NUMPAD6] & ~keyDataOld[KEY_INPUT_NUMPAD6])
		{
			drawDir = DIR_RIGHT;
			moveKeyID = KEY_INPUT_NUMPAD6;
			if ( (mapData & MOVABLE_RIGHT) == 0)
			{
				pushFlg = true;
			}
		}
		else if (keyData[KEY_INPUT_NUMPAD2] & ~keyDataOld[KEY_INPUT_NUMPAD2])
		{
			drawDir = DIR_DOWN;
			moveKeyID = KEY_INPUT_NUMPAD2;
			if ( (mapData & MOVABLE_DOWN) == 0)
			{
				pushFlg = true;
			}
		}
		else if (keyData[KEY_INPUT_NUMPAD8] & ~keyDataOld[KEY_INPUT_NUMPAD8])
		{
			drawDir = DIR_UP;
			moveKeyID = KEY_INPUT_NUMPAD8;
			if ( (mapData & MOVABLE_UP) == 0)
			{
				pushFlg = true;
			}
		}
		else if (keyData[KEY_INPUT_SPACE] & ~keyDataOld[KEY_INPUT_SPACE])
		{
			moveKeyID = KEY_INPUT_SPACE;
			lpEnemyMgr->StopEnemy(pos, 3);
		}
		else
		{
			moveKeyID = 0;
		}
		auto nextMapData = lpMapCtrl->GetMapData(pos, drawDir);
		if (nextMapData == MAP_SCREEN || nextMapData == MAP_WALL)
		{
			moveKeyID = 0;
		}
		if (pushFlg)
		{
			auto pushDir = this->GetRightHandDir(this->GetRightHandDir(drawDir));
			//lpCharactorMgr->PushedWall(pushDir);
		}

		if (keyData[KEY_INPUT_Z])
		{
			lpMapCtrl->SetNextDraw(true);
		}
		else
		{
			lpMapCtrl->SetNextDraw(false);
		}

	}
	if (moveFlg)
	{
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
			moveCnt++;
		}
	}
}

void Camera::Draw()
{
	Object::Draw();
	DrawFormatString(0, 0, 0xffffff, "éËêî: %d ",moveCnt);
}
