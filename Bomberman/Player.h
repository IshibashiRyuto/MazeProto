#pragma once
#include "Object.h"
class Player :
	public Object
{
public:
	Player(const VECTOR2& offset, char(&keyData)[256], char(&keyDataOld)[256]);
	~Player();

	void SetMove();

private:
	unsigned int fireRange{ 1 };

};

