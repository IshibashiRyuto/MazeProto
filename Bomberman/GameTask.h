#pragma once
#include <vector>
#include <list>
#include "VECTOR2.h"
#include <memory>

class Object;

#define SCREEN_SIZE_X		800
#define SCREEN_SIZE_Y		600

#define GAME_SCREEN_SIZE_X	640
#define GAME_SCREEN_SIZE_Y	480


#define LIST_OBJ std::list<Object*>

#define lpGameTask GameTask::GetInstance()

#define GAME_MAP_FILENAME "data/mapdata.map"
class Camera;

enum GMODE
{
	GMODE_SYS_INIT,
	GMODE_GAME_INIT,
	GMODE_GAME_MAIN,
	GMODE_EDIT_INIT,
	GMODE_EDIT_MAIN,
	GMODE_DESTROY,
	GMODE_MAX
};


class GameTask
{
public:
	~GameTask();

	static void Create();
	static GameTask* GetInstance()
	{
		if (s_Instance == nullptr)
		{
			Create();
		}
		return s_Instance;
	}
	static void Destroy();


	bool AddObj(Object* obj, bool isPushBack = true);
	void Update(void);
	void SetOffset(const VECTOR2& pos);
	const VECTOR2& GetOffset();
	const VECTOR2 chipSize{ 20,20 };
	VECTOR2 drawOffset;					// 描画オフセット量
	char keyData[256];					// キーデータの実体
	char keyDataOld[256];				// 1frame前のキーデータ

	GMODE GetGameMode();
private:
	// メンバ
	GMODE mode;							// ゲームモード
	Object *editCursor = nullptr;		// editモードのカーソル
	static GameTask* s_Instance;		// シングルトンインスタンス
	int (GameTask::*GtskTbl[GMODE_MAX])(void);
	LIST_OBJ gameObjList;

	// メソッド
	GameTask();
	int SysInit(void);
	int GameInit(void);
	int GameMain(void);
	int EditInit(void);
	int EditMain(void);
	int SysDestroy(void);

	void GameDestroy(void);
	void EditDestroy(void);
	
	void DeleteObjList(void);

};


int DrawLine(VECTOR2 start, VECTOR2 end, int color, int thicness = 1);