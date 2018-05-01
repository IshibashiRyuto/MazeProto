#include "DxLib.h"
#include "GameTask.h"


// ---------- WinMain�֐�
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	GameTask::Create();
	GameTask *gameTask = GameTask::GetInstance();
	gameTask->Update();

					// ---------- �ް�ٰ��
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0 )
	{
		gameTask->Update();
	}

	GameTask::Destroy();
	
	DxLib_End();	// DXײ���؂̏I������
	return 0;
}

