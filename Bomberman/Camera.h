#pragma once
#include "Object.h"
class Camera :
	public Object
{
public:
	Camera(const VECTOR2& offset, char(&keyData)[256], char(&keyDataOld)[256]);
	~Camera();
	void SetMove();
	void UpdateAnim() {}
};

