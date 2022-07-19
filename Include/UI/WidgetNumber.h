#pragma once
#include "Widget.h"


class CWidgetNumber :
    public CWidget
{
    friend class CWidgetWindow;

protected:
    CWidgetNumber();
    CWidgetNumber(const CWidgetNumber& widget);
    virtual ~CWidgetNumber();
public:
    virtual bool Init();


protected://�ؽ�ó
    CSharedPtr<class CAnimationSequence>  m_NumberImage;
public:

    //�ؽ�ó ���� - �ؽ�ó �ε� ���� �ѵ� ����Ұ�.
    bool SetNumberImage(const std::string& Name);


protected://���� ����
    int m_Number;
    //�Ѵ����� ��� ���ڸ� ������ ���Ϳ� ����
    std::vector<int> m_NumberVec;
    std::stack<int> m_NumberStack;
public:
    void SetNumber(int Num);
    void AddNumber(int Num);


protected://���� �̹����� Ȯ���ϰ� ���� ���
    float m_Scale;
public:
    void SetScale(float Scale);



public:
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);
};

