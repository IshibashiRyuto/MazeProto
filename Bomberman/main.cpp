#include "DxLib.h"
#include "GameTask.h"


// ---------- WinMain関数
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	GameTask::Create();
	GameTask *gameTask = GameTask::GetInstance();
	gameTask->Update();

					// ---------- ｹﾞｰﾑﾙｰﾌﾟ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0 )
	{
		gameTask->Update();
	}

	GameTask::Destroy();
	
	DxLib_End();	// DXﾗｲﾌﾞﾗﾘの終了処理
	return 0;
}

