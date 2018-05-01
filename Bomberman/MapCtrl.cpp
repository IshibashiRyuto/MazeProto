#include "MapCtrl.h"
#include "GameTask.h"
#include "ImageMgr.h"
#include "Object.h"
#include "Player.h"
#include "Enemy1.h"
#include<DxLib.h>
#include<stdio.h>
#include"Camera.h"
#include "Charactor.h"
#include "Enemy.h"
#include "CameraMgr.h"
#include "EnemyMgr.h"
#include "CharactorMgr.h"



MapCtrl* MapCtrl::s_inst = nullptr;

void MapCtrl::Destroy()
{
	if (s_inst)
	{
		delete s_inst;
	}
}

bool MapCtrl::reSetUp(MAP_ID resetWall)
{
	camera = nullptr;
	for (unsigned int y = 0; y < mapData.size(); y++)
	{
		mapData[y].clear();
	}

	mapData.clear();

	// mapDataのリサイズ&初期化
	mapData.resize(mapSize.y);
	
	for (unsigned int y = 0; y < mapData.size(); y++)
	{
		mapData[y].resize(mapSize.x);
	}

	for (unsigned int y = 0; y < mapData.size(); y++)
	{
		for (unsigned int x = 0; x < mapData[y].size(); x++)
		{
			mapData[y][x] = MAP_NON;
		}
	}


	objMapData.resize(mapSize.y);
	for (unsigned int y = 0; y < objMapData.size(); y++)
	{
		objMapData[y].resize(mapSize.x);
	}

	for (unsigned int y = 0; y < objMapData.size(); y++)
	{
		for (unsigned int x = 0; x < objMapData[y].size(); x++)
		{
			objMapData[y][x] = MAP_NON;
		}
	}


	return true;
}

MapCtrl::MapCtrl() : camera(nullptr)
{
}


MapCtrl::~MapCtrl()
{
}

void MapCtrl::Create()
{
	if (s_inst == nullptr)
	{
		s_inst = new MapCtrl();
	}
}


bool MapCtrl::MapSave(std::string fileName)
{
	DataHeader expData =
	{
		BBM_ID_NAME,
		BBM_VER_ID,
		{ 0,0 },
		mapSize.x,
		mapSize.y,
		{ 0,0,0 },
		0x00
	};


	for (unsigned int y = 0; y < mapData.size(); y++)
	{
		for (unsigned int x = 0; x < mapData[y].size(); x++)
		{
			expData.sum += mapData[y][x];
		}
	}

	for (unsigned int y = 0; y < objMapData.size(); ++y)
	{
		for (unsigned int x = 0; x < objMapData[y].size(); ++x)
		{
			expData.sum += objMapData[y][x];
		}
	}

	FILE *fp;
	if (fopen_s(&fp, fileName.c_str(), "wb") != 0)
	{
		MessageBox(GetMainWindowHandle(), "ファイルを開けません", "保存エラー", MB_OK);
		return false;
	}

	fwrite(&expData, sizeof(DataHeader), 1, fp);

	for (unsigned int y = 0; y < mapData.size(); y++)
	{
		fwrite(&mapData[y][0], sizeof(MAP_ID), mapData[y].size(), fp);
	}
	
	for (unsigned int y = 0; y < objMapData.size(); y++)
	{
		fwrite(&objMapData[y][0], sizeof(MAP_ID), objMapData[y].size(), fp);
	}

	fclose(fp);

	return true;
}

bool MapCtrl::MapLoad(std::string fileName)
{
	DataHeader expData;
	FILE *fp;
	if (fopen_s(&fp, fileName.c_str(), "rb") != 0)
	{
		MessageBox(GetMainWindowHandle(), "ファイルを開けません", "読込エラー", MB_OK);
		return false;
	}


	fread(&expData, sizeof(DataHeader), 1, fp);

	if (strcmp(expData.fileID, BBM_ID_NAME) != 0)
	{
		MessageBox(GetMainWindowHandle(), "異なる形式のファイルが開かれました", "ファイルエラー", MB_OK);
		fclose(fp);
		return false;
	}
	if (expData.verID != BBM_VER_ID)
	{
		MessageBox(GetMainWindowHandle(), "ファイルのバージョンが今のバージョンと異なります\nファイルを開けません", "バージョンエラー", MB_OK);
		fclose(fp);
		return false;
	}

	mapData.resize(expData.sizeY);
	for (unsigned int i = 0; i < mapData.size(); i++)
	{
		mapData[i].resize(expData.sizeX);
	}

	for (unsigned int y = 0; y < mapData.size(); y++)
	{
		fread(&mapData[y][0], sizeof(MAP_ID), mapData[y].size(), fp);
	}

	objMapData.resize(expData.sizeY);
	for (unsigned int i = 0; i < objMapData.size(); i++)
	{
		objMapData[i].resize(expData.sizeX);
	}

	for (unsigned int y = 0; y < objMapData.size(); y++)
	{
		fread(&objMapData[y][0], sizeof(MAP_ID), objMapData[y].size(), fp);
	}
	fclose(fp);


	unsigned char checkSum = 0;

	for (unsigned int y = 0; y < mapData.size(); y++)
	{
		for (unsigned int x = 0; x < mapData[y].size(); x++)
		{
			checkSum += mapData[y][x];
		}
	}


	for (unsigned int y = 0; y < objMapData.size(); ++y)
	{
		for (unsigned int x = 0; x < objMapData[y].size(); ++x)
		{
			checkSum += objMapData[y][x];
		}
	}

	if (checkSum != expData.sum)
	{
		MessageBox(GetMainWindowHandle(), "ファイルに改ざんを発見しました", "データエラー", MB_OK);

		reSetUp();
		return false;
	}


	return true;
}

void MapCtrl::MapDraw()
{
	auto drawOffset = lpGameTask->drawOffset;
	auto chipSize = lpGameTask->chipSize;
	auto image = IMAGE_ID("image/mapData.png");
	auto gameMode = lpGameTask->GetGameMode();
	for (unsigned int y = 0; y < mapData.size(); y++)
	{
		for (unsigned int x = 0; (unsigned)x < mapData[y].size(); x++)
		{
			auto mapChipData = mapData[y][x];

			// マップチップの描画
			switch (gameMode)
			{
			case GMODE_EDIT_MAIN:
				if (mapChipData >= START_EDIT_CHIP && mapChipData <= END_EDIT_CHIP)
				{
					DrawGraph(drawOffset.x + chipSize.x * x, drawOffset.y + chipSize.y * y, image[mapChipData], true);
				}
				break;
			case GMODE_GAME_MAIN:
				if (mapData[y][x] >= START_GAME_CHIP && mapData[y][x] <= END_GAME_CHIP)
				{
					DrawGraph(drawOffset.x + chipSize.x * x, drawOffset.y + chipSize.y * y, image[mapChipData], true);
				}
				else if (mapData[y][x] == MAP_SCREEN)
				{
					DrawGraph(drawOffset.x + chipSize.x * x, drawOffset.y + chipSize.y * y, image[GetCameraMapData()], true);
					auto objData = GetCameraObjMapData();					
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);

					DrawBox(drawOffset.x + chipSize.x * x, drawOffset.y + chipSize.y * y, drawOffset.x + chipSize.x * (x+1), drawOffset.y + chipSize.y * (y+1), 0xff0000, true);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				}
				break;
			default:
				DrawGraph(drawOffset.x + chipSize.x * x, drawOffset.y + chipSize.y * y, image[mapChipData], true);

				break;
			}
		}
	}

	for (unsigned int y = 0; y < objMapData.size(); ++y)
	{
		for (unsigned int x = 0; x < objMapData[y].size(); ++x)
		{
			auto mapChipData = objMapData[y][x];
			switch (gameMode)
			{

			case GMODE_EDIT_MAIN:
				if (mapChipData > MAP_SCREEN && mapChipData <= MAP_ENEMY)
				{
					DrawGraph(drawOffset.x + chipSize.x * x, drawOffset.y + chipSize.y * y, image[mapChipData], true);
				}
				break;
			}
		}
	}
}

void MapCtrl::SetUpChar()
{
	for (unsigned int y = 0; y < objMapData.size(); y++)
	{
		for (unsigned int x = 0; x < objMapData[y].size(); x++)
		{
			//Object* obj = nullptr;
			switch (objMapData[y][x])
			{
			case MAP_CAMERA:
				Camera* obj;
				obj = new Camera(lpGameTask->GetOffset(), lpGameTask->keyData, lpGameTask->keyDataOld);
				obj->Init("image/mapData.png", VECTOR2(20,20), VECTOR2(4, 6), VECTOR2(1, 4), 2);
				obj->SetPos(VECTOR2(x*lpGameTask->chipSize.x, y*lpGameTask->chipSize.y));
				//lpGameTask->AddObj(obj);
				lpCameraMgr->AddCamera(obj);
				break;
			case MAP_PLAYER:
				Charactor* chara;
				chara = new Charactor(lpGameTask->GetOffset(), lpGameTask->keyData, lpGameTask->keyDataOld);
				chara->Init("Image/mapData.png", VECTOR2(20, 20), VECTOR2(4, 6), VECTOR2(2, 4), 2);
				chara->SetPos(VECTOR2(x*lpGameTask->chipSize.x, y*lpGameTask->chipSize.y));
				lpCharactorMgr->AddCharactor(chara);
				break;
			case MAP_ENEMY:
				Enemy* enemy;
				enemy = new Enemy(lpGameTask->GetOffset(), lpGameTask->keyData, lpGameTask->keyDataOld);
				enemy->Init("Image/mapData.png", VECTOR2(20, 20), VECTOR2(4, 6), VECTOR2(3, 4), 2);
				enemy->SetPos(VECTOR2(x*lpGameTask->chipSize.x, y*lpGameTask->chipSize.y));
				lpEnemyMgr->AddEnemy(enemy);
				break;
			}
		}
	}
}

bool MapCtrl::SetMapData(MAP_ID id, const VECTOR2 & vec)
{
	if (id == MAP_PLAYER || id == MAP_CAMERA)
	{
		for (unsigned int y = 0; y < objMapData.size(); y++)
		{
			for (unsigned int x = 0; x < objMapData[y].size(); x++)
			{
				if (objMapData[y][x] == id)
				{
					objMapData[y][x] = MAP_NON;
				}
			}
		}
	}
	VECTOR2 pos(vec.x / lpGameTask->chipSize.x, vec.y / lpGameTask->chipSize.y);
	if (id <= MAP_SCREEN)
	{
		if (pos.y >= 0 && (unsigned)pos.y <= mapData.size() - 1
			&& pos.x >= 0 && (unsigned)pos.x <= mapData[pos.y].size() - 1)
		{
			mapData[pos.y][pos.x] = id;
			if (id == MAP_NON)
			{
				objMapData[pos.y][pos.x] = id;
			}
		}
	}
	else
	{
		if (pos.y >= 0 && (unsigned)pos.y <= mapData.size() - 1
			&& pos.x >= 0 && (unsigned)pos.x <= mapData[pos.y].size() - 1)
		{
			objMapData[pos.y][pos.x] = id;
		}
	}
	return true;
}

bool MapCtrl::SetMapData(MAP_ID id, const VECTOR2 & vec, DRAW_DIR dir)
{
	VECTOR2 pos = vec;

	switch (dir)
	{
	case DIR_DOWN:
		pos.y += lpGameTask->chipSize.y + 1;
		break;
	case DIR_UP:
		pos.y -= 1;
		break;
	case DIR_RIGHT:
		pos.x += lpGameTask->chipSize.x + 1;
		break;
	case DIR_LEFT:
		pos.x -= 1;
		break;
	case DIR_DEATH:
		break;
	default:
		break;
	}

	pos.x = pos.x / lpGameTask->chipSize.x;
	pos.y = pos.y / lpGameTask->chipSize.y;

	if (pos.y > 0 && (unsigned)pos.y < mapData.size() - 1
		&& pos.x > 0 && (unsigned)pos.x < mapData[pos.y].size() - 1)
	{
		mapData[pos.y][pos.x] = id;
		return true;
	}
	return false;

}

bool MapCtrl::FillMapData(MAP_ID id, const VECTOR2 & vec)
{

	VECTOR2 pos(vec.x / lpGameTask->chipSize.x, vec.y / lpGameTask->chipSize.y);
	if (pos.y < 0 || pos.y >= lpMapCtrl->mapSize.y - 1 || pos.x < 0 || pos.x >= lpMapCtrl->mapSize.x - 1)
	{
		return false;
	}

	if (GetMapData(pos) == id)
	{
		return true;
	}

	_FillMapData(id, pos.x, pos.y, GetMapData(vec));

	return true;
}

bool MapCtrl::_FillMapData(MAP_ID id, int x, int y, MAP_ID fillTarget)
{
	if (x <= 0 || x >= lpMapCtrl->mapSize.x - 1 || y <= 0 || y >= lpMapCtrl->mapSize.y - 1)
	{
		return true;
	}

	if (mapData[y][x] != fillTarget)
	{
		return true;
	}

	mapData[y][x] = id;

	_FillMapData(id, x + 1, y, fillTarget);
	_FillMapData(id, x - 1, y, fillTarget);
	_FillMapData(id, x, y + 1, fillTarget);
	_FillMapData(id, x, y - 1, fillTarget);

	return true;
}

MAP_ID MapCtrl::GetCameraMapData() const
{
	auto pos = VECTOR2();
	if (camera)
	{
		pos = camera->GetPos();
	}
	return GetMapData(pos);
}

MAP_ID MapCtrl::GetCameraObjMapData() const
{
	auto pos = VECTOR2();
	if (camera)
	{
		pos = camera->GetPos();
	}
	return GetObjMapData(pos);
}

MAP_ID MapCtrl::GetMapData(const VECTOR2 & vec) const
{
	VECTOR2 pos(vec.x / lpGameTask->chipSize.x, vec.y / lpGameTask->chipSize.y);

	if (pos.y >= 0 && (unsigned)pos.y < mapData.size()
		&& pos.x >= 0 && (unsigned)pos.x < mapData[pos.y].size())
	{
		return mapData[pos.y][pos.x];
	}
	
	return MAP_NON;
}


MAP_ID MapCtrl::GetMapData(const VECTOR2 & vec, DRAW_DIR dir) const
{
	VECTOR2 pos = vec;

	switch (dir)
	{
	case DIR_DOWN:
		pos.y += lpGameTask->chipSize.y + 1;
		break;
	case DIR_UP:
		pos.y -= 1;
		break;
	case DIR_RIGHT:
		pos.x += lpGameTask->chipSize.x + 1;
		break;
	case DIR_LEFT:
		pos.x -= 1;
		break;
	default:
		break;
	}

	pos.x = pos.x / lpGameTask->chipSize.x;
	pos.y = pos.y / lpGameTask->chipSize.y;

	if (pos.y >= 0 && (unsigned)pos.y < mapData.size()
		&& pos.x >= 0 && (unsigned)pos.x < mapData[pos.y].size())
	{
		return mapData[pos.y][pos.x];
	}

	return MAP_NON;
}

bool MapCtrl::IsMove(const VECTOR2& pos,DRAW_DIR dir) const
{
	auto nowMapData = GetMapData(pos);
	auto nextMapData = GetMapData(pos, dir);
	if (nowMapData == MAP_SCREEN)
	{
		nowMapData = GetCameraMapData();
	}
	if (nextMapData == MAP_SCREEN)
	{
		nextMapData = GetCameraMapData();
	}
	if (nowMapData & MOVABLE_NOW[dir] && nextMapData & MOVABLE_NEXT[dir])
	{
		return true;
	}
	return false;
}

MAP_ID MapCtrl::GetObjMapData(const VECTOR2 & vec) const
{
	VECTOR2 pos(vec.x / lpGameTask->chipSize.x, vec.y / lpGameTask->chipSize.y);

	if (pos.y >= 0 && (unsigned)pos.y < mapData.size()
		&& pos.x >= 0 && (unsigned)pos.x < mapData[pos.y].size())
	{
		return objMapData[pos.y][pos.x];
	}
	return MAP_NON;
}


MAP_ID MapCtrl::GetObjMapData(const VECTOR2 & vec, DRAW_DIR dir, int num) const
{
	VECTOR2 pos = vec;

	switch (dir)
	{
	case DIR_DOWN:
		pos.y += lpGameTask->chipSize.y * num + 1;
		break;
	case DIR_UP:
		pos.y -= ( 1 + lpGameTask->chipSize.y * (num-1));
		break;
	case DIR_RIGHT:
		pos.x += lpGameTask->chipSize.x * num + 1;
		break;
	case DIR_LEFT:
		pos.x -= (1 + lpGameTask->chipSize.x * (num - 1));
		break;
	default:
		break;
	}

	pos.x = pos.x / lpGameTask->chipSize.x;
	pos.y = pos.y / lpGameTask->chipSize.y;

	if (pos.y >= 0 && (unsigned)pos.y < mapData.size()
		&& pos.x >= 0 && (unsigned)pos.x < mapData[pos.y].size())
	{
		return mapData[pos.y][pos.x];
	}

	return MAP_NON;
}

void MapCtrl::ClearObjMap()
{
	for (int y = 0; y < objMapData.size(); ++y)
	{
		for (int x = 0; x < objMapData[y].size(); ++x)
		{
			objMapData[y][x] = MAP_NON;
		}
	}
}

void MapCtrl::ClearObjMap(MAP_ID mapObj)
{
	for (int y = 0; y < objMapData.size(); ++y)
	{
		for (int x = 0; x < objMapData[y].size(); ++x)
		{
			if (objMapData[y][x] == mapObj)
			{
				objMapData[y][x] = MAP_NON;
			}
		}
	}
}

void MapCtrl::SetObjMap(const VECTOR2 & vec, MAP_ID mapObj)
{
	VECTOR2 pos(vec.x / lpGameTask->chipSize.x, vec.y / lpGameTask->chipSize.y);

	if (pos.y >= 0 && (unsigned)pos.y < mapData.size()
		&& pos.x >= 0 && (unsigned)pos.x < mapData[pos.y].size())
	{
		objMapData[pos.y][pos.x] = mapObj;
	}

}

void MapCtrl::SetCamera(Camera * pCamera)
{
	camera.reset(pCamera);
}

