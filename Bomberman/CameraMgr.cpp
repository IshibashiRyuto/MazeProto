#include "CameraMgr.h"
#include "Camera.h"
#include"CharactorMgr.h"

CameraMgr* CameraMgr::s_Instance;

CameraMgr::CameraMgr()
{
}


CameraMgr::~CameraMgr()
{
}

void CameraMgr::AddCamera(Camera * camera)
{
	cameraList.push_back(std::shared_ptr<Camera>(camera));
}

void CameraMgr::SetMoveFlg(bool moveFlg)
{
	for (auto camera : cameraList)
	{
		camera->SetMoveFlg(moveFlg);
	}
	state = moveFlg;
}

void CameraMgr::Update()
{
	if (state)
	{
		for (auto camera = cameraList.begin(); camera != cameraList.end(); ++camera)
		{
			if (!(*camera))
			{
				cameraList.erase(camera);
			}
		}
		bool moveFlg = false;
		for (auto camera : cameraList)
		{
			camera->Update();
			if (!moveFlg && camera->GetMoveFlg())
			{
				moveFlg = true;
			}
		}
		if (!moveFlg)
		{
			//キャラクターの移動フラグをOnに
			lpCharactorMgr->SetMoveFlg(true);
			state = false;
		}
	}
}

void CameraMgr::Draw()
{
	for (auto camera : cameraList)
	{
		camera->UpdateAnim();
		camera->Draw();
	}
}

bool CameraMgr::GetState()
{
	return state;
}
