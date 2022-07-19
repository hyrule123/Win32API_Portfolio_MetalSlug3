#include "DaiManji.h"

#include "DaiManji.h"

#include "../Scene/MainScene.h"
#include "../Scene/SceneManager.h"
#include "../GameManager.h"
#include "Player.h"

//�߻�ü
#include "ProjectileEnemy.h"

//���ҽ� �ε�
#include "../Scene/SceneResource.h"
#include "../Resource/Texture/Texture.h"


//����Ʈ
#include "Effect.h"
#include "Explosion.h"

//�浹ü
#include "../Collision/ColliderBox.h"
#include "../Collision/ColliderCircle.h"

//���� ����
#include "MiniUFO.h"

//��ġ ��� ī�޶�
#include "../Scene/Camera.h"

#include "../UI/WidgetComponent.h"


CDaiManji::CDaiManji() :
	m_Timer(),
	m_isBoss(),
	m_RotatingAngle(),
	m_RenderPos(),
	m_RotatingRadius(),
	m_Coll(), 
	m_UFOSpawnQueue(),
	m_MainScene(),
	m_UFOSpawnMAX(4),
	m_DeathTimer(2.f)
{
	SetTypeID<CDaiManji>();
}

CDaiManji::CDaiManji(const CDaiManji& Obj) :
	CEnemy(Obj),
	m_Timer(),
	m_isBoss(Obj.m_isBoss),
	m_RotatingAngle(Obj.m_RotatingAngle),
	m_RenderPos(Obj.m_RenderPos),
	m_RotatingRadius(Obj.m_RotatingRadius),
	m_Coll(Obj.m_Coll),
	m_UFOSpawnQueue(Obj.m_UFOSpawnQueue),
	m_MainScene(Obj.m_MainScene),
	m_UFOSpawnMAX(Obj.m_UFOSpawnMAX),
	m_DeathTimer(Obj.m_DeathTimer)
{
}

CDaiManji::~CDaiManji()
{

}

bool CDaiManji::LoadResource()
{
	CreateAnimation();

	if (!LoadCSVPreset(TEXT("Enemy/DaiManji.csv")))
		return false;

	return true;
}

bool CDaiManji::Init(CGameObject* Obj)
{
	CEnemy::Init(Obj);

	SetRenderLayer(ERenderLayer::Default);

	SetCullingDelete(ECullingDelete::All, false);
	SetCullingDelete(ECullingDelete::CullingOn, false);

	SetAnimation("DaiManjiNormal");
	SetAnimation("DaiManjiHatchClosed");

	m_HP = 700;
	m_MaxSpeed = 300.f;
	SetDeAccel(0.8f);
	m_Size.Set(80.f, 80.f);

	m_Coll = AddCollider<CColliderCircle>("DaiManji");
	m_Coll->SetCollisionProfile(ECollisionChannel::Enemy);
	m_Coll->SetRadius(50.f);
	m_Coll->SetOffset(0.f, -60.f);

	//CColliderCircle* Coll = AddCollider<CColliderCircle>("DaiManji");
	//Coll->SetRadius(50.f);

	//������ x���� ȭ�� �ۿ��� ����
	m_Pos.x = (float)(rand() % ORIGINAL_GAME_RES_WIDTH);
	m_Pos.y = -50.f;
	MoveToDest(true, EMoveToMethod::AddForce, Vector2(m_Pos.x, 80.f));

	//ȸ���ݰ� ����
	m_RotatingRadius = 12.f;


	//EndFunction ���.
	SetEndFunction<CDaiManji>("NormalUFOSpawn", this, &CDaiManji::CreateUFO);


	//���� ���� �ּҸ� �̸� ���̳��� ĳ������ ���� �޾Ƴ��´�.
	m_MainScene = dynamic_cast<CMainScene*>(m_Scene);
	if (!m_MainScene)
		return false;

	//����� ��������Ʈ �ð� ������ Ÿ�̸�
	m_ExplosionTimer.InitCooltime(0.1f);


	m_isKey = true;
	return true;
}


void CDaiManji::Update(float DeltaTime)
{
	CEnemy::Update(DeltaTime);


	//������ 360���� �Ѿ��, 360���� ���༭ ���ڰ� �ʹ� Ŀ���� �ʵ��� ���ش�.
	if (m_HP > 0)
	{
		m_RotatingAngle += 360.f * DeltaTime;
		if (m_RotatingAngle >= 1440.f)
			m_RotatingAngle -= 1440.f;

		//���Ȱ��� ���Ѵ�.
		float RotAngleRad = DegreeToRadian(m_RotatingAngle);

		//���Ȱ��� ���� �߽����� ���س���.
		m_RenderPos.x = m_Pos.x + (cosf(RotAngleRad) * m_RotatingRadius);
		m_RenderPos.y = m_Pos.y + (sinf(RotAngleRad) * m_RotatingRadius);
		if (m_Coll)
			m_Coll->SetFixedPos(m_RenderPos);

		Routine(DeltaTime);
	}
	else
	{
		m_ExplosionTimer.UpdateCooltime(DeltaTime);
		if (m_ExplosionTimer.EnterCooltime())
		{
			Vector2 LT = m_Pos - (m_Size * m_Pivot);
			LT.x += (float)(rand() % (int)m_Size.x);
			LT.y += (float)(rand() % (int)m_Size.y);

			SoundPlay("SmallExplosion");
			CExplosion* Explosion = m_Scene->CreateObject<CExplosion>("DaiManjiExplosion");
			Explosion->SetEssential(EExplosionSize::Small, LT);
		}

		m_DeathTimer -= DeltaTime;
		if (m_DeathTimer < 0.f)
		{
			SoundPlay("BigExplosion");

			CExplosion* Explosion = m_Scene->CreateObject<CExplosion>("DaiManjiExplosion");
			Explosion->SetEssential(EExplosionSize::Big, m_Pos);

			ItemDrop();
			SetActive(false);
		}

	}
}

void CDaiManji::PostUpdate(float DeltaTime)
{
	CEnemy::PostUpdate(DeltaTime);

	////ü�� Ȯ��
	//if (m_HP <= 0.f)
	//	SetActive(false);
}

void CDaiManji::Render(HDC hDC, float DeltaTime)
{
	if (!m_Start)
		Start();


	//���� ���� ���
	if (m_ReactionTime > 0.f)
	{
		m_ReactionTime -= DeltaTime;
		if (m_FlickerOn)
		{
			m_FlickerFrequency -= DeltaTime;

			if (m_FlickerFrequency < 0.f)
			{
				m_Flicker = !m_Flicker;
				m_FlickerFrequency = 0.05f;
			}
		}
		else
			m_Flicker = false;
	}
	else
	{
		m_Flicker = false;
		m_CurrentReaction = EReactionChannel::Normal;
	}


	//���� ���� = true�̰� EReactionChannel = Normal�̸� ������ ������ �ݺ�
	if (!m_Flicker || m_CurrentReaction != EReactionChannel::Normal)
	{
		Vector2 Res;
		Vector2	Pos;
		CCamera* Camera = nullptr;

		if (m_Scene)
		{
			Camera = m_Scene->GetCamera();
			Res = Camera->GetRes();
			Pos = m_RenderPos - Camera->GetPos();
		}
		else
		{
			Camera = CSceneManager::GetInst()->GetScene()->GetCamera();
			Res = Camera->GetRes();
			Pos = m_RenderPos - Camera->GetPos();
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

				Vector2 RenderSize = FrameData.End - FrameData.Start;
				//�ִϸ��̼� ��ü�� ������ �� + ������ ��ü�� ���� ������ ��
				Vector2 RenderLT;

				if (m_ForceSizeRender)
				{
					RenderLT = Pos - (m_Size * Current->m_Pivot).Round() + Current->m_Offset + FrameData.Offset;
				}
				else
				{
					RenderLT = Pos - (RenderSize * Current->m_Pivot).Round() + Current->m_Offset + FrameData.Offset;
				}


				//������Ʈ �ø�
				//ȭ���� �ȿ� �ִ� CGameObject�� ����ؼ� ����ȭ ����
				//ȭ���� ����� ����ٴϴ� UI�� ��Ȱ��ȭ
				if (
					m_CullingDeleteFlag
					& (UINT)ECullingDelete::CullingOn
					)
				{
					if (!Culling(RenderLT, RenderSize, Res))
					{
						if (m_WidgetComponent)
							m_WidgetComponent->SetEnable(false);
						return;
					}
					else if (m_WidgetComponent)
					{
						m_WidgetComponent->SetEnable(true);
					}
				}


				CTexture* Tex = Current->m_Sequence->GetTexture();

				if (Current->m_Sequence->GetTextureType() == ETextureType::Sprite)
				{
					//���� ���� Ȯ��. �� ���׼� ����
					int ReactionNum = 0;
					if (m_MaxReactionChannel > EReactionChannel::Normal && !m_Flicker)
					{
						ReactionNum = (int)m_CurrentReaction;
					}


					if (Current->m_Sequence->GetTexture()->GetColorKeyEnable())
					{

						TransparentBlt(hDC,
							(int)RenderLT.x, (int)RenderLT.y,
							(int)RenderSize.x, (int)RenderSize.y,
							Tex->GetDC(ReactionNum),
							(int)FrameData.Start.x, (int)FrameData.Start.y,
							(int)RenderSize.x, (int)RenderSize.y,
							Tex->GetColorKey(ReactionNum));
					}
					else
					{
						StretchBlt(hDC,
							(int)RenderLT.x, (int)RenderLT.y,
							(int)RenderSize.x, (int)RenderSize.y,
							Tex->GetDC(ReactionNum),
							(int)FrameData.Start.x, (int)FrameData.Start.y,
							(int)RenderSize.x, (int)RenderSize.y,
							SRCCOPY);
						//StretchBlt(hDC, (int)RenderLT.x, (int)RenderLT.y, (int)m_Size.x, (int)m_Size.y,
							//Current->m_Sequence->GetTexture()->GetDC(), 0, 0, SRCCOPY, );
					}
				}
				else //ETexturetype::Frame
				{
					if (Current->m_Sequence->GetTexture()->GetColorKeyEnable(CurrentFrame))
					{

						TransparentBlt(hDC,
							(int)RenderLT.x, (int)RenderLT.y,
							(int)RenderSize.x, (int)RenderSize.y,
							Tex->GetDC(CurrentFrame), 0, 0,
							(int)RenderSize.x, (int)RenderSize.y,
							Tex->GetColorKey(CurrentFrame));
					}
					else
					{
						StretchBlt(hDC,
							(int)RenderLT.x, (int)RenderLT.y,
							(int)RenderSize.x, (int)RenderSize.y,
							Tex->GetDC(CurrentFrame), 0, 0,
							(int)RenderSize.x, (int)RenderSize.y, SRCCOPY);

						/*BitBlt(hDC, (int)RenderLT.x, (int)RenderLT.y, (int)m_Size.x, (int)m_Size.y, Current->m_Sequence->GetTexture()->GetDC(CurrentFrame), 0, 0, SRCCOPY);*/
					}
				}

			}


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
}


int CDaiManji::InflictDamage(int Damage)
{
	int DMG = CEnemy::InflictDamage(Damage);

	m_HP -= DMG;
	StartReaction(0.1f, false, EReactionChannel::HitReaction);

	if (m_HP <= 0)
	{
		StopAnimation((int)EDaiManjiLayerInfo::HatchAction);

		//�������� -> Update()���� ó��
		ClearCollider();
		m_Coll = nullptr;
	}

	return DMG;
}

void CDaiManji::SetEssential(float DestPosX)
{
	m_MoveToDest.x = DestPosX;
}

void CDaiManji::Start()
{
	m_Start = true;


}

void CDaiManji::SetBoss(bool isBoss)
{
	m_isBoss = isBoss;
}




void CDaiManji::Routine(float DeltaTime)
{
	switch (m_Routine)
	{
	case (UINT8)EDaiManjiRoutine::Moveto:
	{
		//�̵� ����Ǹ� ���� ��ƾ����
		if(!m_MoveToOn)
			GoNextRoutine();
	}
	break;
	case (UINT8)EDaiManjiRoutine::Wait1:
	{
		if (CheckFirstEnter())
		{
			//3~5�ʸ��� UFO �ϳ� ����
			float WaitTime = (float)(rand() % 10 + 40) / 10.f;
			m_Timer.InitCooltime(WaitTime);
			m_Timer.EnterCooltime();
		}
		if (m_Timer.UpdateCooltime(DeltaTime))
			GoNextRoutine();

			
	}
	break;
	case (UINT8)EDaiManjiRoutine::Spawn:
	{
		if (CheckFirstEnter())
		{
			SpawnMiniUFO();
		}
		//if (m_isBoss)
		//	GoNextRoutine();
		else
		{
			m_Routine = (UINT)EDaiManjiRoutine::Wait1;
			m_FirstEnter = true;
		}
			
	}

	}
}


void CDaiManji::SpawnMiniUFO()
{
	ChangeAnimation("DaiManjiHatchOpen");
	SetEndFunction<CDaiManji>("DaiManjiHatchOpen", this, &CDaiManji::EndFuncSpawn, true);
}

void CDaiManji::EndFuncSpawn()
{
	if (m_MainScene->GetMiniUFOCounter())
	{
		ChangeAnimation("NormalUFOSpawn");
		m_UFOSpawnQueue = 2;
	}
	else
		CloseHatch();

}

void CDaiManji::CreateUFO()
{
	SoundPlay("MiniUFOSpawn");

	CMiniUFO* UFO = m_Scene->CreateObject<CMiniUFO>("MiniUFO");
	UFO->SetPos(m_RenderPos.x -2.f, m_RenderPos.y + 26.f);
	--m_UFOSpawnQueue;

	if (!m_MainScene->GetMiniUFOCounter() || m_UFOSpawnQueue <= 0)
		CloseHatch();
}



void CDaiManji::CloseHatch()
{
	StopAnimation((int)EDaiManjiLayerInfo::HatchAction);	//���������̾� 2 = ��ġ �� ���� �׼�
	ChangeAnimation("DaiManjiHatchClose");
}


//void CDaiManji::CollBegin(class CCollider* Src, class CCollider* Dest)
//{
//
//}