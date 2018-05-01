#pragma once
#include "Object.h"
class Enemy :
	public Object
{
public:
	Enemy(const VECTOR2& offset, char(&keyData)[256], char(&keyDataOld)[256]);
	~Enemy();
	void SetMove();
	void UpdateAnim();
};

