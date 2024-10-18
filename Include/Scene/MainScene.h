#pragma once

#include "Scene.h"

enum ESpacePhase
{
	ESPhase0,
	ESPhase1_Wait,
	ESPhase2_Wait,
	ESPhase3_Wait,
	ESPhase4_UFOSpawn1,
	ESPhase4_1_Wait,
	ESPhase5_UFOPhase2,
	ESPhase6_1_Wait,
	ESPhase7_Meteor,
	ESPhase8_BigMeteor0,
	ESPhase9_BigMeteor1,
	ESPhase10_BigMeteor2,
	ESPhase11_BigMeteor3,
	ESPhase12_UFO0,
	ESPhase12_1_Wait,
	ESPhase13_UFO1,
	ESPhase13_1_Wait,
	ESPhase14_UFO2,
	ESPhase14_1_Wait,
	ESPhase15_UFO3,
	ESPhase15_1_Wait,
	ESPhase16_Meteor1,
	ESPhase17_Meteor2,
	ESPhase18_Meteor3,
	ESPhase19_Meteor4,
	ESPhase20_DaiManji1,
	ESPhase20_1_Wait,
	ESPhase21_DaiManjiElite,
	ESPhase22_Ending1,
	ESPhase23_Ending2,
	ESPhase24_Ending3


};


class CMainScene :
	public CScene
{
	friend class CSceneManager;

protected:
	CMainScene();
	virtual ~CMainScene();
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);

protected:
	class CBackgroundSpace* m_BackGround;


private://Phase 관련 변수 메소드 모음
	UINT8 m_PhaseFlags;
	CooltimeChecker m_Timer0;
	CooltimeChecker m_Timer1;
	CooltimeChecker m_Timer2;

	INT16 m_Counter;
	INT16 m_KeyMonsterCounter;
	INT16 m_MiniUFOCounter;
	INT16 m_MiniUFOMax;
	bool m_FirstPhaseEnter;
	bool m_InCutscene;
public:
	void GoNextPhase();//다음 페이즈 진입시 반드시 이 함수를 통해 진입할것
	bool CheckFirstEnter();  //페이스 첫진입 확인용 함수
	void AddMiniUFOCounter(int Num);
	bool GetMiniUFOCounter() const; //갯수를 반환하지 않고 추가로 생성해도 되는지를 반환
	void SetCutsceneMode(bool On);
	bool CheckCutscenePlay() const;



public:
	
	void SetScrollMapSpeed(float Speed);
	void SetScrollMapSpeedSoft(float AdjustSpeed, float SpeedPerSec);
	void KeyMonsterDead();


private:
	void LoadResource();
	void PhaseUpdate(float DeltaTime);

	//기다리는 페이즈용 함수. 페이즈 안에서 사용하는 용도임.
	void WaitPhase(float WaitTime, float DeltaTime);
	EItemList RandItem();
	EItemList RandWeapon();
};

