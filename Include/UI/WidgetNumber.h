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


protected://텍스처
    CSharedPtr<class CAnimationSequence>  m_NumberImage;
public:

    //텍스처 설정 - 텍스처 로딩 먼저 한뒤 사용할것.
    bool SetNumberImage(const std::string& Name);


protected://숫자 지정
    int m_Number;
    //한단위씩 떼어서 숫자를 저장할 벡터와 스택
    std::vector<int> m_NumberVec;
    std::stack<int> m_NumberStack;
public:
    void SetNumber(int Num);
    void AddNumber(int Num);


protected://숫자 이미지를 확대하고 싶을 경우
    float m_Scale;
public:
    void SetScale(float Scale);



public:
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);
};

