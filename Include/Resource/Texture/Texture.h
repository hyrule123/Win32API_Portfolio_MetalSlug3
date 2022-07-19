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
        //���� ������ �ִ� �׸��� ������ �ٽ� ���Ƴ����ְ�,
        SelectObject(hMemDC, hPrevBmp);

        //�̹����� ����ִ� �׸��� ������ �����ϰ�,
        DeleteObject(hBmp);

        //���������� �׸��� ������ �����Ѵ�.
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

    //��Ʈ�� �̹����� ������ �� ����� �̹��� ����� �޾ƿ�
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
    //Texture �ε� ����

    //Sprite ����
    bool LoadTexture(const std::tstring& FileName, const std::string& PathName = TEXTURE_PATH, EReactionChannel MaxChannel = EReactionChannel::Normal);
    bool LoadTextureFullPath(const std::tstring& FullPath, EReactionChannel MaxChannel = EReactionChannel::Normal);



    //Frame ���� ��� �޼ҵ�
    bool LoadTextureByTextureNumbers(const std::tstring& FolderPath, int NumberOfTexture, const std::string& PathName = TEXTURE_PATH);

    bool LoadTexture(const std::vector<std::tstring>& vecFileName, const std::string& PathName = TEXTURE_PATH);
    bool LoadTextureFullPath(const std::vector<std::tstring>& vecFullPath);

private:
    //���Ϸκ��� �ε��� �� ����ϴ� �޼ҵ�.
    //�̹������� 1��
    bool LoadTexture(ImageInfo* Info, const TCHAR* FileName,
        const std::string& PathName = TEXTURE_PATH);
    bool LoadTextureFullPath(ImageInfo* Info, const TCHAR* FullPath);

    //�̹������� ������
    bool LoadTexture(std::vector<ImageInfo*>* vecInfo, const std::vector<std::wstring>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    bool LoadTextureFullPath(std::vector<ImageInfo*>* vecInfo, const std::vector<std::wstring>& vecFullPath);



};

