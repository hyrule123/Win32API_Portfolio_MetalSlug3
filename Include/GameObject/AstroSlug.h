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


	bool m_isSeparate; //�и��� �ִϸ��̼����� �ƴ��� Ȯ��
	EPartState m_PartState[2]; //0=��, 1 = ��
	float m_SyncTimer;//�ð��� ����ؼ� ��/���� �ִϸ��̼��� ��ũ��Ŵ
	float m_SyncTimerMax;

	EWeapon m_CurrentWeapon;
	bool m_Busy;
	float m_KnockBack;	//�˹�� Ȱ��ȭ. ���� �Ұ�����

	Vector2 m_SideGunOffset;
	int m_PistolFireAngle;	//1,0,-1�� �Դٰ��� �ϸ鼭 ��Ÿ�� ������ �̹��ϰ� ��ȯ
	int m_FireBuffer; //���Է� ����
	bool m_FireTurn;	//����, �̻��� �� ������ �ִ� ����� ��� �߻��ؾ� �ϴ���
	int m_FireHMG;
	bool m_BombTurn;	//��ź�� ������ ������ �������ϴ���

	float m_ExplodeRemainTime;
	bool m_SelfDestruct;

	//�ν��� ���� �ִϸ��̼� ��������� SFX
	bool m_BoostOn; 
	class CAstroSlugSFX* m_BoostSFXSide;	//�ν��� ���̵� ����(��ü���� ���� ���̾ �׷��� ��)
	class CAstroSlugSFX* m_BoostSFXTail;	//�ν��� ���� ��(��ü���� ���� ���̾�)
	std::string m_BoosterDirSideName[EBoosterDirSideMax];	//�ν��� ���� �̸� ����� (���̵�)
	std::string m_BoosterDirTailName[EBoosterDirTailMax];	//����
	float m_BoostDuration;	//�ν�Ʈ �� �� ���ӽ�ų���� ������ ����

	//�̵� �Է��� �ߴ��� Ȯ��
	bool m_isKeyDown;

	//���� ������ �߻������� Ȯ��. ��� �߻����̸� �������� ����
	//������ ���� �߻簡 off�Ǵ� ���� ������ �� ������ �����ϰ� �ٷ� ���� ����
	CSharedPtr<class CLaser> m_isLaserFire[LeftRightMax];

	//�ƾ� ���� ������
	UINT8 m_IntroCutSceneFlags;
	Vector2 m_CutSceneOffset;	//�ƾ� �߿� ��ġ ������ �ؾ� �ϴ� ���
	float m_CutSceneTimer;

	bool m_isAttack;


private:
	void CheckParts();

	//�̵� ����
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();
	void KeyDownLeft();
	void KeyDownRight();
	void KeyDownUp();
	void KeyDownDown();

	void PlayerTurnLeftToRightEnd();

	//XŰ ����
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

	//ZŰ ����(��ź)
	void FireBomb();

	//cŰ �ν���(����Ű)
	void Boost();
	void BoostEnd();

	
	//����
	void SelfDestruct();

	//����
	void Destroy();	//����
	void Eject();	//�÷��̾� Ż��
	void EjectDoorFrame2Notify();
	void EjectModeDoorEnd();



private:


	void EventCutsceneProcess();
	void KnockBack();
	void RegisterAnimName();
	void RegisterInput();
	void DebugDamage();
	void SelfDestructCollBegin(class CCollider* Src, class CCollider* Dest);

	bool m_isAfterCutscene;	//�ƾ� ���� ������ Ȯ��
	bool m_CutSceneMode;
	bool m_TailBoosterOn;
public:
	void SetIntroCutsceneMode();
	virtual void SetCutsceneMode(bool On);
	void SetCutSceneOffset(const Vector2& Offset);
	void SetTailBooster(bool TailBoosterOn);

};

