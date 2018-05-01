#include "GameTask.h"
#include "EditCursor.h"
#include "VECTOR2.h"
#include "DxLib.h"
#include "ImageMgr.h"
#include <stdio.h>
#include "MapCtrl.h"
#include "CameraMgr.h"
#include "EnemyMgr.h"
#include "CharactorMgr.h"

GameTask* GameTask::s_Instance = nullptr;


GameTask::GameTask()
{
	mode = GMODE_SYS_INIT;
	GtskTbl[GMODE_SYS_INIT] = &GameTask::SysInit;
	GtskTbl[GMODE_GAME_INIT] = &GameTask::GameInit;
	GtskTbl[GMODE_GAME_MAIN] = &GameTask::GameMain;
	GtskTbl[GMODE_EDIT_INIT] = &GameTask::EditInit;
	GtskTbl[GMODE_EDIT_MAIN] = &GameTask::EditMain;
	GtskTbl[GMODE_DESTROY] = &GameTask::SysDestroy;

	SetOffset(VECTOR2(0, 0));
	


	lpMapCtrl->reSetUp();
}


GameTask::~GameTask()
{
	SysDestroy();
}

void GameTask::Create()
{
	ImageMgr::Create();
	MapCtrl::Create();
	if (s_Instance == nullptr)
	{
		s_Instance = new GameTask();
	}
}


void GameTask::Destroy()
{
	if (s_Instance != nullptr)
	{
		delete s_Instance;
		s_Instance = nullptr;
	}
}

void GameTask::Update(void)
{
	memcpy(keyDataOld, keyData, sizeof(keyDataOld));
	GetHitKeyStateAll(keyData);
	(this->*GtskTbl[mode])();
}

void GameTask::SetOffset(const VECTOR2& pos)
{
	drawOffset = pos;
}

const VECTOR2 & GameTask::GetOffset()
{
	return drawOffset;
}

GMODE GameTask::GetGameMode()
{
	return mode;
}

int GameTask::SysInit(void)
{
	// ���я���
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);		// 65536�FӰ�ނɐݒ�
	ChangeWindowMode(true);				// true:window�@false:�ٽ�ذ�
	SetWindowText("BomberMan_1701362_�΋����l");
	if (DxLib_Init() == -1)
	{
		return false;	// DXײ���؏���������

	}
	SetDrawScreen(DX_SCREEN_BACK);		// �ЂƂ܂��ޯ��ޯ̧�ɕ`��
	mode = GMODE_EDIT_INIT;

	return true;
}


int GameTask::GameInit(void)
{
	// �Q�[���I�u�W�F�N�g���X�g�̏���������
	DeleteObjList();

	
	// �Q�[���Z�b�g�A�b�v����
	lpMapCtrl->reSetUp();
	lpMapCtrl->MapLoad(GAME_MAP_FILENAME);
	lpMapCtrl->SetUpChar();
	lpCameraMgr->SetMoveFlg(true);
	lpEnemyMgr->SetMoveFlg(false);
	lpCharactorMgr->SetMoveFlg(false);

	mode = GMODE_GAME_MAIN;
	return 1;
}

// �ްт�Ҳݏ���
int GameTask::GameMain(void)
{

	if (keyData[KEY_INPUT_F1] ^ keyDataOld[KEY_INPUT_F1] & keyData[KEY_INPUT_F1])
	{
		GameDestroy();
		mode = GMODE_EDIT_INIT;
		return 1;
	}

	ClsDrawScreen(); // ��ʏ���

	
	// �}�b�v�`��
	lpMapCtrl->MapDraw();
	/*
	// �I�u�W�F�N�g�폜����
	for (auto _it = gameObjList.begin(); _it != gameObjList.end();)
	{
		if (!(*_it)->GetActive())
		{
			delete (*_it);
			_it = gameObjList.erase(_it);
			continue;
		}
		_it++;
	}

	// �I�u�W�F�N�g����
	for (auto _it : gameObjList)
	{
		(*_it).Update();
		(*_it).UpdateAnim();
		(*_it).Draw();
	}
	*/

	lpCameraMgr->Update();
	lpCharactorMgr->Update();
	lpEnemyMgr->Update();

	lpCameraMgr->Draw();
	lpCharactorMgr->Draw();
	lpEnemyMgr->Draw();

	ScreenFlip();		// �ް�ٰ�߂̍Ō�ɕK���K�v
	return 1;
}

int GameTask::SysDestroy(void)
{
	if (!editCursor)
	{
		delete editCursor;
	}
	// �Q�[���I�u�W�F�N�g���X�g�̏���������
	DeleteObjList();
	return 1;
}

void GameTask::GameDestroy(void)
{
}

void GameTask::EditDestroy(void)
{
}

void GameTask::DeleteObjList(void)
{
	for (auto _it = gameObjList.begin(); _it != gameObjList.end(); _it++)
	{
		delete (*_it);
	}
	gameObjList.clear();
}

bool GameTask::AddObj(Object * obj,bool isPushBack)
{
	if (obj != nullptr)
	{
		if (isPushBack)
		{
			gameObjList.push_back(obj);
		}
		else
		{
			gameObjList.push_front(obj);
		}
		return true;
	}
	return false;
}


int GameTask::EditInit(void)
{
	SetOffset(VECTOR2(20, 20));
	ImageMgr::GetInstance()->GetID("image/mapData.png", chipSize, VECTOR2(4, 6));
	editCursor = new EditCursor(GetOffset(),keyData,keyDataOld);
	editCursor->Init("image/mapData.png", chipSize, VECTOR2(4, 6), VECTOR2(0, 5 ),20);
	
	lpMapCtrl->reSetUp();
	
	mode = GMODE_EDIT_MAIN;
	return 1;
}


int GameTask::EditMain(void)
{

	if (keyData[KEY_INPUT_F1] ^ keyDataOld[KEY_INPUT_F1] & keyData[KEY_INPUT_F1])
	{
		EditDestroy();
		mode = GMODE_GAME_INIT;
		return 1;
	}

	if (keyData[KEY_INPUT_F5])
	{
		int mbID = MessageBox(GetMainWindowHandle(), "�G�f�B�b�g���e��ۑ����܂���?", "�m�F�_�C�A���O", MB_OKCANCEL);
		if (mbID == IDOK)
		{
			// �ۑ��ꏊ�ƃt�@�C������n��

			// �Ȃ�����O�f���̂łƂ肠����������
			/*
			char fileName_c[MAX_PATH] ="";
			OPENFILENAME openFile;
			openFile.lStructSize = sizeof(OPENFILENAME);
			openFile.hwndOwner = GetMainWindowHandle();
			openFile.lpstrFilter = "MapData(*.map)\0*.map\0\0";
			openFile.lpstrFile = fileName_c;
			openFile.nMaxFile = MAX_PATH;
			openFile.Flags = OFN_FILEMUSTEXIST;
			GetSaveFileName(&openFile);
			*/
			if (lpMapCtrl->MapSave("data/mapdata.map"))
			{
				MessageBox(GetMainWindowHandle(), "�f�[�^��ۑ����܂���", "�f�[�^�̕ۑ�", MB_OK);
			}
			else
			{
				MessageBox(GetMainWindowHandle(), "�f�[�^�̕ۑ��Ɏ��s���܂���", "�f�[�^�̕ۑ�", MB_OK);
			}
		}
	}
	if (keyData[KEY_INPUT_F6])
	{
		int mbID = MessageBox(GetMainWindowHandle(), "�}�b�v����ǂݍ��݂܂���?", "�m�F�_�C�A���O", MB_OKCANCEL);

		if (mbID == IDOK)
		{
			// �Ȃ�����O�f���̂łƂ肠����������
			/*
			char fileName_c[MAX_PATH]="";
			
			OPENFILENAME openFile;
			openFile.lStructSize = sizeof(OPENFILENAME);
			openFile.hwndOwner = GetMainWindowHandle();
			openFile.lpstrFilter = "MapData(*.map)\0*.map\0\0";
			openFile.lpstrFile = fileName_c;
			openFile.nMaxFile = MAX_PATH;
			openFile.Flags = OFN_FILEMUSTEXIST;
			GetOpenFileName(&openFile);
			*/
			if (lpMapCtrl->MapLoad("data/mapdata.map"))
			{
				MessageBox(GetMainWindowHandle(), "�f�[�^��ǂݍ��݂܂���", "�f�[�^�̓ǂݍ���", MB_OK);
			}
			else
			{
				MessageBox(GetMainWindowHandle(), "�f�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�f�[�^�̓ǂݍ���", MB_OK);
			}
		}
	}

	// �G�f�B�b�g�J�[�\������
	if (editCursor != nullptr)
	{
		editCursor->Update();
	}

	// �`�揈��
	ClsDrawScreen();

	// �}�b�v�f�[�^�̕`��
	lpMapCtrl->MapDraw();

	// �G�f�B�b�g�J�[�\���̕`��
	if (editCursor != nullptr)
	{
		editCursor->Draw();
	}


	// �o�ܐ��̕`��
	VECTOR2 tmp1, tmp2;
	for (tmp1 = GetOffset(); tmp1.x <= SCREEN_SIZE_X-GetOffset().x; tmp1.x += 20)
	{
		tmp2 = VECTOR2(tmp1.x, SCREEN_SIZE_Y - GetOffset().y+1);
		DrawLine(tmp1, tmp2, 0xffffff);
	}
	for (tmp1 = GetOffset(); tmp1.y <= SCREEN_SIZE_Y-GetOffset().y; tmp1.y += 20)
	{
		tmp2 = VECTOR2(SCREEN_SIZE_X - GetOffset().x, tmp1.y);
		DrawLine(tmp1, tmp2, 0xffffff);
	}

	ScreenFlip();
	return 1;
}



int DrawLine(VECTOR2 start, VECTOR2 end, int color, int thicness)
{
	return DrawLine(start.x, start.y, end.x, end.y, color, thicness);
}

