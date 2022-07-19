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

//���� �׼�
enum class EDirAction
{
	Idle_Pistol,
	Attack_Pistol,
	Idle_Rifle,
	Attack_HMG,
	Attack_Shotgun,
	Attack_Rocket,	//����, ������ �� �پ��ϰ� ����,
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


	//���� �¿� ����� �ൿ�� �����ϴ� ����(���� �Լ��� �̿��ؼ��� ����)
	//���� ������ m_DirTimeMax, m_DitTime�� �ִ��� m_DirTime + m_Tolerance
	float m_DirTime;
	//�̵����� ���⿡ ���� +-DeltaTime�� ���� ����.
	//������Ʈ ������ Input->Update->PostUpdate->Render
	float m_DirTimeMax;
	float m_DirTimeMaxHalf;
	float m_Tolerance; //�ε��Ҽ��� ���� ������ ����(�� ũ��)

	EDirAction m_CurrentAct;
	EDirAction m_PrevAct;
	EDirSpace m_CurrentDir;
	EDirSpace m_PrevDir;
	EWeapon m_CurrentWeapon;
	bool m_NoActionChange; //Ȱ��ȭ �� �ٸ� �׼� �Ұ���

	//����� �׼ǿ� ���� ������ �ִϸ��̼� �̸��� �����ϴ� ���ڿ� �迭
	std::string m_vecAnimName[(int)EDirSpace::MAX][(int)EDirAction::MAX];
	//�ν��� �̸� ����
	std::string m_vecTailBoosterName[(int)EDirSpace::MAX];
	//�¿�(2)�� ���� ���⺰ ���� �ν��� �����°�
	Vector2 m_vecSideBoosterOffset[(int)EDirSpace::MAX][LeftRightEnd];

	INT8 m_HMGDir;
	int	 m_HMGQueue;		//���� �߻� ��
	bool m_GunFired;		//���� �߻����� �� �ൿ�Ұ� Ȯ�� ����
	CooltimeChecker m_HMGDelay;
	class CLaser* m_isLaserFire;	//������ �߻��ߴ��� Ȯ�ο�

	Vector2 m_Offset;
	bool m_BombTurn;


protected:
	//���� �׼��� ����ϴ� �޼ҵ�
	void ComputeCurrentDir(float DeltaTime);
	void ComputeCurrentAct(float DeltaTime); //�׼��� ����ɶ����� ���



private:
	void MoveLeft();
	void MoveRight();

	void MoveUp();
	void MoveDown();
	void LeftKeyDown();
	void RightKeyDown();

	void IdleStatusNotify();

	void FireGun();//���⼱ ���Է� ������ �÷���
	void FireGunCheck();//���⼱ ���Է� ���� ���� �߻�
	void AttackEnd();
	void HMGLaserAtkEnd();

	//���� ���� ���� 
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

