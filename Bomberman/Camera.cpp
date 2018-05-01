#include "Camera.h"
#include"DxLib.h"
#include "MapCtrl.h"

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
		if (keyData[KEY_INPUT_NUMPAD4] & ~keyDataOld[KEY_INPUT_NUMPAD4] )
		{
			drawDir = DIR_LEFT;
			moveKeyID = KEY_INPUT_NUMPAD4;
		}
		else if (keyData[KEY_INPUT_NUMPAD6] & ~keyDataOld[KEY_INPUT_NUMPAD6])
		{
			drawDir = DIR_RIGHT;
			moveKeyID = KEY_INPUT_NUMPAD6;
		}
		else if (keyData[KEY_INPUT_NUMPAD2] & ~keyDataOld[KEY_INPUT_NUMPAD2])
		{
			drawDir = DIR_DOWN;
			moveKeyID = KEY_INPUT_NUMPAD2;
		}
		else if (keyData[KEY_INPUT_NUMPAD8] & ~keyDataOld[KEY_INPUT_NUMPAD8])
		{
			drawDir = DIR_UP;
			moveKeyID = KEY_INPUT_NUMPAD8;
		}
		else
		{
			moveKeyID = 0;
		}
		if (lpMapCtrl->GetMapData(pos, drawDir) == MAP_SCREEN)
		{
			moveKeyID = 0;
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
		}
	}
}