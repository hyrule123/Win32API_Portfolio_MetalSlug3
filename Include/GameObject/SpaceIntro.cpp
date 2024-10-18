#include "SpaceIntro.h"

//디버그용
#include "../GameManager.h"

//게임오브젝트 생성
#include "../Scene/MainScene.h"
//사운드 로드
#include "../Scene/SceneResource.h"

//로켓 파트
#include "IntroRocketMid.h"
#include "IntroRocketLowest.h"

//아스트로 슬러그 생성
#include "AstroSlug.h"



CSpaceIntro::CSpaceIntro():
	m_StartPoint(1500.f),
	m_RocketPart{},
	m_Phase(),
	m_PhaseFlags(),
	m_DeleteTimer(5.f)
{
	SetTypeID<CSpaceIntro>();
}

CSpaceIntro::CSpaceIntro(const CSpaceIntro& Obj) :
	CGameObject(Obj),
	m_StartPoint(Obj.m_StartPoint),
	m_RocketPart{},
	m_Phase(),
	m_PhaseFlags(),
	m_DeleteTimer(5.f)
{
}

CSpaceIntro::~CSpaceIntro()
{
}



bool CSpaceIntro::Init(CGameObject* Obj)
{
	CGameObject::Init();

	CMainScene* MainScene = dynamic_cast<CMainScene*>(m_Scene);
	if (MainScene)
		MainScene->SetScrollMapSpeed(-400.f);

	m_Pos.x = (float)ORIGINAL_GAME_RES_WIDTH / 2.f;
	m_Pos.y = - m_StartPoint;
	m_StartPoint -= 200.f;

	Phase0();

	return true;
}

void CSpaceIntro::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	//DEBUG
	//CGameManager::GetInst()->DebugTextOut("Pos", m_ThrustA->GetPos().y);

	if (m_Phase == 0)
	{
		if (m_RocketPart[m_Phase][Ring])
		{
			if (m_RocketPart[m_Phase][Ring]->GetPos().y > -50.f)
			{
				++m_Phase;
				Phase1();
				m_Scene->GetSceneResource()->SoundPlay("LowPitchThrust");
				
			}
		}
	}
	else if (m_Phase == 1)
	{

		if (!(m_PhaseFlags & (UINT8)(1 << 0)))
		{
			if (m_RocketPart[m_Phase][Ring])
			{
				if (m_RocketPart[m_Phase][Ring]->GetPos().y > -230.f)
				{
					m_PhaseFlags |= (UINT8)(1 << 0);
					
					m_Scene->GetSceneResource()->SoundPlay("Separation");

					m_RocketPart[m_Phase][Body]->SetAnimation("MiddleSeparateSFX", true);
					for (UINT8 i = 0; i < ERocketBodyInfoMax; ++i)
					{
						m_RocketPart[m_Phase - 1][i]->Separate();
					}


				}
			}
		}
		else if (!(m_PhaseFlags & (UINT8)(1 << 1)))
		{
			if (m_RocketPart[m_Phase][Ring])
			{
				if (m_RocketPart[m_Phase][Ring]->GetPos().y > -190.f)
				{
					m_PhaseFlags |= (UINT8)(1 << 1);

					m_RocketPart[m_Phase][Body]->ChangeAnimation("SideThrustStart");
					m_RocketPart[m_Phase][Thrust]->ChangeAnimation("ThrustOn");

					++m_Phase;
					m_PhaseFlags = 0;
				}
			}
		}
	}
	else if (m_Phase == 2)
	{
		if (!m_RocketPart[m_Phase][Ring])
		{
			Phase23();
			for (UINT8 i = 0; i < ERocketBodyInfoMax; ++i)
			{
				m_RocketPart[EPhase0][i]->SetActive(false);
				m_RocketPart[EPhase0][i] = nullptr;
			}
		}
		else
		{
			if (!(m_PhaseFlags & (UINT8)(1 << 0)))
			{
	
				if (m_RocketPart[m_Phase][Ring]->GetPos().y > -230.f)
				{
					m_PhaseFlags |= (UINT8)(1 << 0);

					m_RocketPart[m_Phase][Body]->SetAnimation("MiddleSeparateSFX", true);
					for (UINT8 i = 0; i < ERocketBodyInfoMax; ++i)
					{
						m_RocketPart[m_Phase - 1][i]->Separate();
					}

					m_Scene->GetSceneResource()->SoundPlay("Separation");
				}

			}
			else if (!(m_PhaseFlags & (UINT8)(1 << 1)))
			{

				if (m_RocketPart[m_Phase][Ring]->GetPos().y > -190.f)
				{
					m_PhaseFlags |= (UINT8)(1 << 1);

					m_RocketPart[m_Phase][Body]->ChangeAnimation("SideThrustStart");
					m_RocketPart[m_Phase][Thrust]->ChangeAnimation("ThrustOn");

					

					++m_Phase;
					m_PhaseFlags = 0;
				}

			}
		}
	}
	else if (m_Phase == 3)
	{
		if (!m_RocketPart[m_Phase][Ring])
		{
			Phase23();
			for (UINT8 i = 0; i < ERocketBodyInfoMax; ++i)
			{
				m_RocketPart[EPhase1][i]->SetActive(false);
				m_RocketPart[EPhase1][i] = nullptr;
			}
		}
		else
		{
			if (!(m_PhaseFlags & (UINT8)(1 << 0)))
			{

				if (m_RocketPart[m_Phase][Ring]->GetPos().y > -230.f)
				{
					m_PhaseFlags |= (UINT8)(1 << 0);

					m_RocketPart[m_Phase][Body]->SetAnimation("MiddleSeparateSFX", true);
					for (UINT8 i = 0; i < ERocketBodyInfoMax; ++i)
					{
						m_RocketPart[m_Phase - 1][i]->Separate();
					}

					m_Scene->GetSceneResource()->SoundPlay("Separation");
				}

			}
			else if (!(m_PhaseFlags & (UINT8)(1 << 1)))
			{

				if (m_RocketPart[m_Phase][Ring]->GetPos().y > -190.f)
				{
					m_PhaseFlags |= (UINT8)(1 << 1);

					m_RocketPart[m_Phase][Body]->ChangeAnimation("SideThrustStart");
					m_RocketPart[m_Phase][Thrust]->ChangeAnimation("ThrustOn");

					++m_Phase;
					m_PhaseFlags = 0;
				}

			}
		}
	}
	else if (m_Phase == 4)
	{
	//페이즈4는 Ring이 없으므로 Body로 체크
		if (!m_RocketPart[m_Phase][Body])
		{
			Phase4();
			for (UINT8 i = 0; i < ERocketBodyInfoMax; ++i)
			{
				m_RocketPart[EPhase2][i]->SetActive(false);
				m_RocketPart[EPhase2][i] = nullptr;
			}
		}
		else
		{
			if (!(m_PhaseFlags & (UINT8)(1 << 0)))
			{

				if (m_RocketPart[m_Phase][Body]->GetPos().y > -230.f)
				{
					m_PhaseFlags |= (UINT8)(1 << 0);

					m_RocketPart[m_Phase][Body]->SetAnimation("MiddleSeparateSFX", true);
					for (UINT8 i = 0; i < ERocketBodyInfoMax; ++i)
					{
						m_RocketPart[m_Phase - 1][i]->Separate();
					}

					m_Scene->GetSceneResource()->SoundPlay("Separation");
				}

			}
			else if (!(m_PhaseFlags & (UINT8)(1 << 1)))
			{
				if (m_RocketPart[m_Phase][Body]->GetPos().y > -190.f)
				{
					m_PhaseFlags |= (UINT8)(1 << 1);

					m_RocketPart[m_Phase][Body]->ChangeAnimation("SideThrustStart");
					m_RocketPart[m_Phase][Thrust]->ChangeAnimation("ThrustOn");
				}

			}
			else if (!(m_PhaseFlags & (UINT8)(1 << 2)))
			{
				if (m_RocketPart[m_Phase][Body]->GetPos().y > -80.f)
				{
					m_PhaseFlags |= (UINT8)(1 << 2);

					m_RocketNoseRight->SetAnimation("CapSeparateSFX", true);
					m_Scene->GetSceneResource()->SoundPlay("NoseOpen");
					
					//플레이어 생성
					m_Player = m_Scene->CreateObject<CAstroSlug>("AstroSlug", this);
					m_Player->SetRenderLayer(ERenderLayer::Slug);
					m_Player->SetCutSceneOffset(Vector2(-20.f, 450.f));
					
				}
			}
			else if (!(m_PhaseFlags & (UINT8)(1 << 3)))
			{
				if(m_RocketNoseRight->GetCapSeparateSFXNotified())
				{
					m_PhaseFlags |= (UINT8)(1 << 3);

					m_RocketNoseLeftUpper->DeleteMasterSlave();
					m_RocketNoseLeftUpper->NoseSeparation(-1.f);

					m_RocketNoseRight->DeleteMasterSlave();
					m_RocketNoseRight->NoseSeparation(1.f);

					
					
				}
			}
			else if (!(m_PhaseFlags & (UINT8)(1 << 4)))
			{
				if (m_RocketPart[m_Phase][Body]->GetPos().y > 80.f)
				{
					m_PhaseFlags |= (UINT8)1 << 4;

				}
			}
			else if (!(m_PhaseFlags & (UINT8)(1 << 5)))
			{
				if (m_RocketPart[m_Phase][Body]->GetPos().y > 130.f)
				{
					m_PhaseFlags |= (UINT8)1 << 5;

					m_Player->DeleteMasterSlave();
					m_Player->SetIntroCutsceneMode();
					m_Scene->GetSceneResource()->SoundPlay("AstroThrust");
					m_Scene->GetSceneResource()->SoundPlay("MainTheme");

				}
			}

			else if (!(m_PhaseFlags & (UINT8)(1 << 6)))
			{

				if (m_DeleteTimer > 0.f)
				{
					m_DeleteTimer -= DeltaTime;
				}
				else
				{
					//m_PhaseFlags |= (UINT8)(1 << 4);
					CutSceneOver();
				}

				//++m_Phase;
				//m_PhaseFlags = 0;
			}

		}
	}



	//위치 이동
	m_Pos.y += 200.f * DeltaTime;

	
}

void CSpaceIntro::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);

}

void CSpaceIntro::Phase0()
{
	{//Phase 0
		m_RocketPart[m_Phase][Ring] = m_Scene->CreateObject<CIntroRocket>("InnerRing", this);
		m_RocketPart[m_Phase][Ring]->SetRenderLayer(ERenderLayer::Default);
		m_RocketPart[m_Phase][Ring]->SetOffsetY(m_StartPoint);
		m_RocketPart[m_Phase][Ring]->ChangeAnimation("BodyUpperRingInner");

		m_RocketPart[m_Phase][Body] = m_Scene->CreateObject<CIntroRocket>("BodyLowest", this);
		m_RocketPart[m_Phase][Body]->ChangeAnimation("BodyLowest");

		m_RocketPart[m_Phase][Body]->SetRenderLayer(ERenderLayer::Slug);
		m_RocketPart[m_Phase][Body]->SetOffsetY(m_StartPoint);

		m_RocketPart[m_Phase][Thrust] = m_Scene->CreateObject<CIntroRocket>("Thrust", this);
		m_RocketPart[m_Phase][Thrust]->SetRenderLayer(ERenderLayer::Enemy);
		m_RocketPart[m_Phase][Thrust]->ChangeAnimation("ThrustOn");
		m_RocketPart[m_Phase][Thrust]->SetOffsetY(m_StartPoint);

		m_StartPoint -= 230.f;
	}
}



void CSpaceIntro::Phase1()
{
	m_RocketPart[m_Phase][Ring] = m_Scene->CreateObject<CIntroRocket>("InnerRing", this);
	m_RocketPart[m_Phase][Ring]->SetRenderLayer(ERenderLayer::Default);
	m_RocketPart[m_Phase][Ring]->SetOffsetY(m_StartPoint);
	m_RocketPart[m_Phase][Ring]->SetAnimOffset("BodyUpperRingInner", 0.f, -15.f);
	m_RocketPart[m_Phase][Ring]->ChangeAnimation("BodyUpperRingInner");

	m_RocketPart[m_Phase][Body] = m_Scene->CreateObject<CIntroRocket>("BodyMiddle", this);
	m_RocketPart[m_Phase][Body]->SetRenderLayer(ERenderLayer::Slug);
	m_RocketPart[m_Phase][Body]->SetOffsetY(m_StartPoint);
	m_RocketPart[m_Phase][Body]->ChangeAnimation("BodyMiddle");
	m_RocketPart[m_Phase][Body]->SetAnimOffset("MiddleSeparateSFX", 0.f, 255.f);


	m_RocketPart[m_Phase][Thrust] = m_Scene->CreateObject<CIntroRocket>("Thrust", this);
	m_RocketPart[m_Phase][Thrust]->SetAnimOffset("ThrustNormal", 0.f, 240.f);
	m_RocketPart[m_Phase][Thrust]->SetAnimOffset("ThrustOn", 0.f, 240);

	m_RocketPart[m_Phase][Thrust]->SetRenderLayer(ERenderLayer::Enemy);
	m_RocketPart[m_Phase][Thrust]->ChangeAnimation("ThrustNormal");
	m_RocketPart[m_Phase][Thrust]->SetOffsetY(m_StartPoint);

	m_StartPoint -= 230.f;
}

void CSpaceIntro::Phase23()
{
	m_RocketPart[m_Phase][Ring] = m_Scene->CreateObject<CIntroRocket>("InnerRing", this);
	m_RocketPart[m_Phase][Ring]->SetRenderLayer(ERenderLayer::Default);
	m_RocketPart[m_Phase][Ring]->SetOffsetY(m_StartPoint);
	m_RocketPart[m_Phase][Ring]->SetAnimOffset("BodyUpperRingInner", 0.f, -15.f);
	m_RocketPart[m_Phase][Ring]->ChangeAnimation("BodyUpperRingInner");

	m_RocketPart[m_Phase][Body] = m_Scene->CreateObject<CIntroRocket>("BodyMiddle", this);
	m_RocketPart[m_Phase][Body]->SetRenderLayer(ERenderLayer::Slug);
	m_RocketPart[m_Phase][Body]->SetOffsetY(m_StartPoint);
	m_RocketPart[m_Phase][Body]->ChangeAnimation("BodyMiddle");
	m_RocketPart[m_Phase][Body]->SetAnimOffset("MiddleSeparateSFX", 0.f, 255.f);


	m_RocketPart[m_Phase][Thrust] = m_Scene->CreateObject<CIntroRocket>("Thrust", this);
	m_RocketPart[m_Phase][Thrust]->SetAnimOffset("ThrustNormal", 0.f, 240.f);
	m_RocketPart[m_Phase][Thrust]->SetAnimOffset("ThrustOn", 0.f, 240);

	m_RocketPart[m_Phase][Thrust]->SetRenderLayer(ERenderLayer::Enemy);
	m_RocketPart[m_Phase][Thrust]->ChangeAnimation("ThrustNormal");
	m_RocketPart[m_Phase][Thrust]->SetOffsetY(m_StartPoint);

	m_StartPoint -= 230.f;
}

void CSpaceIntro::Phase4()
{
	m_RocketNoseLeftUpper = m_Scene->CreateObject<CIntroRocket>("HeadLeftSideUpper", this);
	m_RocketNoseLeftUpper->SetRenderLayer(ERenderLayer::EffectLow);
	m_RocketNoseLeftUpper->ChangeAnimation("HeadLeftSideUpper");
	m_RocketNoseLeftUpper->SetOffsetY(m_StartPoint);
	


	m_RocketNoseLeftLow = m_Scene->CreateObject<CIntroRocket>("HeadLeftSideLower", m_RocketNoseLeftUpper);
	m_RocketNoseLeftLow->SetRenderLayer(ERenderLayer::Default);
	m_RocketNoseLeftLow->ChangeAnimation("HeadLeftSideLower");
	m_RocketNoseLeftLow->SetOffsetY(m_StartPoint);


	m_RocketNoseRight = m_Scene->CreateObject<CIntroRocket>("HeadRightSideWhole", this);
	m_RocketNoseRight->SetRenderLayer(ERenderLayer::EffectHigh);
	m_RocketNoseRight->ChangeAnimation("HeadRightSideWhole");
	m_RocketNoseRight->SetOffsetY(m_StartPoint);



	m_RocketPart[m_Phase][Body] = m_Scene->CreateObject<CIntroRocket>("HeadInner", this);
	m_RocketPart[m_Phase][Body]->SetRenderLayer(ERenderLayer::Player);
	m_RocketPart[m_Phase][Body]->SetOffsetY(m_StartPoint);
	m_RocketPart[m_Phase][Body]->ChangeAnimation("HeadInner");
	m_RocketPart[m_Phase][Body]->SetAnimOffset("MiddleSeparateSFX", 0.f, 255.f);


	m_RocketPart[m_Phase][Thrust] = m_Scene->CreateObject<CIntroRocket>("Thrust", this);
	m_RocketPart[m_Phase][Thrust]->SetAnimOffset("ThrustNormal", 0.f, 240.f);
	m_RocketPart[m_Phase][Thrust]->SetAnimOffset("ThrustOn", 0.f, 240);

	m_RocketPart[m_Phase][Thrust]->SetRenderLayer(ERenderLayer::Enemy);
	m_RocketPart[m_Phase][Thrust]->ChangeAnimation("ThrustNormal");
	m_RocketPart[m_Phase][Thrust]->SetOffsetY(m_StartPoint);

	m_StartPoint -= 230.f;
}

void CSpaceIntro::CutSceneOver()
{



	for (UINT8 i = EPhase3; i < EPhaseMax ; ++i)
	{
		for (UINT8 j = 0; j < ERocketBodyInfoMax; ++j)
		{
			if (m_RocketPart[i][j])
			{
				m_RocketPart[i][j]->SetActive(false);
			}
		}
	}
	m_RocketNoseLeftUpper->SetActive(false);
	m_RocketNoseLeftLow->SetActive(false);
	m_RocketNoseRight->SetActive(false);
	SetActive(false);
}

