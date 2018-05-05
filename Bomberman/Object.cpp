#include "Object.h"
#include "DxLib.h"
#include "ImageMgr.h"

Object::Object(const VECTOR2& drawOffset, char(&_keyData)[256], char(&_keyDataOld)[256]) : keyData(_keyData), keyDataOld(_keyDataOld)
{
	pos = VECTOR2(0, 0);
	active = false;
	this->drawOffset = drawOffset;
	imageName = "";
	angle[DIR_UP] = 0.0f;
	angle[DIR_LEFT] = PI * 3 / 2;
	angle[DIR_RIGHT] = PI / 2;
	angle[DIR_DOWN] = PI;
}


Object::~Object()
{
}

void Object::Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCnt, VECTOR2 chipOffset,int speed, int animSpeed, int stateAnimDiv)
{
	ImageMgr::GetInstance()->GetID(fileName, divSize, divCnt);
	this->divSize = divSize;
	this->divCnt = divCnt;
	this->chipOffset = chipOffset;
	this->speed = speed;
	this->imageName = fileName;
	this->drawDir = DIR_DOWN;
	this->animCnt = 0;
	this->animSpeed = animSpeed;
	this->stateAnimDiv = stateAnimDiv;
	this->state = ST_STAND;
	active = true;
}

const VECTOR2& Object::GetPos()
{
	return this->pos;
}

const VECTOR2& Object::GetDivSize()
{
	return this->divSize;
}

void Object::SetDivSize(const VECTOR2 & divSize)
{
	this->divSize = divSize;
}

const VECTOR2 & Object::GetChipOffset()
{
	return chipOffset;
}

void Object::SetChipOffset(const VECTOR2 & chipOffset)
{
	this->chipOffset = chipOffset;
}

bool Object::GetActive()
{
	return active;
}

void Object::SetActive(bool active)
{
	this->active = active;
}

void Object::Draw(void) const
{
	if (imageName.empty())
	{
		return;
	}
	DrawGraph(this->pos.x + drawOffset.x , this->pos.y + drawOffset.y, IMAGE_ID(imageName)[chipOffset.y * divCnt.x + chipOffset.x], true);
}

void Object::Draw(const VECTOR2 & pos) const
{
	if (imageName.empty())
	{
		return;
	}
	DrawGraph(pos.x, pos.y, IMAGE_ID(imageName)[chipOffset.y * divCnt.x + chipOffset.x], true);
}

void Object::SetMove(void)
{
}

void Object::Update(void)
{
	SetMove();
}

void Object::UpdateAnim(void)
{
	animCnt++;
	chipOffset = VECTOR2(drawDir, state * stateAnimDiv + (animCnt/animSpeed) % stateAnimDiv );

}

void Object::SetMoveFlg(bool flg)
{
	moveFlg = flg;
}

bool Object::GetMoveFlg() const
{
	return moveFlg;
}

void Object::SetDir(DRAW_DIR dir)
{
	drawDir = dir;
}

void Object::SetStopCnt(int stopCnt)
{
	this->stopCnt = stopCnt;
}

DRAW_DIR Object::GetRightHandDir(const DRAW_DIR dir)
{
	auto moveDir = dir;
	switch (moveDir)
	{
	case DIR_DOWN:
		moveDir = DIR_LEFT;
		break;
	case DIR_LEFT:
		moveDir = DIR_UP;
		break;
	case DIR_UP:
		moveDir = DIR_RIGHT;
		break;
	case DIR_RIGHT:
		moveDir = DIR_DOWN;
		break;
	}
	return moveDir;
}

void Object::SetPos(const VECTOR2 & pos)
{
	this->pos = pos;
}

