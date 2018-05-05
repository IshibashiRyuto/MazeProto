#pragma once
#include <string>
#include "VECTOR2.h"
#include "GameTask.h"



#define NUM_READ_MOVE 3
#define MOVE_WAIT_CNT 20


enum OBJECT_TYPE
{
	OBJECT_TYPE_PLAYER,
	OBJECT_TYPE_ENEMY,
	OBJECT_TYPE_MAX
};

enum STATE
{
	ST_STAND,
	ST_WALK,
	ST_MAX
};

enum DRAW_DIR
{
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT,
	DIR_UP,
	DIR_DEATH,
	DIR_MAX
};

class Object
{
public:
	Object(const VECTOR2& drawOffset, char (&keyData)[256],char (&keyDataOld)[256]);
	virtual ~Object();
	virtual void Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCnt, VECTOR2 chipOffset, int speed, int animSpeed = 10, int stateAnimDiv = 2);


	const VECTOR2 & GetPos();
	void SetPos(const VECTOR2 & pos);

	const VECTOR2 & GetDivSize();
	void SetDivSize(const VECTOR2 & divSize);

	const VECTOR2 & GetChipOffset();
	void SetChipOffset(const VECTOR2 & chipOffset);

	bool GetActive();
	void SetActive(bool active);
	
	virtual void Draw(void) const;
	virtual void Draw(const VECTOR2 & pos) const;

	virtual void SetMove(void);
	void Update(void);
	virtual void UpdateAnim(void);
	virtual void SetMoveFlg(bool flg);
	virtual bool GetMoveFlg() const;

	void SetDir(DRAW_DIR dir);

	void SetStopCnt(int stopCnt);

protected:
	VECTOR2 pos;			// �L�����̈ʒu(�`��ʒu
	int speed;
	bool active;			// �����t���O
	char (&keyData)[256];	// �L�[�f�[�^�̎Q��
	char(&keyDataOld)[256];	// 1frame�O�̃L�[�f�[�^�̎Q��
	std::string imageName;	// �`��`�b�v�̃C���[�W
	VECTOR2 divSize;		// �`��`�b�v�̃T�C�Y
	VECTOR2 divCnt;			// �`��`�b�v���i�c��
	VECTOR2 chipOffset;		// �`��`�b�v�̃C���[�WID�̓񎟌��z���ł̎g�p�ӏ��̊J�n�ʒu
	VECTOR2 drawOffset;		// �`��ʒu�I�t�Z�b�g
	DRAW_DIR drawDir;		// �I�u�W�F�N�g�̌���

	int animCnt;
	int animSpeed;
	int stateAnimDiv;
	STATE state;

	int moveKeyID;		//�ړ������̃L�[�f�[�^
	const float PI = 3.14159265f;

	bool moveFlg{ false };		//�ړ��\�����肷��t���O
	float angle[4];
	int stopCnt{ 0 };

	///�w�肵�������̉E���90�x������Ԃ�
	DRAW_DIR GetRightHandDir(const DRAW_DIR dir);
	
};

