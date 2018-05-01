#pragma once
#include "Object.h"
#include "MapCtrl.h"

#define EDIT_KEY_GET_DEF_RNG (30)
#define EDIT_KEY_GET_DEF_RNG_MIN (5)

class EditCursor :
	public Object
{
public:
	~EditCursor();
	EditCursor(const VECTOR2& offset, char (&keyData)[256], char (&keyDataOld)[256]);

	void SetMove();
	void Draw(void) const;

private:
	MAP_ID selectMapID{ MAP_NON };
	unsigned int cnt;				// 生成からのカウント
	unsigned int keyGetRng;			// 次のキー処理までの間隔
	unsigned int inputFrame;		// 前回の移動からの間隔
};

