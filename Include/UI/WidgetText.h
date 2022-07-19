#pragma once
#include "Widget.h"
class CWidgetText :
    public CWidget
{
    friend class CWidgetComponent;
    friend class CWidgetWindow;

protected:
    CWidgetText();
    CWidgetText(const CWidgetText& widget);
    virtual ~CWidgetText();
public:
    virtual bool Init();


protected://����� �ؽ�Ʈ ����
    TCHAR* m_Text;
    int    m_Count;
    int    m_Capacity;
public:
    void SetText(const std::tstring& Text);
    void AddText(const TCHAR Text);
    void AddText(const std::tstring& Text);
    void pop_back();
    void clear();


protected://�ؽ�Ʈ �� ����
    COLORREF    m_TextColor;
public:
    void SetColor(unsigned char r, unsigned char g, unsigned char b);


protected://��Ʈ ����
    CSharedPtr<class CFont> m_Font;
public:
    void SetFont(const std::string& Name);


protected://�� ���ھ� ����Ұ�����
    bool m_OneLetterAtTime;
    float m_Interval; //��� �ð� ����
    float m_TimeCount;
public:
    void SetOneLetterAtTime(bool Enable = true);
    void SetInterval(float Interval);


protected://�׸��� ��� ���� / ����, ������ ���� �Ÿ�����
    bool        m_Shadow;
    COLORREF    m_ShadowColor;
    Vector2     m_ShadowOffset;
public:
    void SetTextShadowColor(unsigned char r, unsigned char g, unsigned char b);
    void EnableShadow(bool Shadow = true);
    void SetShadowOffset(float x, float y);



public:
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);
    virtual void Render(HDC hDC, const Vector2& Pos, float DeltaTime);
};

