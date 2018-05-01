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
	unsigned int cnt;				// ��������̃J�E���g
	unsigned int keyGetRng;			// ���̃L�[�����܂ł̊Ԋu
	unsigned int inputFrame;		// �O��̈ړ�����̊Ԋu
};

