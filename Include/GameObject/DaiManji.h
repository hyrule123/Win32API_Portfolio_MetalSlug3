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
	bool m_isBoss;	//������ �ൿ������ �ణ �ٸ���
	float m_RotatingAngle;
	Vector2 m_RenderPos;	//m_Pos�� �߽����� UFO�� ȸ���Ѵ�.
	float m_RotatingRadius;	//ȸ�� ����
	class CColliderCircle* m_Coll;

	//UFO ��ȯ ����.
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
	void EndFuncSpawn();	//UFO ���� �ִϸ��̼��� ���. ��ġ�� �������� ����Ǵ� EndFunction
	void CreateUFO();		//������ UFO�� ����
	void CloseHatch();
};

