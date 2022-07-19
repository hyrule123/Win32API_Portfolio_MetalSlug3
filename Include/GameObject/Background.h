#pragma once
#include "GameObject.h"

class CBackground :
    public CGameObject
{
    friend class CScene;

protected:
    CBackground();
    ~CBackground();
public:
    bool Init(CGameObject* Obj = nullptr);
    void Start();


public:
    void Update(float DeltaTime);
    void Render(HDC hDC, float DeltaTime);
};

