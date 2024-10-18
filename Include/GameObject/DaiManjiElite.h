#pragma once
#include "DaiManji.h"

enum EDaiManjiElitePhase2Flag
{
	EDMPhase0_Docking,
	EDMPhase1,
	EDMPhase2,
	EDMPhase3,
	EDMPhase4_BattleStart,
	EDMPhase5_RandomAction
};

enum EDaiManjiEliteActions
{
	Idle,
	SpawnNormal,
	SpawnElite,
	Energyball,
	End
};

class CDaiManjiElite :
	public CDaiManji
{
	friend class CScene;

protected:
	CDaiManjiElite();
	CDaiManjiElite(const CDaiManjiElite& Obj);
	virtual ~CDaiManjiElite();
public:
	
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual int InflictDamage(int Damage);
	virtual void SetEssential(float DestPosX);




private:
	bool m_LastCutScene;
	class CRugname* m_Rugname;
	CooltimeChecker m_Timer;
	CooltimeChecker m_ActionTimer;
	EDaiManjiEliteActions m_Action;
	bool m_ForceMove;	//구석에 몰렸을때 강제로 가운데로 이동시킴.
	bool m_Phase2;
	INT16 m_SpawnQueue;
	virtual void Routine(float DeltaTime);
	void Phase2Routine(float DeltaTime);


	bool m_InAction;
	void AttackByAction();
	void EliteUFOSpawn();
	void NormalUFOSpawn();
	void DaiManjiDeployEnd();
	void DaiManjiChargingEnd();	//여기서 실제 에너지볼이 발사된다.
	void DaiManjiReverseEnd();
};

