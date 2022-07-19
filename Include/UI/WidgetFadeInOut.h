#pragma once
#include "WidgetImage.h"
class CWidgetFadeInOut :
    public CWidgetImage
{

    friend class CWidgetWindow;

protected:
    CWidgetFadeInOut();
    CWidgetFadeInOut(const CWidgetFadeInOut& widget);
    virtual ~CWidgetFadeInOut();
public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);

protected:
    bool m_isFadeIn;    //true일 시 fadein, false일 시 fadeout
    float m_StepMovePos;    //한스텝마다 얼마나 이동할것인지
    INT8 m_FrameTickLeft;
    INT8 m_FrameTickSet;
    bool m_isSet;
    bool m_End;
	std::function<void()> m_EndFunc;
public:
    void SetEssential(bool isFadeIn);
    void PosCalc();

	template <typename T>
	void AddEndFunction(
		T* classptr,
		void (T::* Func)())
	{
		//함수에 인자를 bind
        m_EndFunc = std::bind(Func, classptr);
	}


};

