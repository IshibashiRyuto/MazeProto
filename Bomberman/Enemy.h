#pragma once
#include "Object.h"
class Enemy :
	public Object
{
private:
	std::shared_ptr<Enemy> dammy;
	bool dammyOn{ false };
	int dammyMoveCnt{ 0 };
public:
	Enemy(const VECTOR2& offset, char(&keyData)[256], char(&keyDataOld)[256],bool useDammy = true);
	~Enemy();
	void SetMove();
	void UpdateAnim();
	void Draw()const;

	void SetDammyFlg(bool flg);

	void Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCnt, VECTOR2 chipOffset, int speed, int animSpeed = 10, int stateAnimDiv = 2);
};

