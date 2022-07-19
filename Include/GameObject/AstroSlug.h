#pragma once
#include "Player.h"

enum class EPartState
{
	Normal,
	NormalRifle,
	GunGet,
	GunDrop
};

enum class ELayerInfo
{
	BackSFX,
	LeftorOne,
	Right,
	Door,
	PlayerWindow,
	FrontSFX
};

enum EBoosterDirSide
{
	SmallBoosterNE = 0,
	SmallBoosterE,
	SmallBoosterSE,
	SmallBoosterSW,
	SmallBoosterW,
	SmallBoosterNW,
	EBoosterDirSideMax
};

enum EBoosterDirTail
{
	TailBoost1 = 0,
	TailBoost2,
	TailSFX,
	TailLeft,
	TailRight,
	TailStraight,
	TailIdle,
	EBoosterDirTailMax
};

enum LeftRight
{
	Left = 0,
	Right,
	LeftRightMax
};

enum class EIntroCutSceneFlags : UINT8
{
	IntroCutSceneOn = 0b10000000,
	Action1 = 0b00000001,
	Action2 = 0b00000010,
	Action3 = 0b00000100,
	Action4 = 0b00001000,
	Action5 = 0b00010000,
	Action6 = 0b00100000
};

class CAstroSlug :
	public CPlayer
{
	friend class CScene;

protected:
	CAstroSlug();
	CAstroSlug(const CAstroSlug& Obj);
	virtual ~CAstroSlug();
public:
	virtual bool LoadResource();
	virtual void Start();
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual int InflictDamage(int Damage);
	virtual void GetGas();


	bool m_isSeparate; //분리된 애니메이션인지 아닌지 확인
	EPartState m_PartState[2]; //0=좌, 1 = 우
	float m_SyncTimer;//시간을 계산해서 좌/우측 애니메이션을 싱크시킴
	float m_SyncTimerMax;

	EWeapon m_CurrentWeapon;
	bool m_Busy;
	float m_KnockBack;	//넉백시 활성화. 조작 불가능함

	Vector2 m_SideGunOffset;
	int m_PistolFireAngle;	//1,0,-1을 왔다갔다 하면서 평타의 각도를 미묘하게 변환
	int m_FireBuffer; //선입력 버퍼
	bool m_FireTurn;	//샷건, 미사일 등 순서가 있는 무기들 어디서 발사해야 하는지
	int m_FireHMG;
	bool m_BombTurn;	//폭탄의 순서가 어디부터 나가야하는지

	float m_ExplodeRemainTime;
	bool m_SelfDestruct;

	//부스터 관련 애니메이션 재생시켜줄 SFX
	bool m_BoostOn; 
	class CAstroSlugSFX* m_BoostSFXSide;	//부스터 사이드 측면(본체보다 높은 레이어에 그려야 함)
	class CAstroSlugSFX* m_BoostSFXTail;	//부스터 꼬리 면(본체보다 낮은 레이어)
	std::string m_BoosterDirSideName[EBoosterDirSideMax];	//부스터 방향 이름 저장용 (사이드)
	std::string m_BoosterDirTailName[EBoosterDirTailMax];	//꼬리
	float m_BoostDuration;	//부스트 몇 초 지속시킬건지 사전에 지정

	//이동 입력을 했는지 확인
	bool m_isKeyDown;

	//현재 레이저 발사중인지 확인. 계속 발사중이면 레이저에 전달
	//레이저 지속 발사가 off되는 순간 레이저 이 변수에 전달하고 바로 연결 해제
	CSharedPtr<class CLaser> m_isLaserFire[LeftRightMax];

	//컷씬 관련 변수들
	UINT8 m_IntroCutSceneFlags;
	Vector2 m_CutSceneOffset;	//컷씬 중에 위치 보정을 해야 하는 경우
	float m_CutSceneTimer;

	bool m_isAttack;


private:
	void CheckParts();

	//이동 관련
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();
	void KeyDownLeft();
	void KeyDownRight();
	void KeyDownUp();
	void KeyDownDown();

	void PlayerTurnLeftToRightEnd();

	//X키 공격
	void FireGunBuffer();
	void FireGun();
	void FireHMG();
	void FireShotgun(bool isRight);
	void FireRocket(bool isRight);
	void FireLaser(bool isRight);
	void AttackEnd();
	void RifleGet();
	void RifleDrop();
	void ChangeRifleEnd();
	void FireNotify();

	virtual void ChangeWeapon(EWeapon Weapon);
	void ChangeHMG();
	void ChangeShotgun();
	void ChangeRocketLauncher();
	void ChangeLaser();

	//Z키 공격(폭탄)
	void FireBomb();

	//c키 부스터(점프키)
	void Boost();
	void BoostEnd();

	
	//자폭
	void SelfDestruct();

	//폭발
	void Destroy();	//폭발
	void Eject();	//플레이어 탈출
	void EjectDoorFrame2Notify();
	void EjectModeDoorEnd();



private:


	void EventCutsceneProcess();
	void KnockBack();
	void RegisterAnimName();
	void RegisterInput();
	void DebugDamage();
	void SelfDestructCollBegin(class CCollider* Src, class CCollider* Dest);

	bool m_isAfterCutscene;	//컷씬 시작 전인지 확인
	bool m_CutSceneMode;
	bool m_TailBoosterOn;
public:
	void SetIntroCutsceneMode();
	virtual void SetCutsceneMode(bool On);
	void SetCutSceneOffset(const Vector2& Offset);
	void SetTailBooster(bool TailBoosterOn);

};

