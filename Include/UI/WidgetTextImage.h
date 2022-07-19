#pragma once
#include "Widget.h"

//�ؽ�ó ������ ���ĺ��� ���. ���ĺ��� ������� ���������
//1. SetTextImage() -> ����� �ؽ�ó �� �ִϸ��̼� ������ �ε�
//2. SetKerning() - �ڰ� ����
//3. SetAlign() - ���� ��� ����




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


protected://�ؽ�ó
    CSharedPtr<class CAnimationSequence>  m_TextImage;
public:

    //�ؽ�ó ���� - �ؽ�ó �ε� ���� �ѵ� ����Ұ�.
    bool SetTextImage(const std::string& Name);


protected://���� ����
    std::string m_Text;
    UINT8 m_Kerning; //�ڰ�.
    std::vector<INT16> m_vecCharWidth;  //������ ���� x�� ����. ���ں��� �ٸ� �� �����Ƿ� vector��
    //������ y�� ���̴� m_Size.y�� �����Ѵ�.
    int m_RenderPosX;   //���� ���ڿ��� x���� ���̸� ����. �Ź� ��� �� �δ��� �ǹǷ� �ؽ�Ʈ �߰��ÿ� ���
    EAlignMode m_AlignMode;
public:
    void SetText(const std::string& Txt);
    void AddText(const std::string& Txt);
    void SetText(int Num);
    void AddText(int Num);      //���ڸ� ������
    void SetBulletLeft(int Num);    //0�� �� ���� Ư�����ڸ� ǥ���Ѵ�.
    void SetScore(int Score);       //���ڸ��� 0�� ���ش�.
    void SetKerning(INT8 Kerning);
    void SetAlignMode(EAlignMode AlignMode);


protected://���� �̹����� Ȯ���ϰ� ���� ���
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

