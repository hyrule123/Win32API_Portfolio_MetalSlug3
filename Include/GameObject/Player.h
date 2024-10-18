#pragma once
#include "Character.h"

enum class EPlayerSkill
{
	Pistol,
	HMG,
	Shotgun,
	Rocket,
	Laser,
	Bomb,
	End
};

enum class EWeapon
{
	Pistol,
	HMG,
	Shotgun,
	Rocket,
	Laser,
	Bomb,
	MAX
};


class CPlayer :
	public CCharacter
{
	friend class CScene;

protected:
	CPlayer();
	CPlayer(const CPlayer& Obj);
	virtual ~CPlayer();
public:
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual int InflictDamage(int Damage);

	
protected:
	int m_AttackBuffer; //공격 선입력 버퍼
	int m_Bullet;
	int m_Bomb;
	UINT8 m_FiredRocket;	//현재 로켓 발사 수 제한. 최대 2발.
	class CWindowSpace* m_WindowSpace;	//위젯에 남은 총알, 폭탄 수 전달
public:
	void DecreaseRocket();
	virtual void ChangeWeapon(EWeapon Weapon);
	virtual void GetItem(int Score);
	virtual void GetGas() = 0;
	virtual void SetCutsceneMode(bool isOn) = 0;

protected:
	virtual void MoveLeft() = 0;
	virtual void MoveRight() = 0;
	virtual void MoveUp() = 0;
	virtual void MoveDown() = 0;

	virtual void FireGun();
	virtual void FireBomb() = 0;
	virtual void FireNotify() = 0;
	virtual void AttackEnd() = 0;

};

