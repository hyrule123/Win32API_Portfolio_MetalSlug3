#pragma once

#include "../../GameInfo.h"

class CFontManager
{
	friend class CResourceManager;

private:
	CFontManager();
	~CFontManager();
public:
	bool Init();

private:
	std::unordered_map<std::string, CSharedPtr<class CFont>>	m_mapFont;
	std::vector<std::tstring> m_LoadedFontFullpath;

public:
	class CFont* FindFont(const std::string& Name);


	bool LoadFontInfo(const std::string& Name, const std::tstring& FontName,
		int Width, int Height);

	bool LoadFontResource(
		const std::tstring& FontFileName,
		const std::string& PathName = FONT_PATH);
	void SetFont(const std::string& Name, HDC hDC);
	void ResetFont(const std::string& Name, HDC hDC);
	void ReleaseFont(const std::string& Name);
};

