#include "PlayerGround.h"
#include "../GameManager.h"
#include "../input.h"
#include "../Scene/Scene.h"

#include "../Scene/SceneResource.h"

//충돌체
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

	//Player의 주소를 CGameManager에 등록한다.
	CGameManager::GetInst()->SetPlayer(this);


	//위치 설정
	SetPos(10.f, 10.f);
	SetSize(100.f, 170.f);
	SetDir(1.f, 0.f);
	SetPivot(0.0f, 1.f);
	SetScale(1.f);


	m_MaxSpeed = 200.f;

	m_isAttacking = false;


	//입력 초기화
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




	//스킬 설정
	m_vecCooltime.resize((size_t)EWeapon::MAX);
	m_vecCooltime[(int)EWeapon::Pistol].Cooltime = 0.5f;




	//아우렐리온 솔 w스킬 호출
	//m_Sol = m_Scene->CreateObject<CSolBullet>("CSolBullet", this);




	///////////////////// 애니메이션 생성 ///////////////////
	//CreateAnimation();

	////애니메이션 이름, 폴더 경로, 프레임 경로를 전달하면 프레임을 가진 CTexture이 만들어진다.
	//m_Scene->GetSceneResource()->LoadTextureByTextureNumbers("IdleRight", TEXT("IdleRight/"), 3);

	////이름을 정하고 파일 이름이 들어있는 배열을 전달한다.
	//m_Scene->GetSceneResource()->CreateAnimationSequence("IdleRight", "IdleRight");

	////프레임 형식의 애니메이션일 경우 AddAnimationFrameFrame 메소드를 통해 각 프레임 정보를 등록한다.
	//m_Scene->GetSceneResource()->AddAnimationFrameFrame("IdleRight");

	////컬러키를 지정해준다.(이름만 넣으면 기본값인 true(사용) + 마젠타 색상으로 등록된다.
	//m_Scene->GetSceneResource()->SetColorKey("IdleRight");

	////만든 애니메이션을 CGameObject의 m_Animation에 사용 등록한다.
	//AddAnimationInfo("IdleRight", "IdleRight");

	////좌우 애니메이션 구분용 배열에 삽입한다.
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



	//충돌체 설정
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
		//움직였을 경우 공격 중이어도 공격을 취소한다.
		m_isAttacking = false;

		//왼쪽/오른쪽 구분 후 1번 애니메이션(달리기)를 호출해준다.
		//ChangeAnimation(m_vecDirection[EDirection][1]);


	}
	//공격 중도 아니고 이동 중도 아니면 Idle
	else if (!m_isAttacking)
	{
		//왼쪽 오른쪽 구분 후 0번 애니메이션(아이들)을 호출해준다.
		//ChangeAnimation(m_vecDirection[m_isRightDir][0]);
	}


	//쿨타임 계산
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
	//쿨타임이면 그냥 return
	if (m_vecCooltime[(int)EWeapon::Pistol].isCooltime)
		return;

	//쿨타임이 아니면 쿨타임으로 바꾸는 메소드를 호출하고
	EnterSkillCoolTIme((int)EWeapon::Pistol);

	m_isAttacking = true;

	//공격 애니메이션 호출!(이후 총알 발사는 FireNotify에서 진행)
	//ChangeAnimation(m_vecDirection[m_isRightDir][2]);
}

void CPlayerGround::FireNotify()
{
	//스킬 로직 진행
	//CProjectile* Bullet = m_Scene->CreateObject<CProjectile>("PlayerBullet", this);
	//Bullet->SetMaxVelocityDir(1000.f, m_Dir);
	//Bullet->SetPos(m_Pos);
	//Bullet->SetDamage(20);
}

void CPlayerGround::AttackEnd()
{
	//공격 끝났으니까 false로 바꾸고 idle 모션으로 복귀
	m_isAttacking = false;
	//ChangeAnimation(m_vecDirection[m_isRightDir][0]);
}
