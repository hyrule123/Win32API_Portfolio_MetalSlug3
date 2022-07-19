#pragma once

#include "Widget.h"

enum class EProgressBar_TextureType
{
    Back,
    Bar,
    End
};

enum class EProgressBar_Dir
{
    LeftToRight,
    RightToLeft,
    BottomToTop,
    TopToBottom,
    End
};

class CWidgetProgressBar :
    public CWidget
{
    friend class CWidgetWindow;

protected:
    CWidgetProgressBar();
    CWidgetProgressBar(const CWidgetProgressBar& widget);
    virtual ~CWidgetProgressBar();
public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);
    virtual void Render(HDC hDC, const Vector2& Pos, float DeltaTime);

protected:
    CSharedPtr<class CTexture>  m_Texture[(int)EProgressBar_TextureType::End];
    EProgressBar_Dir    m_Dir;
    float               m_Value;    // 0 ~ 1 사이의 값
    float               m_PrevValue;
    //내부 바의 표시 위치
    BoxInfo             m_BarPos;

protected://플리커 관련
    bool  m_HitFlickerOn;
    float m_ReactionTime;
    float m_FlickerFrequency;
    bool  m_Flicker;
    EReactionChannel m_Reaction;

public:
    bool SetHitFlicker(bool On, float Duration);



protected:
    //패딩
    float m_Padding;
public:
    //안쪽 바의 패딩(틀과 안쪽 바의 간격)
    void SetPadding(float Padding);

protected:
    bool m_isSizeSet;
public:
    //프로그레스 바 틀의 사이즈.
    void SetBarFrameSize(float SizeX, float SizeY);
    void SetBarFrameSize(const Vector2& Size);




    bool SetTexture(EProgressBar_TextureType Type, const std::string& Name);
    void SetBarDir(EProgressBar_Dir Dir);
    void SetValue(float Value);
    void AddValue(float Value);




};

