#pragma once

#include "../../Ref.h"

class CFont :
    public CRef
{
    friend class CFontManager;

private:
    CFont();
    ~CFont();

private:
    LOGFONT m_FontInfo;
    HFONT   m_hFont;
    HFONT   m_hPrevFont;

public:
    bool LoadFontInfo(const std::string& Name, const std::tstring& FontName, int Width, int Height);
    void SetFont(HDC hDC);
    void ResetFont(HDC hDC);
};

