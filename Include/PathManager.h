#pragma once

#include "Singleton.h"
#include "GameInfo.h"


struct PathInfo
{
	TCHAR Path[MAX_PATH];
	char PathMultiByte[MAX_PATH];


	PathInfo():
		Path{},
		PathMultiByte{}
	{}
};


class CPathManager
{
	
private:
	std::unordered_map <std::string, PathInfo*> m_mapPath;


public:
	bool Init();

	bool AddPath(const std::string& Name, 
		const std::tstring& addpath, 
		const std::string& basepathName = ROOT_PATH);

	const PathInfo* FindPath(const std::string& Name);


	DECLARE_SINGLETON(CPathManager)

};

