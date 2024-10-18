#include "PathManager.h"

DEFINITION_SINGLETON(CPathManager)

CPathManager::CPathManager()
{

}

CPathManager::~CPathManager()
{
	auto iter = m_mapPath.begin();
	auto iterEnd = m_mapPath.end();
	while (iter != iterEnd)
	{
		SAFE_DELETE(iter->second);
		++iter;
	}

}

bool CPathManager::Init()
{
	TCHAR Root[MAX_PATH] = {};

	GetModuleFileName(0, Root, MAX_PATH);

	{
		int length = lstrlen(Root) - 1;
		for (int i = length; i >= 0; --i)
		{
			if (Root[i] == '/' || Root[i] == '\\')
			{

				memset(&Root[i + 1], 0, sizeof(TCHAR) * ((size_t)length - (size_t)i - (size_t)1));
				break;
			}
		}
	}

	PathInfo* Info = new PathInfo;

	lstrcpy(Info->Path, Root);

	//유니코드면 변환 함수를 통해 저장
#ifdef UNICODE
	int length = WideCharToMultiByte(CP_ACP, 0, Info->Path, -1, nullptr, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, Info->Path, -1, Info->PathMultiByte, length, 0, 0);

	//유니코드가 아니면 그대로 복사
#else
	strcpy_s(Info->PathMultiByte, Info->Path);
#endif
	//루트 경로를 기본으로 등록
	m_mapPath.insert(std::make_pair(ROOT_PATH, Info));

	//루트 경로안의 여러 리소스 폴더를 등록
	AddPath(TEXTURE_PATH, TEXT("Texture/"), ROOT_PATH);
	AddPath(SOUND_PATH, TEXT("Sound/"), ROOT_PATH);
	AddPath(FONT_PATH, TEXT("Font/"), ROOT_PATH);
	AddPath(DATA_PATH, TEXT("Data/"), TEXTURE_PATH);

	return true;
}

bool CPathManager::AddPath(const std::string& Name, const std::tstring& addpath, const std::string& basepathName)
{
	if (FindPath(Name))
		return false;

	const PathInfo* BasePath = FindPath(basepathName);

	PathInfo* Info = new PathInfo;

	if (BasePath)
		lstrcpy(Info->Path, BasePath->Path);

	lstrcat(Info->Path, addpath.c_str());

	//유니코드면 변환 함수를 통해 저장
#ifdef UNICODE
	int length = WideCharToMultiByte(CP_ACP, 0, Info->Path, -1, nullptr, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, Info->Path, -1, Info->PathMultiByte, length, 0, 0);

	//유니코드가 아니면 그대로 복사
#else
	strcpy_s(Info->PathMultiByte, Info->Path);
#endif


	m_mapPath.insert(std::make_pair(Name, Info));

	return true;
}

const PathInfo* CPathManager::FindPath(const std::string& Name)
{
	auto iter = m_mapPath.find(Name);

	if(iter == m_mapPath.end())
		return nullptr;

	return iter->second;
}
