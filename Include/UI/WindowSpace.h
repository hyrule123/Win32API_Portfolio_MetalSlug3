#pragma once
#include "WidgetWindow.h"
class CWindowSpace :
    public CWidgetWindow
{
    friend class CScene;

protected:
    CWindowSpace();
    CWindowSpace(const CWindowSpace& widget);
    virtual ~CWindowSpace();
public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);

protected:
    class CWidgetTextImage* m_BulletLeft;
    class CWidgetTextImage* m_BombLeft;
    class CWidgetProgressBar* m_HPBar;
    class CWidgetTextImage* m_Score;
    class CWidgetTextImage* m_TimeNumber;
    float m_TimeLeft;
    int m_AdjustTime;

public:
    void ResetTimeLeft();
    void SetBulletLeft(int Bullet);
    void SetBombLeft(int Bomb);
    void SetHPBar(int HP);
    void DisplayHUD();


};

