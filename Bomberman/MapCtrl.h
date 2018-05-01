#pragma once
#include "VECTOR2.h"
#include "Object.h"
#include <vector>
#include<memory>

enum MAP_ID
{
	MAP_NON,
	MAP_UP,
	MAP_DOWN,
	MAP_UP_DOWN,
	MAP_RIGHT,
	MAP_UP_RIGHT,
	MAP_DOWN_RIGHT,
	MAP_NOT_LEFT,
	MAP_LEFT,
	MAP_UP_LEFT,
	MAP_DOWN_LEFT,
	MAP_NOT_RIGHT,
	MAP_RIGHT_LEFT,
	MAP_NOT_DOWN,
	MAP_NOT_UP,
	MAP_INTERSECTION,
	MAP_SCREEN,
	MAP_CAMERA,
	MAP_PLAYER,
	MAP_ENEMY,
	MAP_ID_MAX
};


#define START_EDIT_CHIP MAP_NON
#define END_EDIT_CHIP MAP_ENEMY
#define START_GAME_CHIP MAP_NON
#define END_GAME_CHIP MAP_INTERSECTION

#define lpMapCtrl 	MapCtrl::GetInstance()

struct DataHeader
{
	char fileID[13];			// �t�@�C����ID���
	unsigned char verID;		// �t�@�C���̃o�[�W�������
	unsigned char res1[2];		// �A���C�����g���\��̈�Ƃ��Ă���
	int sizeX;					// �}�b�v�̃T�C�YX
	int sizeY;					// �}�b�v�̃T�C�Y
	unsigned char res2[3];		// �A���C�����g���\��̈�Ƃ��Ă���
	unsigned char sum;			// SUM�l
};

#define BBM_ID_NAME "BBM_MAP_DATA"	// �t�@�C�����
#define BBM_VER_ID 0x05			// �t�@�C���o�[�W����

#define MOVABLE_UP 0x01
#define MOVABLE_DOWN 0x02
#define MOVABLE_RIGHT 0x04
#define MOVABLE_LEFT 0x08


#define PI 3.14159265f
#define DEG(X) ((X)*PI/180.0f)


using VEC_MAP_ID = std::vector<std::vector<MAP_ID>>;
using VEC_ITEM_MAP_ID = std::vector<std::vector<bool>>;
using VEC_OBJ_MAP_ID = std::vector<std::vector<MAP_ID>>;

class Camera;

class MapCtrl
{
public:
	~MapCtrl();
	static void Create();
	static MapCtrl* GetInstance()
	{
		if (s_inst == nullptr)
		{
			Create();
		}

		return s_inst;
	}
	static void Destroy();

	bool reSetUp(MAP_ID resetWall = MAP_NON);
	bool MapSave(std::string fileName);
	bool MapLoad(std::string fileName);
	void MapDraw();

	void SetUpChar();

	bool SetMapData(MAP_ID id,const VECTOR2& pos);
	bool SetMapData(MAP_ID id, const VECTOR2& pos, DRAW_DIR dir);
	bool FillMapData(MAP_ID id, const VECTOR2& pos);

	MAP_ID GetMapData(const VECTOR2& pos) const;
	MAP_ID GetMapData(const VECTOR2& pos, DRAW_DIR dir) const;

	///���݈ʒu����w������ւ̈ړ����\���`�F�b�N
	bool IsMove(const VECTOR2& pos,DRAW_DIR dir) const;

	///�I�u�W�F�}�b�v�̃f�[�^�𓾂�
	MAP_ID GetObjMapData(const VECTOR2& pos) const;
	MAP_ID GetObjMapData(const VECTOR2& pos, DRAW_DIR dir, int num = 1) const;

	///�I�u�W�F�}�b�v���N���A����
	void ClearObjMap();

	///�w�肵���I�u�W�F�N�g���}�b�v�f�[�^����N���A����
	void ClearObjMap(MAP_ID mapObj);
	
	///�w�肵�����W�ɃI�u�W�F��ݒ肷��
	void SetObjMap(const VECTOR2& pos, MAP_ID mapObj);


	///�J�����I�u�W�F�N�g���}�b�v�Ɋ֘A�t����
	void SetCamera(Camera* pCamera);

	const VECTOR2 mapSize{ 38, 28 };
private:
	MapCtrl();
	bool _FillMapData(MAP_ID id, int x, int y, MAP_ID fillTarget);
	static MapCtrl* s_inst;
	VEC_MAP_ID mapData;
	VEC_OBJ_MAP_ID objMapData;
	std::shared_ptr<Camera> camera;


	const int MOVABLE_NOW[4] = { MOVABLE_DOWN, MOVABLE_LEFT, MOVABLE_RIGHT, MOVABLE_UP };
	const int MOVABLE_NEXT[4] = { MOVABLE_UP, MOVABLE_RIGHT,MOVABLE_LEFT,MOVABLE_DOWN };
	MAP_ID GetCameraMapData() const;
	///�J������������W�̃}�b�v�I�u�W�F�N�g��Ԃ�
	MAP_ID GetCameraObjMapData() const;
};

