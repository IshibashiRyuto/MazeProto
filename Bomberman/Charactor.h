#pragma once
#include "Object.h"
class Charactor :
	public Object
{
public:
	Charactor(const VECTOR2& drawOffset, char(&keyData)[256], char(&keyDataOld)[256]);
	~Charactor();
	void SetMove();
	void UpdateAnim() {}
};

