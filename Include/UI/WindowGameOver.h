#pragma once
#include "WidgetWindow.h"
class CWindowGameOver :
    public CWidgetWindow
{
    friend class CScene;

protected:
    CWindowGameOver();
    virtual ~CWindowGameOver();
public:
    virtual bool Init();
    virtual void Update(float DeltaTime);

private:
    bool m_PlayGameDelayOn;
    float m_Delay;

    void ReturnMainMenuCallBack();
};
