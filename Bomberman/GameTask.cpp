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
	// ｼｽﾃﾑ処理
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);		// 65536色ﾓｰﾄﾞに設定
	ChangeWindowMode(true);				// true:window　false:ﾌﾙｽｸﾘｰﾝ
	SetWindowText("BomberMan_1701362_石橋龍人");
	if (DxLib_Init() == -1)
	{
		return false;	// DXﾗｲﾌﾞﾗﾘ初期化処理

	}
	SetDrawScreen(DX_SCREEN_BACK);		// ひとまずﾊﾞｯｸﾊﾞｯﾌｧに描画
	mode = GMODE_EDIT_INIT;

	return true;
}


int GameTask::GameInit(void)
{
	// ゲームオブジェクトリストの初期化処理
	DeleteObjList();

	
	// ゲームセットアップ処理
	lpMapCtrl->reSetUp();
	lpMapCtrl->MapLoad(GAME_MAP_FILENAME);
	lpMapCtrl->SetUpChar();
	lpCameraMgr->SetMoveFlg(true);
	lpEnemyMgr->SetMoveFlg(false);
	lpCharactorMgr->SetMoveFlg(false);

	mode = GMODE_GAME_MAIN;
	return 1;
}

// ｹﾞｰﾑのﾒｲﾝ処理
int GameTask::GameMain(void)
{

	if (keyData[KEY_INPUT_F1] ^ keyDataOld[KEY_INPUT_F1] & keyData[KEY_INPUT_F1])
	{
		GameDestroy();
		mode = GMODE_EDIT_INIT;
		return 1;
	}

	ClsDrawScreen(); // 画面消去

	
	// マップ描画
	lpMapCtrl->MapDraw();
	/*
	// オブジェクト削除処理
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

	// オブジェクト処理
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

	ScreenFlip();		// ｹﾞｰﾑﾙｰﾌﾟの最後に必ず必要
	return 1;
}

int GameTask::SysDestroy(void)
{
	if (!editCursor)
	{
		delete editCursor;
	}
	// ゲームオブジェクトリストの初期化処理
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
		int mbID = MessageBox(GetMainWindowHandle(), "エディット内容を保存しますか?", "確認ダイアログ", MB_OKCANCEL);
		if (mbID == IDOK)
		{
			// 保存場所とファイル名を渡す

			// なぜか例外吐くのでとりあえず未実装
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
				MessageBox(GetMainWindowHandle(), "データを保存しました", "データの保存", MB_OK);
			}
			else
			{
				MessageBox(GetMainWindowHandle(), "データの保存に失敗しました", "データの保存", MB_OK);
			}
		}
	}
	if (keyData[KEY_INPUT_F6])
	{
		int mbID = MessageBox(GetMainWindowHandle(), "マップ情報を読み込みますか?", "確認ダイアログ", MB_OKCANCEL);

		if (mbID == IDOK)
		{
			// なぜか例外吐くのでとりあえず未実装
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
				MessageBox(GetMainWindowHandle(), "データを読み込みました", "データの読み込み", MB_OK);
			}
			else
			{
				MessageBox(GetMainWindowHandle(), "データの読み込みに失敗しました", "データの読み込み", MB_OK);
			}
		}
	}

	// エディットカーソル処理
	if (editCursor != nullptr)
	{
		editCursor->Update();
	}

	// 描画処理
	ClsDrawScreen();

	// マップデータの描画
	lpMapCtrl->MapDraw();

	// エディットカーソルの描画
	if (editCursor != nullptr)
	{
		editCursor->Draw();
	}


	// 経緯線の描画
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

