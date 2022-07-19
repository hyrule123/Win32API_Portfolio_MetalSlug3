#pragma once
#include "MeteorBig.h"

enum class EMeteorBigHalfPart
{
    Left,
    Right
};

class CMeteorBigHalf :
    public CMeteor
{
    friend class CScene;
protected:
    CMeteorBigHalf();
    CMeteorBigHalf(const CMeteorBigHalf& Obj);
    virtual ~CMeteorBigHalf();
public:
    virtual bool Init(CGameObject* Obj = nullptr);
    virtual void Start();
    virtual void Update(float DeltaTime);
    virtual void SetEssential(const Vector2& Pos, EMeteorBigHalfPart Part);

private:
    EMeteorBigHalfPart m_Part;

};

