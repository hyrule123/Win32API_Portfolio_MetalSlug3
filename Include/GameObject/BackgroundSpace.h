#pragma once
#include "Background.h"

//종스크롤
class CBackgroundSpace :
    public CBackground
{
    friend class CScene;

protected:
    CBackgroundSpace();
    CBackgroundSpace(const CBackgroundSpace& Obj);
    ~CBackgroundSpace();
public:
    bool Init(CGameObject* Obj = nullptr);
    void Start();
    void Update(float DeltaTime);
    void Render(HDC hDC, float DeltaTime);

protected:

    float m_ScrollingSpeed;
    float m_ScrollPos;
    float m_AdjustSpeed;
public:
    void SetSpeed(float ScrollingSpeed);
    void SetSpeedSoft(float AdjustSpeed, float SpeedPerSec);
};


