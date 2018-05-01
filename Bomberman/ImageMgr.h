#pragma once
#include<map>
#include<vector>
#include"VECTOR2.h"

using VEC_INT = std::vector<int>;
#define IMAGE_ID(X) (ImageMgr::GetInstance()->GetID(X))

class ImageMgr
{
public:
	static void Create();
	static ImageMgr* GetInstance()
	{
		if (s_Instance == nullptr)
		{
			Create();
		}
		return s_Instance;
	}
	static void Destroy();
	~ImageMgr();

	const VEC_INT& GetID(std::string f_name);
	const VEC_INT& GetID(std::string f_name,VECTOR2 divSize, VECTOR2 divCnt);

private:
	ImageMgr();
	static ImageMgr *s_Instance;
	std::map<std::string, VEC_INT> imgMap;
	
};

