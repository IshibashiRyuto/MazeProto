#pragma once
#include "Object.h"
class Charactor :
	public Object
{
private:
	std::shared_ptr<Charactor> dammy;
	bool dammyOn{ false };
	int dammyMoveCnt{ 0 };

	int nextScreenCnt;
	int GetMoveCntNextScreen();
	VECTOR2 nextScreenPos{};

public:
	Charactor(const VECTOR2& drawOffset, char(&keyData)[256], char(&keyDataOld)[256], bool useDammy = true);
	~Charactor();
	void SetMove();
	void Draw() const;
	void UpdateAnim() {}
	void SetDammyFlg(bool flg);

	void Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCnt, VECTOR2 chipOffset, int speed, int animSpeed = 10, int stateAnimDiv = 2);

};

