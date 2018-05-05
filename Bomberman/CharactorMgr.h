#pragma once
#include<vector>
#include<memory>
#include"VECTOR2.h"
#include"Object.h"

#define lpCharactorMgr CharactorMgr::GetInstance()
class Charactor;

class CharactorMgr
{
public:
	static void Create()
	{
		if (!s_Instance)
		{
			s_Instance = new CharactorMgr();
		}
	}
	static CharactorMgr* GetInstance()
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
	~CharactorMgr();

	void AddCharactor(Charactor* charactor);
	void SetMoveFlg(bool moveFlg);
	void SetOnDammyFlg(bool flg);
	void Update();
	void Draw();
	bool GetState() const;
	void PushedWall(DRAW_DIR pushDir);
	bool IsCharactor(const VECTOR2& pos) const;
private:
	CharactorMgr();
	CharactorMgr(const CharactorMgr&) {}
	CharactorMgr& operator=(const CharactorMgr&) {}
	static CharactorMgr* s_Instance;
	std::vector<std::shared_ptr<Charactor>> charactorList;
	bool state{ false };
};

