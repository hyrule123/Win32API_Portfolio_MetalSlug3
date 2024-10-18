#pragma once
#include "Meteor.h"

enum class EEnterDir
{
	Left,
	Top,
	Right,
	Bottom
};

class CMeteorBig :
	public CMeteor
{
	friend class CScene;
protected:
	CMeteorBig();
	CMeteorBig(const CMeteorBig& Obj);
	virtual ~CMeteorBig();
public:
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual void SetEssential(float StartPosX, float DestPosX);
	virtual int InflictDamage(int Damage);


private:
	class CPlayer* m_Player;
	class CColliderBox* m_PlayerColl;
	class CColliderBox* m_MyColl;
	bool m_isCheckedColl;
	
	
	virtual void CollisionBegin(class CCollider* Src, class CCollider* Dest);
	virtual void CollisionEnd(class CCollider* Src, class CCollider* Dest);
	void CollisionUpdate();
	void HitEffectEnd();
	void MeteorBigDestroy();


};

