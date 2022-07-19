#pragma once
#include "WidgetWindow.h"
class CWindowEdit :
    public CWidgetWindow
{
    friend class CScene;

protected:
    CWindowEdit();
    virtual ~CWindowEdit();
public:
    virtual bool Init();
    virtual void Update(float DeltaTime);


protected:
    class CWidgetText* m_Text;
};

