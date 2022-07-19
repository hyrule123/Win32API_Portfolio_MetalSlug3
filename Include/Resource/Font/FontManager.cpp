
#include "FontManager.h"
#include "Font.h"
#include "../../PathManager.h"

#include "../../GameManager.h"

CFontManager::CFontManager()
{
}

CFontManager::~CFontManager()
{
	size_t size = m_LoadedFontFullpath.size();

	for (size_t i = 0; i < size; ++i) 
	{
		RemoveFontResource(m_LoadedFontFullpath[i].c_str());
	}
}

bool CFontManager::Init()
{
	LoadFontResource(TEXT("Noto Sans KR Black.otf"));
	LoadFontResource(TEXT("µÕ±Ù¸ð²Ã.otf"));


	LoadFontInfo("DefaultFont", TEXT("Noto Sans KR Black"), 0, 0);
	LoadFontInfo("PixelFont", TEXT("µÕ±Ù¸ð²Ã"), 12, 12);

	return true;
}

bool CFontManager::LoadFontInfo(const std::string& Name, const std::tstring& FontName, 
	int Width, int Height)
{
	CFont* Font = FindFont(Name);

	if (Font)
		return true;

	Font = new CFont;


	Font->SetName(Name);


	if (!Font->LoadFontInfo(Name, FontName, Width, Height))
	{
		SAFE_DELETE(Font);
		return false;
	}

	m_mapFont.insert(std::make_pair(Name, Font));

	return true;
}

bool CFontManager::LoadFontResource(
	const std::tstring& FontFileName, 
	const std::string& PathName)
{
	TCHAR	FullPath[MAX_PATH] = {};

	const PathInfo* Info = CPathManager::GetInst()->FindPath(FONT_PATH);

	if (Info)
		lstrcpy(FullPath, Info->Path);

	lstrcat(FullPath, FontFileName.c_str());

	if (AddFontResource(FullPath) == 0)
		return false;

	m_LoadedFontFullpath.push_back(FullPath);

	return true;
}

void CFontManager::SetFont(const std::string& Name, HDC hDC)
{
	CFont* Font = FindFont(Name);

	if (!Font)
		return;

	Font->SetFont(hDC);
}

void CFontManager::ResetFont(const std::string& Name, HDC hDC)
{
	CFont* Font = FindFont(Name);

	if (!Font)
		return;

	Font->ResetFont(hDC);
}

CFont* CFontManager::FindFont(const std::string& Name)
{
	auto	iter = m_mapFont.find(Name);

	if (iter == m_mapFont.end())
		return nullptr;

	return iter->second;
}

void CFontManager::ReleaseFont(const std::string& Name)
{
	auto	iter = m_mapFont.find(Name);

	if (iter == m_mapFont.end())
		return;

	if (iter->second->GetRefCount() == 1)
		m_mapFont.erase(iter);
}
