#pragma once

#include "../../GameInfo.h"
#include "../../Ref.h"
#include "../../PathManager.h"


struct ImageInfo
{
    HDC         hMemDC;
    HBITMAP     hBmp;
    HBITMAP     hPrevBmp;
    BITMAP      BmpInfo;
    bool        ColorKeyEnable;
    COLORREF    ColorKey;
    TCHAR       FileName[MAX_PATH];
    TCHAR       FullPath[MAX_PATH];
    std::string PathName;

    ImageInfo():
        hMemDC(0),
        hBmp(0),
        hPrevBmp(0),
        BmpInfo{},
        ColorKeyEnable(false),
        ColorKey(RGB(255, 0, 255)),
        FileName{},
        FullPath{}
    {}

    ~ImageInfo()
    {
        //원래 끼워져 있던 그리기 도구로 다시 갈아끼워주고,
        SelectObject(hMemDC, hPrevBmp);

        //이미지가 들어있던 그리기 도구를 삭제하고,
        DeleteObject(hBmp);

        //마지막으로 그리기 도구를 제거한다.
        DeleteDC(hMemDC);
    }
};


class CTexture :
    public CRef
{
    friend class CTextureManager;

private:
    CTexture();
    ~CTexture();


private:
    ETextureType m_TextureType;
    std::vector<ImageInfo*> m_vecImageInfo;


public:

    HBITMAP* GetHBITMAP(int index = 0);
    BITMAP GetBITMAP(int index = 0);

    int GetTextureNumbers() const;

    //비트맵 이미지를 생성할 때 저장된 이미지 사이즈를 받아옴
    Vector2 GetTextureSize(int index = 0)  const;


    ETextureType GetTextureType()   const;

    HDC GetDC(int index = 0) const;

    void SetColorKey(unsigned char r = 255, 
        unsigned char g = 0, 
        unsigned char b = 255, 
        int index = -1);

    COLORREF GetColorKey(int index = 0)  const;

    bool GetColorKeyEnable(int index = 0)  const;

    int GetMaxReactionChannel() const;


private:
    //Texture 로딩 관련

    //Sprite 형식
    bool LoadTexture(const std::tstring& FileName, const std::string& PathName = TEXTURE_PATH, EReactionChannel MaxChannel = EReactionChannel::Normal);
    bool LoadTextureFullPath(const std::tstring& FullPath, EReactionChannel MaxChannel = EReactionChannel::Normal);



    //Frame 형식 등록 메소드
    bool LoadTextureByTextureNumbers(const std::tstring& FolderPath, int NumberOfTexture, const std::string& PathName = TEXTURE_PATH);

    bool LoadTexture(const std::vector<std::tstring>& vecFileName, const std::string& PathName = TEXTURE_PATH);
    bool LoadTextureFullPath(const std::vector<std::tstring>& vecFullPath);

private:
    //파일로부터 로드할 때 사용하는 메소드.
    //이미지파일 1개
    bool LoadTexture(ImageInfo* Info, const TCHAR* FileName,
        const std::string& PathName = TEXTURE_PATH);
    bool LoadTextureFullPath(ImageInfo* Info, const TCHAR* FullPath);

    //이미지파일 여러개
    bool LoadTexture(std::vector<ImageInfo*>* vecInfo, const std::vector<std::wstring>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    bool LoadTextureFullPath(std::vector<ImageInfo*>* vecInfo, const std::vector<std::wstring>& vecFullPath);



};

