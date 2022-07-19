#pragma once
#include "Enemy.h"



enum class EMeteorSize
{
    Small,
    Midium,
    Big
};


class CMeteor :
    public CEnemy
{
    friend class CScene;
protected:
    CMeteor();
    CMeteor(const CMeteor& Obj);
    virtual ~CMeteor();
public:
    virtual bool LoadResource();
    virtual bool Init(CGameObject* Obj = nullptr);
};

