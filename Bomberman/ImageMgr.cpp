#include "ImageMgr.h"
#include"DxLib.h"
ImageMgr* ImageMgr::s_Instance;


ImageMgr::ImageMgr()
{
	s_Instance = nullptr;
}


void ImageMgr::Create()
{
	if (!s_Instance)
	{
		s_Instance = new ImageMgr();
	}
}

void ImageMgr::Destroy()
{
	if (s_Instance)
	{
		delete s_Instance;
		s_Instance = nullptr;
	}
}

ImageMgr::~ImageMgr()
{
}

const VEC_INT & ImageMgr::GetID(std::string f_name)
{
	if (imgMap.find(f_name) == imgMap.end())
	{
		imgMap[f_name].resize(1);
		imgMap[f_name][0] = LoadGraph(f_name.c_str());
	}
	return imgMap[f_name];
}

const VEC_INT & ImageMgr::GetID(std::string f_name, VECTOR2 divSize, VECTOR2 divCnt)
{
	if (imgMap.find(f_name) == imgMap.end())
	{
		imgMap[f_name].resize(divCnt.x*divCnt.y);
		if (LoadDivGraph(f_name.c_str(), divCnt.x*divCnt.y, divCnt.x, divCnt.y, divSize.x, divSize.y, &imgMap[f_name][0]) == -1)
		{
			MessageBox(NULL, "ÉçÅ[Éhé∏îs", "ÉçÅ[Éhé∏îs", MB_OK);
		}
	}
	return imgMap[f_name];
}

