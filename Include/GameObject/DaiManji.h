#pragma once
#include "Enemy.h"

enum class EDaiManjiRoutine
{
	Moveto,
	Wait1,
	Spawn,
	Wait2
};

enum class EDaiManjiLayerInfo
{
	Hatch,
	Body,
	HatchAction,
	SFX
};


class CDaiManji :
    public CEnemy
{
	friend class CScene;

protected:
	CDaiManji();
	CDaiManji(const CDaiManji& Obj);
	virtual ~CDaiManji();
public:
	virtual bool LoadResource();
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual int InflictDamage(int Damage);
	virtual void SetEssential(float DestPosX);
	void SetBoss(bool isBoss);

protected:
	bool m_isBoss;	//보스는 행동패턴을 약간 다르게
	float m_RotatingAngle;
	Vector2 m_RenderPos;	//m_Pos를 중심으로 UFO가 회전한다.
	float m_RotatingRadius;	//회전 지름
	class CColliderCircle* m_Coll;

	//UFO 소환 관련.
	INT8 m_UFOSpawnQueue;
	INT8 m_UFOSpawnMAX;
	class CMainScene* m_MainScene;
	



	//void CollBegin(class CCollider* Src, class CCollider* Dest);
protected:
	CooltimeChecker m_Timer;
	CooltimeChecker m_ExplosionTimer;
	float	m_DeathTimer;
	virtual void Routine(float DeltaTime);
	void SpawnMiniUFO();
	void EndFuncSpawn();	//UFO 스폰 애니메이션을 재생. 해치가 열리고나서 실행되는 EndFunction
	void CreateUFO();		//실제로 UFO를 생성
	void CloseHatch();
};

