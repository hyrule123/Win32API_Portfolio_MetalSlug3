#pragma once
#include "GameObject.h"

enum EPhase
{
	EPhase0,
	EPhase1,
	EPhase2,
	EPhase3,
	EPhase4,
	EPhaseMax
};

enum RocketBodyInfo
{
	Ring,
	Body,
	Thrust,
	ERocketBodyInfoMax
};



//여러 개의 로켓 몸통을 한데 묶어 관리하는 관리 클래스
class CSpaceIntro :
	public CGameObject
{
	friend class CScene;
protected:
	CSpaceIntro();
	CSpaceIntro(const CSpaceIntro& Obj);
	virtual ~CSpaceIntro();
public:
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);

protected:
	float m_StartPoint;
	UINT8 m_Phase;
	UINT8 m_PhaseFlags;

	class CIntroRocket* m_RocketPart[EPhaseMax][ERocketBodyInfoMax];
	class CIntroRocket* m_RocketNoseLeftUpper;
	class CIntroRocket* m_RocketNoseLeftLow;
	class CIntroRocket* m_RocketNoseRight;

	class CAstroSlug* m_Player;
	float m_DeleteTimer;

	void Phase0();
	void Phase1();
	void Phase23();
	void Phase4();
	void CutSceneOver();

	
};

