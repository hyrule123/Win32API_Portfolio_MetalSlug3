#include "PlayerGround.h"
#include "../GameManager.h"
#include "../input.h"
#include "../Scene/Scene.h"

#include "../Scene/SceneResource.h"

//�浹ü
#include "../Collision/ColliderBox.h"


CPlayerGround::CPlayerGround() :
	m_isAttacking(false)
{
	SetTypeID<CPlayerGround>();
}

CPlayerGround::CPlayerGround(const CPlayerGround& Obj) :
	CPlayer(Obj)
{
}

CPlayerGround::~CPlayerGround()
{
	CInput::GetInst()->DeleteBindClass<CPlayerGround>(this);

}

bool CPlayerGround::LoadResource()
{
	return true;
}

bool CPlayerGround::Init(CGameObject* Obj)
{
	CCharacter::Init(Obj);

	//Player�� �ּҸ� CGameManager�� ����Ѵ�.
	CGameManager::GetInst()->SetPlayer(this);


	//��ġ ����
	SetPos(10.f, 10.f);
	SetSize(100.f, 170.f);
	SetDir(1.f, 0.f);
	SetPivot(0.0f, 1.f);
	SetScale(1.f);


	m_MaxSpeed = 200.f;

	m_isAttacking = false;


	//�Է� �ʱ�ȭ
	CInput::GetInst()->AddBindFunction<CPlayerGround>("MoveLeft", EInput_Type::Push,
		this, &CPlayerGround::MoveLeft);

	CInput::GetInst()->AddBindFunction<CPlayerGround>("MoveRight", EInput_Type::Push,
		this, &CPlayerGround::MoveRight);

	CInput::GetInst()->AddBindFunction<CPlayerGround>("MoveUp", EInput_Type::Push,
		this, &CPlayerGround::MoveUp);

	CInput::GetInst()->AddBindFunction<CPlayerGround>("MoveDown", EInput_Type::Push,
		this, &CPlayerGround::MoveDown);

	CInput::GetInst()->AddBindFunction<CPlayerGround>("GunFire", EInput_Type::Push,
		this, &CPlayerGround::GunFire);

	CInput::GetInst()->AddBindFunction<CPlayerGround>("Jump", EInput_Type::Down,
		this, &CPlayerGround::Jump);




	//��ų ����
	m_vecCooltime.resize((size_t)EWeapon::MAX);
	m_vecCooltime[(int)EWeapon::Pistol].Cooltime = 0.5f;




	//�ƿ췼���� �� w��ų ȣ��
	//m_Sol = m_Scene->CreateObject<CSolBullet>("CSolBullet", this);




	///////////////////// �ִϸ��̼� ���� ///////////////////
	//CreateAnimation();

	////�ִϸ��̼� �̸�, ���� ���, ������ ��θ� �����ϸ� �������� ���� CTexture�� ���������.
	//m_Scene->GetSceneResource()->LoadTextureByTextureNumbers("IdleRight", TEXT("IdleRight/"), 3);

	////�̸��� ���ϰ� ���� �̸��� ����ִ� �迭�� �����Ѵ�.
	//m_Scene->GetSceneResource()->CreateAnimationSequence("IdleRight", "IdleRight");

	////������ ������ �ִϸ��̼��� ��� AddAnimationFrameFrame �޼ҵ带 ���� �� ������ ������ ����Ѵ�.
	//m_Scene->GetSceneResource()->AddAnimationFrameFrame("IdleRight");

	////�÷�Ű�� �������ش�.(�̸��� ������ �⺻���� true(���) + ����Ÿ �������� ��ϵȴ�.
	//m_Scene->GetSceneResource()->SetColorKey("IdleRight");

	////���� �ִϸ��̼��� CGameObject�� m_Animation�� ��� ����Ѵ�.
	//AddAnimationInfo("IdleRight", "IdleRight");

	////�¿� �ִϸ��̼� ���п� �迭�� �����Ѵ�.
	////vecDirectionPushBack(EDirection::Right, "IdleRight");
	////////

	////IdleLeft
	//m_Scene->GetSceneResource()->LoadTextureByTextureNumbers("IdleLeft", TEXT("IdleLeft/"), 3);
	//m_Scene->GetSceneResource()->CreateAnimationSequence("IdleLeft", "IdleLeft");
	//m_Scene->GetSceneResource()->AddAnimationFrameFrame("IdleLeft");
	//m_Scene->GetSceneResource()->SetColorKey("IdleLeft");
	//AddAnimationInfo("IdleLeft", "IdleLeft");
	////vecDirectionPushBack(EDirection::Left, "IdleLeft");



	////WalkRight
	//m_Scene->GetSceneResource()->LoadTextureByTextureNumbers("WalkRight", TEXT("WalkRight/"), 9);
	//m_Scene->GetSceneResource()->CreateAnimationSequence("WalkRight", "WalkRight");
	//m_Scene->GetSceneResource()->AddAnimationFrameFrame("WalkRight");
	//m_Scene->GetSceneResource()->SetColorKey("WalkRight");
	//AddAnimationInfo("WalkRight", "WalkRight");
	////vecDirectionPushBack(EDirection::Right, "WalkRight");

	////WalkLeft
	//m_Scene->GetSceneResource()->LoadTextureByTextureNumbers("WalkLeft", TEXT("WalkLeft/"), 9);
	//m_Scene->GetSceneResource()->CreateAnimationSequence("WalkLeft", "WalkLeft");
	//m_Scene->GetSceneResource()->AddAnimationFrameFrame("WalkLeft");
	//m_Scene->GetSceneResource()->SetColorKey("WalkLeft");
	//AddAnimationInfo("WalkLeft", "WalkLeft");
	////vecDirectionPushBack(EDirection::Left, "WalkLeft");



	////AttackRight
	//m_Scene->GetSceneResource()->LoadTextureByTextureNumbers("AttackRight", TEXT("AttackRight/"), 4);
	//m_Scene->GetSceneResource()->CreateAnimationSequence("AttackRight", "AttackRight");
	//m_Scene->GetSceneResource()->AddAnimationFrameFrame("AttackRight");
	//m_Scene->GetSceneResource()->SetColorKey("AttackRight");
	//AddAnimationInfo("AttackRight", "AttackRight");
	////vecDirectionPushBack(EDirection::Right, "AttackRight");
	//AddNotify("AttackRight", 1, this, &CPlayerGround::FireNotify);
	//SetEndFunction("AttackRight", this, &CPlayerGround::AttackEnd);
	//SetPlayTime("AttackRight", 0.2f);

	////AttackLeft
	//m_Scene->GetSceneResource()->LoadTextureByTextureNumbers("AttackLeft", TEXT("AttackLeft/"), 4);
	//m_Scene->GetSceneResource()->CreateAnimationSequence("AttackLeft", "AttackLeft");
	//m_Scene->GetSceneResource()->AddAnimationFrameFrame("AttackLeft");
	//m_Scene->GetSceneResource()->SetColorKey("AttackLeft");
	//AddAnimationInfo("AttackLeft", "AttackLeft");
	////vecDirectionPushBack(EDirection::Left, "AttackLeft");
	//AddNotify("AttackLeft", 1, this, &CPlayerGround::FireNotify);
	//SetEndFunction("AttackLeft", this, &CPlayerGround::AttackEnd);
	//SetPlayTime("AttackLeft", 0.2f);



	//�浹ü ����
	m_Collider = AddCollider<CColliderBox>("PlayerBox");
	m_Collider->SetCollisionProfile(ECollisionChannel::Player);



	return true;
}

void CPlayerGround::Update(float DeltaTime)
{
	CCharacter::Update(DeltaTime);
}

void CPlayerGround::PostUpdate(float DeltaTime)
{
	CCharacter::PostUpdate(DeltaTime);

	m_Collider->SetSize(m_Size * m_Scale);
	m_Collider->SetPivot(m_Pivot);



	if (m_MoveDist.x != 0.f || m_MoveDist.y != 0.f)
	{
		//�������� ��� ���� ���̾ ������ ����Ѵ�.
		m_isAttacking = false;

		//����/������ ���� �� 1�� �ִϸ��̼�(�޸���)�� ȣ�����ش�.
		//ChangeAnimation(m_vecDirection[EDirection][1]);


	}
	//���� �ߵ� �ƴϰ� �̵� �ߵ� �ƴϸ� Idle
	else if (!m_isAttacking)
	{
		//���� ������ ���� �� 0�� �ִϸ��̼�(���̵�)�� ȣ�����ش�.
		//ChangeAnimation(m_vecDirection[m_isRightDir][0]);
	}


	//��Ÿ�� ���
	CheckSkillCooltime((int)EPlayerSkill::End, DeltaTime);

}

void CPlayerGround::Render(HDC hDC, float DeltaTime)
{
	CCharacter::Render(hDC, DeltaTime);




	//char Text[64] = {};
	//sprintf_s(Text, "Number Of Slave Array: %d", (int)m_SlaveObject.size());
	//TextOutA(hDC, 1, 30, Text, (int)strlen(Text));
}

int CPlayerGround::InflictDamage(int Damage)
{
	CCharacter::InflictDamage(Damage);

	return Damage;
}




void CPlayerGround::MoveLeft()
{
	//m_isRightDir = false;
	SetPivot(1.f, 1.f);
	SetDir(-1.f, 0.f);
	MoveDir(m_Dir);
}

void CPlayerGround::MoveRight()
{
	//m_isRightDir = true;
	SetPivot(0.f, 1.f);
	SetDir(1.f, 0.f);
	MoveDir(m_Dir);
}

void CPlayerGround::MoveUp()
{
	SetDir(0.f, -1.f);
	MoveDir(m_Dir);
}

void CPlayerGround::MoveDown()
{
	SetDir(0.f, 1.f);
	MoveDir(m_Dir);
}

void CPlayerGround::GunFire()
{
	//��Ÿ���̸� �׳� return
	if (m_vecCooltime[(int)EWeapon::Pistol].isCooltime)
		return;

	//��Ÿ���� �ƴϸ� ��Ÿ������ �ٲٴ� �޼ҵ带 ȣ���ϰ�
	EnterSkillCoolTIme((int)EWeapon::Pistol);

	m_isAttacking = true;

	//���� �ִϸ��̼� ȣ��!(���� �Ѿ� �߻�� FireNotify���� ����)
	//ChangeAnimation(m_vecDirection[m_isRightDir][2]);
}

void CPlayerGround::FireNotify()
{
	//��ų ���� ����
	//CProjectile* Bullet = m_Scene->CreateObject<CProjectile>("PlayerBullet", this);
	//Bullet->SetMaxVelocityDir(1000.f, m_Dir);
	//Bullet->SetPos(m_Pos);
	//Bullet->SetDamage(20);
}

void CPlayerGround::AttackEnd()
{
	//���� �������ϱ� false�� �ٲٰ� idle ������� ����
	m_isAttacking = false;
	//ChangeAnimation(m_vecDirection[m_isRightDir][0]);
}
