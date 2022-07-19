#pragma once
#include "Widget.h"

//텍스처 형태의 알파벳을 출력. 알파벳은 순서대로 집어넣을것
//1. SetTextImage() -> 사용할 텍스처 및 애니메이션 시퀀스 로드
//2. SetKerning() - 자간 설정
//3. SetAlign() - 정렬 방식 설정




class CWidgetTextImage :
    public CWidget
{
    friend class CWidgetWindow;

protected:
    CWidgetTextImage();
    CWidgetTextImage(const CWidgetTextImage& widget);
    virtual ~CWidgetTextImage();
public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);


protected://텍스처
    CSharedPtr<class CAnimationSequence>  m_TextImage;
public:

    //텍스처 설정 - 텍스처 로딩 먼저 한뒤 사용할것.
    bool SetTextImage(const std::string& Name);


protected://문자 지정
    std::string m_Text;
    UINT8 m_Kerning; //자간.
    std::vector<INT16> m_vecCharWidth;  //문자의 개별 x축 길이. 문자별로 다를 수 있으므로 vector로
    //문자의 y축 길이는 m_Size.y에 저장한다.
    int m_RenderPosX;   //현재 문자열의 x축의 길이를 저장. 매번 계산 시 부담이 되므로 텍스트 추가시에 계산
    EAlignMode m_AlignMode;
public:
    void SetText(const std::string& Txt);
    void AddText(const std::string& Txt);
    void SetText(int Num);
    void AddText(int Num);      //숫자를 더해줌
    void SetBulletLeft(int Num);    //0일 떄 무한 특수문자를 표시한다.
    void SetScore(int Score);       //앞자리에 0을 써준다.
    void SetKerning(INT8 Kerning);
    void SetAlignMode(EAlignMode AlignMode);


protected://숫자 이미지를 확대하고 싶을 경우
    float m_Scale;
public:
    void SetScale(float Scale);

private:
    void CalcTextLen();

protected:
	CooltimeChecker m_Flicker;
    CooltimeChecker m_FlickerDuration;
    bool            m_FlickerSwitch;
    EReactionChannel m_ColorFlicker;
public:
    void SetFlicker(float Duration, float Frequency);
    void SetColorFlicker(float Duration, float Frequency, EReactionChannel ColorFlicker);
    void SetColor(float Duration, EReactionChannel ColorFlicker);
    bool GetFlickerDone() const;
 
};

