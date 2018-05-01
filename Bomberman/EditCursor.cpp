#include "EditCursor.h"
#include "Dxlib.h"
#include "ImageMgr.h"
#include<math.h>


EditCursor::~EditCursor()
{
}

EditCursor::EditCursor(const VECTOR2 & offset, char(&keyData)[256], char (&keyDataOld)[256]) : Object(offset,keyData, keyDataOld)
{
	inputFrame = EDIT_KEY_GET_DEF_RNG;
	keyGetRng = EDIT_KEY_GET_DEF_RNG;
	cnt = 0;
}

void EditCursor::SetMove()
{
	VECTOR2 tmpPos = pos;


	if (keyData[KEY_INPUT_NUMPAD8])
	{
		if (pos.y > 0)
		{
			tmpPos.y -= GetDivSize().y;
		}
	}
	if (keyData[KEY_INPUT_NUMPAD2])
	{
		if (pos.y < SCREEN_SIZE_Y - 3 * GetDivSize().y)
		{
			tmpPos.y += GetDivSize().y;
		}
	}
	if (keyData[KEY_INPUT_NUMPAD4])
	{
		if (pos.x > 0)
		{
			tmpPos.x -= GetDivSize().x;
		}
	}
	if (keyData[KEY_INPUT_NUMPAD6])
	{
		if (pos.x < SCREEN_SIZE_X - 3 * GetDivSize().x)
		{
			tmpPos.x += GetDivSize().x;
		}
	}

	if (keyData[KEY_INPUT_Z])
	{
		lpMapCtrl->SetMapData(selectMapID, pos);
	}

	// 塗りつぶし機能
	// Fキーを押すと、対象とするマスのマップオブジェクトと同じオブジェクトを持つ隣接するマスを選択した
	// マップオブジェクトで上書きする
	if (keyData[KEY_INPUT_F] ^ keyDataOld[KEY_INPUT_F] & keyData[KEY_INPUT_F])
	{

		lpMapCtrl->FillMapData(selectMapID, pos);
	}

	if (tmpPos != this->pos)
	{
		inputFrame++;
		cnt = 0;
		if (inputFrame >= keyGetRng)
		{
			pos = tmpPos;
			inputFrame = 0;
			keyGetRng = (keyGetRng <= EDIT_KEY_GET_DEF_RNG_MIN ? EDIT_KEY_GET_DEF_RNG_MIN : keyGetRng / 2);
		}
	}
	else
	{
		cnt++;
		inputFrame = EDIT_KEY_GET_DEF_RNG;
		keyGetRng = EDIT_KEY_GET_DEF_RNG;
		if (keyData[KEY_INPUT_S] ^ keyDataOld[KEY_INPUT_S] & keyData[KEY_INPUT_S])
		{
			if (selectMapID != END_EDIT_CHIP)
			{
				selectMapID = (MAP_ID)(selectMapID+1);
			}
			else
			{
				selectMapID = START_EDIT_CHIP;
			}
			cnt = 0;
		}
		else if (keyData[KEY_INPUT_A] ^ keyDataOld[KEY_INPUT_A] & keyData[KEY_INPUT_A])
		{

			if (selectMapID != START_EDIT_CHIP)
			{
				selectMapID = (MAP_ID)(selectMapID - 1);
			}
			else
			{
				selectMapID = END_EDIT_CHIP;
			}
			cnt = 0;
		}
	}
}

void EditCursor::Draw(void) const
{
	// イメージファイルを持っている場合
	if (imageName.empty())
	{
		return;
	}

	auto image = IMAGE_ID(imageName);

	// エディットカーソルの描画
	DrawGraph(this->pos.x + drawOffset.x, this->pos.y + drawOffset.y, image[chipOffset.y * divCnt.x + chipOffset.x], true);

	// エディットカーソルの存在する行・列にエディットカーソルを薄く描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
	
	for(int y = 0; y < lpMapCtrl->mapSize.y; y++)
	{
		DrawGraph(this->pos.x + drawOffset.x,
			divSize.y * y + drawOffset.y,
			image[chipOffset.y * divCnt.x + chipOffset.x], true);
	}
	for (int x = 0; x < lpMapCtrl->mapSize.x; x++)
	{
		DrawGraph(divSize.x * x + drawOffset.x,
			this->pos.y + drawOffset.y,
			image[chipOffset.y * divCnt.x + chipOffset.x], true);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	// 特定のキーが押されていた場合、選択中のマップオブジェクトのひとつ前、一つ次の要素を描画
	if (keyData[KEY_INPUT_A] | keyData[KEY_INPUT_S])
	{
		int prevImage = selectMapID;
		int nextImage = selectMapID;

		if (selectMapID == START_EDIT_CHIP)
		{
			prevImage = image[END_EDIT_CHIP];
			nextImage = image[selectMapID + 1];
		}
		else if (selectMapID == END_EDIT_CHIP)
		{
			prevImage = image[selectMapID - 1];
			nextImage = image[START_EDIT_CHIP];
		}
		else
		{
			prevImage = image[selectMapID - 1];
			nextImage = image[selectMapID + 1];
		}

		DrawGraph(this->pos.x + drawOffset.x - divSize.x,
			this->pos.y + drawOffset.y,
			prevImage, true);
		DrawGraph(this->pos.x + drawOffset.x + divSize.x,
			this->pos.y + drawOffset.y,
			nextImage, true);

	}

	// 選択中のマップオブジェクトを描画

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, abs( (int)(256-((cnt*10)%512)) ) );
	DrawGraph(this->pos.x + drawOffset.x, this->pos.y + drawOffset.y, image[selectMapID], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
