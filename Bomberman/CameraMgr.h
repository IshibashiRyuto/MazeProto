#pragma once
#include <vector>
#include <memory>

#define lpCameraMgr CameraMgr::GetInstance()

class Camera;

class CameraMgr
{
public:
	static void Create()
	{
		if (!s_Instance)
		{
			s_Instance = new CameraMgr();
		}
	}

	static CameraMgr* GetInstance()
	{
		if (s_Instance == nullptr)
		{
			Create();
		}
		return s_Instance;
	}

	static void Destroy()
	{
		if (s_Instance)
		{
			delete s_Instance;
			s_Instance = nullptr;
		}
	}

	~CameraMgr();

	void AddCamera(Camera* camera);
	void SetMoveFlg(bool moveFlg);
	void Update();
	void Draw();
	bool GetState();

private:
	CameraMgr();
	CameraMgr(const CameraMgr&) {}
	CameraMgr& operator =(const CameraMgr&) {}

	static CameraMgr *s_Instance;
	std::vector<std::shared_ptr<Camera>> cameraList;
	bool state{ false };
};

