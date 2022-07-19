#include "Player.h"
#include "../GameManager.h"
#include "../input.h"
#include "../Scene/Scene.h"

#include "../Scene/SceneResource.h"

//�浹ü
#include "../Collision/ColliderBox.h"

#include "../UI/WindowSpace.h"

#include "../ScoreManager.h"


CPlayer::CPlayer():
	m_AttackBuffer(),
	m_Bullet(0),
	m_Bomb(10),
	m_FiredRocket(),
	m_WindowSpace()
{
	SetTypeID<CPlayer>();
}

CPlayer::CPlayer(const CPlayer& Obj):
	CCharacter(Obj),
	m_AttackBuffer(),
	m_Bullet(Obj.m_Bullet),
	m_Bomb(Obj.m_Bomb),
	m_FiredRocket(),
	m_WindowSpace(Obj.m_WindowSpace)
{
}

CPlayer::~CPlayer()
{
	CInput::GetInst()->DeleteBindClass<CPlayer>(this);
	m_Scene->DeletePlayer(this);
	CGameManager::GetInst()->DeletePlayer(this);
}  

bool CPlayer::Init(CGameObject* Obj)
{
	CCharacter::Init(Obj);

	//�ø� ����
	SetCullingDelete(ECullingDelete::All, false);

	//Player�� �ּҸ� CGameManager�� ����Ѵ�.
	CGameManager::GetInst()->SetPlayer((CPlayer*)this);
	m_Scene->SetPlayer((CPlayer*)this);


	m_WindowSpace = dynamic_cast<CWindowSpace*>
		(m_Scene->FindWidgetWindow("WindowSpace"));
	if (!m_WindowSpace)
		return false;

	return true;
}

void CPlayer::Update(float DeltaTime)
{
	CCharacter::Update(DeltaTime);
}

void CPlayer::PostUpdate(float DeltaTime)
{
	CCharacter::PostUpdate(DeltaTime);

	CGameManager::GetInst()->DebugTextOut("RocketNum", m_FiredRocket);

	//��Ÿ�� ���
	CheckSkillCooltime((int)EWeapon::MAX, DeltaTime);

}

void CPlayer::Render(HDC hDC, float DeltaTime)
{
	CCharacter::Render(hDC, DeltaTime);

}

int CPlayer::InflictDamage(int Damage)
{
	return CCharacter::InflictDamage(Damage);
}

void CPlayer::DecreaseRocket()
{
	if (this)
		--m_FiredRocket;
}

void CPlayer::ChangeWeapon(EWeapon Weapon)
{
	if (m_Bullet > 999)
		m_Bullet = 999;

	m_WindowSpace->SetBulletLeft(m_Bullet);
}


void CPlayer::FireGun()
{
	++m_AttackBuffer;
}



void CPlayer::GetItem(int Score)
{
	SoundPlayOneChan("MiscItemGet");
	CScoreManager::GetInst()->AddScore(Score);
}



