#pragma once
#include "Widget.h"


class CWidgetImage :
    public CWidget
{

    friend class CWidgetWindow;

protected:
    CWidgetImage();
    CWidgetImage(const CWidgetImage& widget);
    virtual ~CWidgetImage();
public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);



protected://�ؽ�ó
    CSharedPtr<class CTexture>  m_Texture;


protected:
    bool m_isSizeSet;
public:
    void SetSize(float _x, float _y);
    void SetSize(const Vector2& Size);



    //���� �̹����� ������ ������ ����Ұ����� ����
    BoxInfo m_ImageRenderRange;
    //���ǰ��� �������� �ʾҴٸ� �׳� �̹��� size��ŭ ���
    bool m_isRangeSet;
public:
    void SetRange(const Vector2& Start, const Vector2& End);

    //�ؽ�ó ���� - �ؽ�ó �ε� ���� �ѵ� ����Ұ�.
    bool SetTexture(const std::string& Name);




};

