#pragma once
#include "Projectile.h"


class CLaser :
	public CProjectile
{
	friend class CScene;

protected:

	class CColliderLine* m_Coll;
	class CGameObject* m_CollidingObj;
	float m_CollidingObjYPos;  //현재 충돌중인 게임오브젝트의 y값

	UINT8 m_ColorNum[4];    //4가지 색상 컬러
	UINT8 m_Index;
	bool  m_ChangeIndex;

	float m_KeepFire;    //계속 발사 중인지
	float m_KeepFireRefVal;

	float m_DamageFreq;  //데미지를 주는 빈도수. 레이저 존재 시간에 맞추어서 계산된다.
	float m_DamageFreqRefVal;

	Vector2 m_Offset;

	bool m_TurnOff; //레이저 발사 종료될 때 한번만 실행될수있게 하는 변수

protected:
	CLaser();
	CLaser(const CLaser& Obj);
	virtual ~CLaser();

public:
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual bool LoadResource();

	void SetKeepFire(bool KeepFire);
	float GetKeepFire() const;
	void LaserLineOffEnd();
	void LaserHitNotify();


	virtual void SetEssential(const Vector2& Dir, const Vector2& Pos, const Vector2& Offset);
	virtual void SetEssential(float DirX, float DirY, float PosX, float PosY, float OffsetX, float OffsetY);



private:
	void CollisionBegin(class CCollider* Src, class CCollider* Dest);
	void CollisionEnd(class CCollider* Src, class CCollider* Dest);
	static bool SortCollY(const CSharedPtr<class CCollider>& Src, const CSharedPtr<class CCollider>& Dest);
};

