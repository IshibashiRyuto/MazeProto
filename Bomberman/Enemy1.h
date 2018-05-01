#pragma once
#include "Object.h"

#define MOVE_WAIT_TIME 60

class Enemy1 :
	public Object
{
public:
	Enemy1(const VECTOR2& offset, char(&keyData)[256], char(&keyDataOld)[256]);
	~Enemy1();

	void SetMove();
private:
	int moveWaitTime;
};

