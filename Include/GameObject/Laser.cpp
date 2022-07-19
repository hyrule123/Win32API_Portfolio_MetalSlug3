#include "Laser.h"
#include "LaserDebris.h"

#include "../GameManager.h"
#include "../Collision/Collider.h"
#include "../Collision/ColliderLine.h"

#include "../Scene/Camera.h"
#include "../Scene/SceneManager.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Texture/Texture.h"

//�����ѹ� �޾ƿ�������
#include "../GameManager.h"

#include "../ScoreManager.h"


CLaser::CLaser() :
	m_ColorNum{},
	m_Index(),
	m_KeepFireRefVal(0.3f),
	m_Coll(),
	m_CollidingObj(),
	m_CollidingObjYPos(),
	m_ChangeIndex(),
	m_DamageFreq(),
	m_TurnOff(true)
{
	SetTypeID<CLaser>();

	m_KeepFire = m_KeepFireRefVal;
	m_DamageFreqRefVal = m_KeepFireRefVal / 6.f;//�������� 6ȸ(�ܻ����)
}

CLaser::CLaser(const CLaser& Obj) :
	CProjectile(Obj),
	m_ColorNum{},	//Init���� �ʱ�ȭ
	m_Index(Obj.m_Index),
	m_KeepFire(Obj.m_KeepFire),
	m_KeepFireRefVal(Obj.m_KeepFireRefVal),
	m_Coll(Obj.m_Coll),
	m_CollidingObj(Obj.m_CollidingObj),
	m_CollidingObjYPos(Obj.m_CollidingObjYPos),
	m_ChangeIndex(Obj.m_ChangeIndex),
	m_DamageFreq(Obj.m_DamageFreq),
	m_TurnOff(Obj.m_TurnOff)
{
	m_KeepFire = m_KeepFireRefVal;
	m_DamageFreqRefVal = m_KeepFireRefVal / 6.f;//�������� 6ȸ(�ܻ����)
}

CLaser::~CLaser()
{
}

bool CLaser::Init(CGameObject* Obj)
{
	CGameObject::Init(Obj);

	m_Damage = 8;

	m_RenderLayer = ERenderLayer::EffectLow;

	m_Coll = AddCollider<CColliderLine>("Laser");
	m_Coll->SetCollisionProfile(ECollisionChannel::PlayerAttack);
	m_Coll->SetDir(270.f, (float)abs(ORIGINAL_GAME_RES_HEIGHT - m_Pos.y));
	m_Coll->SetCollisionBeginFunc<CLaser>(this, &CLaser::CollisionBegin);
	m_Coll->SetCollisionEndFunc<CLaser>(this, &CLaser::CollisionEnd);

	m_ColorNum[0] = (UINT8)EReactionChannel::Normal;
	m_ColorNum[1] = (UINT8)EReactionChannel::Laser1;
	m_ColorNum[2] = (UINT8)EReactionChannel::Laser2;
	m_ColorNum[3] = (UINT8)EReactionChannel::Laser3;

	//������ �߻� ���� ������� ����
	SetEndFunction<CLaser>("LaserLineOff", this, &CLaser::LaserLineOffEnd);

	//���� ���� ��Ƽ���� ����
	AddNotify<CLaser>("LaserHit", 0, this, &CLaser::LaserHitNotify);
	AddNotify<CLaser>("LaserHitOff", 0, this, &CLaser::LaserHitNotify);

	

	ChangeAnimation("LaserLine");
	ChangeAnimation("LaserFire");

	SoundStop("LaserFire");
	SoundPlayOneChan("LaserFire");
	//m_Scene->GetSceneResource()->SoundResume("LaserFire");
	return true;
}

void CLaser::Start()
{
	m_Start = true;

	m_PrevPos = m_Pos;

	
}

void CLaser::Update(float DeltaTime)
{
	if (!m_Start)
		Start();

	CGameObject::Update(DeltaTime);



	//������ ���̸� �� ������ â �������� �����Ѵ�
	m_Coll->SetDistance(fabs(m_Pos.y - m_Scene->GetCamPos().y - 5.f));

	if (m_KeepFire > 0.f)
		m_KeepFire -= DeltaTime;
	else
	{
		ChangeAnimation("LaserLineOff");
		ChangeAnimation("LaserOff");
		SoundFadeOut("LaserFire", 0.19f);
	}

	if (m_MasterObject)
	{
		m_Pos = m_MasterObject->GetPos() + m_Offset;
	}


	//�ε����� ����� �ִٸ� ��Ʈ����Ʈ�� ��Ʈ����Ʈ�� �����Ѵ�. �׷��� ���� ��� �ִϸ��̼� ����

	if (m_Coll->CheckCollidingAny())
	{
		auto CollisionList = m_Coll->GetCollisionList();
		if (CollisionList->size() > 1)
			CollisionList->sort(SortCollY);

		m_CollidingObj = CollisionList->begin()->Get()->GetOwnerObj();


		if (m_KeepFire > 0.f)
		{
			m_CollidingObjYPos = m_Coll->GetHitPoint().y;
			ChangeAnimation("LaserHit");
			SetAnimOffset("LaserHit", 0.f, -1.f * (m_Pos.y - m_CollidingObjYPos) + 20.f);
		}
		else
		{
			m_CollidingObjYPos = m_Coll->GetHitPoint().y;
			ChangeAnimation("LaserHitOff");
			SetAnimOffset("LaserHitOff", 0.f, -1.f * (m_Pos.y - m_CollidingObjYPos) + 15.f);
		}

		m_DamageFreq -= DeltaTime;
		if (m_DamageFreq <= 0.f)
		{
			m_DamageFreq = m_DamageFreqRefVal;

			CScoreManager::GetInst()->AddScore(100);
			m_CollidingObj->InflictDamage(m_Damage);
		}
	}
	else
	{
		//��Ʈ����Ʈ���� �߻��ϴ� �ִϸ��̼��� ����� �����Ѵ�.
		StopAnimation(2);
	}

}

void CLaser::PostUpdate(float DeltaTime)
{
	if (!m_Start)
		Start();


	CGameObject::PostUpdate(DeltaTime);
}




void CLaser::SetEssential(const Vector2& Dir, const Vector2& Pos, const Vector2& Offset)
{
	m_isReady = true;

	m_Dir = Dir;
	m_Pos = Pos;
	
	m_Offset = Offset;

	if (m_MasterObject)
		m_Pos = m_MasterObject->GetPos() + m_Offset;

}

void CLaser::SetEssential(float DirX, float DirY, float PosX, float PosY, float OffsetX, float OffsetY)
{
	m_isReady = true;

	m_Dir.x = DirX;
	m_Dir.y = DirY;
	m_Pos.x = PosX;
	m_Pos.y = PosY;
	m_Offset.x = OffsetX;
	m_Offset.y = OffsetY;

	if (m_MasterObject)
		m_Pos = m_MasterObject->GetPos() + m_Offset;

}

void CLaser::CollisionBegin(CCollider* Src, CCollider* Dest)
{

	m_CollidingObj = Dest->GetOwnerObj();
	m_CollidingObjYPos = Src->GetHitPoint().y;

}

void CLaser::CollisionEnd(CCollider* Src, CCollider* Dest)
{
	if (Dest->GetOwnerObj() == m_CollidingObj)
	{
		m_CollidingObj = nullptr;
	}
}

bool CLaser::SortCollY(const CSharedPtr<class CCollider>& Src, const CSharedPtr<class CCollider>& Dest)
{
	return (*Src)->GetBottom() > (*Dest)->GetBottom();
}


void CLaser::Render(HDC hDC, float DeltaTime)
{
	if (!m_Start)
		Start();

		Vector2 Res;
		Vector2	Pos;
		CCamera* Camera = nullptr;

		if (m_Scene)
		{
			Camera = m_Scene->GetCamera();
			Res = Camera->GetRes();
			Pos = m_Pos - Camera->GetPos();
		}
		else
		{
			Camera = CSceneManager::GetInst()->GetScene()->GetCamera();
			Res = Camera->GetRes();
			Pos = m_Pos - Camera->GetPos();
		}

		//�ִϸ��̼��� �ִٸ� �ִϸ��̼��� �켱
		if (m_Animation)
		{
			for (int i = 0; i <= m_Animation->GetHighestLayer(); ++i)
			{
				if (!m_Animation->m_CurrentAnimation[i])
					continue;

				CAnimationInfo* Current = m_Animation->m_CurrentAnimation[i];
				int CurrentFrame = Current->m_Frame;
				const Sprite_FrameData& FrameData = Current->m_Sequence->GetFrameData(CurrentFrame);

				Vector2 RenderSize;
				Vector2 RenderLT;
				if (Current->m_Sequence->GetName() == "LaserLine" ||
					Current->m_Sequence->GetName() == "LaserLineOff")
				{
					RenderSize.x = 2;

					if (m_CollidingObj)
					{
						RenderSize.y = fabs(m_CollidingObjYPos - m_Pos.y) - 17.f;
					}
					else
					{
						RenderSize.y = ORIGINAL_GAME_RES_HEIGHT;
					}
					RenderLT.y -= 1.f;
				}
				else
				{
					RenderSize = FrameData.End - FrameData.Start;
				}
				RenderLT += Pos - (RenderSize * Current->m_Pivot * m_Scale).Round() + Current->m_Offset + FrameData.Offset;
				


				CTexture* Tex = Current->m_Sequence->GetTexture();


				TransparentBlt(hDC,
					(int)RenderLT.x, (int)RenderLT.y,
					(int)RenderSize.x, (int)RenderSize.y,
					Tex->GetDC(m_ColorNum[m_Index]),
					(int)FrameData.Start.x, (int)FrameData.Start.y,
					(int)(FrameData.End.x - FrameData.Start.x), (int)(FrameData.End.y - FrameData.Start.y),
					Tex->GetColorKey(m_ColorNum[m_Index]));


			}


			//2�����Ӹ��� �ѹ��� ������ ������ �ٲ��ش�.
			if(m_ChangeIndex)
				m_Index = CGameManager::GetInst()->GetRandNum() % 4;
			m_ChangeIndex = !m_ChangeIndex;

		}


		if (DEBUG_RENDER)
		{

			//�浹ü ����Ʈ�� ����ִٸ� ���ص� ��
			if (!m_listCollider.empty())
			{
				auto iter = m_listCollider.begin();
				auto iterEnd = m_listCollider.end();

				while (iter != iterEnd)
				{
					//���� �浹ü�� ���ŵǱ�� ���� �Ǿ��־��ٸ�
					if (!(*iter)->GetActive())
					{
						iter++;
						continue;
					}

					//���� �ӽ÷� ��Ȱ��ȭ�� �Ǿ��ִٸ� ó������ �ʰ� �Ѿ��.
					else if (!(*iter)->GetEnable())
					{
						iter++;
						continue;
					}

					(*iter)->Render(hDC, DeltaTime);

					++iter;
				}
			}
		}
}

bool CLaser::LoadResource()
{
	if (!LoadCSVPreset(TEXT("SFX/Laser.csv")))
		return false;


	return true;
}

void CLaser::SetKeepFire(bool KeepFire)
{
	if (KeepFire)
	{
		m_KeepFire = m_KeepFireRefVal;
	}
}

float CLaser::GetKeepFire() const
{
	return m_KeepFire;
}



void CLaser::LaserLineOffEnd()
{
	m_Scene->GetSceneResource()->SoundStop("LaserFire");
	SetActive(false);
}

void CLaser::LaserHitNotify()
{
	int rnd = rand();
	float RandDir = rnd % 2 == 0 ? -1.f : 1.f;	//������ ���� ����

	CLaserDebris* Debris = m_Scene->CreateObject<CLaserDebris>("LaserDebris");
	Debris->SetEssential(RandDir, 0.f, m_Pos.x, m_Pos.y - m_CollidingObjYPos, 0.f, 0.f, float(rnd % 100));
}
