#include "DxLib.h"
#include "GameTask.h"


// ---------- WinMainŠÖ”
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	GameTask::Create();
	GameTask *gameTask = GameTask::GetInstance();
	gameTask->Update();

					// ---------- ¹Ş°ÑÙ°Ìß
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0 )
	{
		gameTask->Update();
	}

	GameTask::Destroy();
	
	DxLib_End();	// DX×²ÌŞ×Ø‚ÌI—¹ˆ—
	return 0;
}

