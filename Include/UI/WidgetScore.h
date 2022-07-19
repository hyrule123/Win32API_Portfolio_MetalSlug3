#pragma once
#include "WidgetTextImage.h"

class CWidgetScore :
    public CWidgetTextImage
{
    friend class CWidgetWindow;

protected:
    CWidgetScore();
    CWidgetScore(const CWidgetScore& widget);
    virtual ~CWidgetScore();
public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);

private:
    float m_Timer;
};

