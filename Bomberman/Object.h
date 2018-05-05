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
	VECTOR2 pos;			// キャラの位置(描画位置
	int speed;
	bool active;			// 生存フラグ
	char (&keyData)[256];	// キーデータの参照
	char(&keyDataOld)[256];	// 1frame前のキーデータの参照
	std::string imageName;	// 描画チップのイメージ
	VECTOR2 divSize;		// 描画チップのサイズ
	VECTOR2 divCnt;			// 描画チップ数（縦横
	VECTOR2 chipOffset;		// 描画チップのイメージIDの二次元配列上での使用箇所の開始位置
	VECTOR2 drawOffset;		// 描画位置オフセット
	DRAW_DIR drawDir;		// オブジェクトの向き

	int animCnt;
	int animSpeed;
	int stateAnimDiv;
	STATE state;

	int moveKeyID;		//移動方向のキーデータ
	const float PI = 3.14159265f;

	bool moveFlg{ false };		//移動可能か判定するフラグ
	float angle[4];
	int stopCnt{ 0 };

	///指定した方向の右回り90度方向を返す
	DRAW_DIR GetRightHandDir(const DRAW_DIR dir);
	
};

