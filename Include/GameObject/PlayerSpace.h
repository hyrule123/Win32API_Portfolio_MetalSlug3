#pragma once
#include "Player.h"

#include "../ScoreManager.h"

enum class EDirSpace
{
	Left2,
	Left1,
	Mid,
	Right1,
	Right2,
	MAX
};

//현재 액션
enum class EDirAction
{
	Idle_Pistol,
	Attack_Pistol,
	Idle_Rifle,
	Attack_HMG,
	Attack_Shotgun,
	Attack_Rocket,	//로켓, 레이저 등 다양하게 사용됨,
	Attack_Laser,
	Rifle_Drop,
	Slug_Ejection,
	Death,
	MAX
};

enum EDirLeftRight
{
	DirLeft = 0,
	DirRight,
	LeftRightEnd
};



class CPlayerSpace :
	public CPlayer
{
	friend class CScene;
protected:
	CPlayerSpace();
	CPlayerSpace(const CPlayerSpace& Obj);
	virtual ~CPlayerSpace();

public:
	virtual bool LoadResource();
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);

protected:


	//현재 좌우 방향과 행동을 저장하는 변수(밑의 함수를 이용해서만 변경)
	//판정 기준은 m_DirTimeMax, m_DitTime의 최댓값은 m_DirTime + m_Tolerance
	float m_DirTime;
	//이동중인 방향에 따라 +-DeltaTime을 해줄 변수.
	//업데이트 순서는 Input->Update->PostUpdate->Render
	float m_DirTimeMax;
	float m_DirTimeMaxHalf;
	float m_Tolerance; //부동소수점 오차 허용범위 설정(좀 크게)

	EDirAction m_CurrentAct;
	EDirAction m_PrevAct;
	EDirSpace m_CurrentDir;
	EDirSpace m_PrevDir;
	EWeapon m_CurrentWeapon;
	bool m_NoActionChange; //활성화 시 다른 액션 불가능

	//방향과 액션에 따른 적절한 애니메이션 이름을 저장하는 문자열 배열
	std::string m_vecAnimName[(int)EDirSpace::MAX][(int)EDirAction::MAX];
	//부스터 이름 저장
	std::string m_vecTailBoosterName[(int)EDirSpace::MAX];
	//좌우(2)에 대한 방향별 측면 부스터 오프셋값
	Vector2 m_vecSideBoosterOffset[(int)EDirSpace::MAX][LeftRightEnd];

	INT8 m_HMGDir;
	int	 m_HMGQueue;		//남은 발사 수
	bool m_GunFired;		//총을 발사했을 때 행동불가 확인 변수
	CooltimeChecker m_HMGDelay;
	class CLaser* m_isLaserFire;	//레이저 발사했는지 확인용

	Vector2 m_Offset;
	bool m_BombTurn;


protected:
	//현재 액션을 계산하는 메소드
	void ComputeCurrentDir(float DeltaTime);
	void ComputeCurrentAct(float DeltaTime); //액션이 변경될때마다 사용



private:
	void MoveLeft();
	void MoveRight();

	void MoveUp();
	void MoveDown();
	void LeftKeyDown();
	void RightKeyDown();

	void IdleStatusNotify();

	void FireGun();//여기선 선입력 갯수만 늘려줌
	void FireGunCheck();//여기선 선입력 수에 따라서 발사
	void AttackEnd();
	void HMGLaserAtkEnd();

	//이하 개별 무기 
	void FireHMG();
	void FireLaser();
	void FireRocket();
	void FireShotgun();



	void FireBomb();
	void FireNotify();

	void RifleDrop();
	void RifleDropEnd();

	virtual void ChangeWeapon(EWeapon Weapon);
	void ChangeRifleHMG();
	void ChangeRifleShotgun();
	void ChangeRifleRocket();
	void ChangeRifleLaser();
	void ChangePistol();
	void GetBomb();
	void GetGas();

	void Death();
	void DeathEnd();

public:
	void SlugEjection();
	virtual int InflictDamage(int Damage = 1);
	void SetCutsceneMode(bool On);

private:
	void SlugEjectionEnd();
	void SetGameOver();
	

private:
	void RegisterAnimName();
	void RegisterInput();
};

