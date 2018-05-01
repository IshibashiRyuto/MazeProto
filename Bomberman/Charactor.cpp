#include "Charactor.h"
#include"DxLib.h"
#include"MapCtrl.h"
#include"EnemyMgr.h"

Charactor::Charactor(const VECTOR2& drawOffset, char(&keyData)[256], char(&keyDataOld)[256]) : Object(drawOffset,keyData, keyDataOld)
{
}


Charactor::~Charactor()
{
}

void Charactor::SetMove()
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
			//�G�T�[�`
			

			//���肵���ړ������Ɋ�Â��Ĉړ�����
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