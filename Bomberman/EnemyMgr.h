#pragma once
#include <vector>
#include <memory>
#include "VECTOR2.h"

#define lpEnemyMgr EnemyMgr::GetInstance()
class Enemy;

class EnemyMgr
{
public:
	static void Create()
	{
		if (!s_Instance)
		{
			s_Instance = new EnemyMgr();
		}
	}
	static EnemyMgr* GetInstance()
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
	~EnemyMgr();

	void AddEnemy(Enemy* enemy);
	void SetMoveFlg(bool moveFlg);
	void Update();
	void Draw();
	bool GetState()const;

	bool IsEnemy(const VECTOR2& pos) const;

private:
	EnemyMgr();
	EnemyMgr(const EnemyMgr&) {}
	EnemyMgr& operator =(const EnemyMgr&) {}

	static EnemyMgr *s_Instance;
	std::vector<std::shared_ptr<Enemy>> enemyList;
	bool state{ false };
};

