#include "MainScene.h"
#include "SceneManager.h"
#include "../Input.h"
#include "GameOver.h"

//����ϴ� ���ӿ�����Ʈ
#include "../GameObject/AstroSlug.h"
#include "../GameObject/AstroSlugSFX.h"
#include "../GameObject/PlayerSpace.h"
#include "../GameObject/Explosion.h"
#include "../GameObject/Enemy.h"
#include "../GameObject/UFO.h"
#include "../GameObject/BackgroundSpace.h"
#include "../GameObject/PistolHit.h"
#include "../GameObject/Shotgun.h"
#include "../GameObject/AstroBomb.h"
#include "../GameObject/Pistol.h"
#include "../GameObject/RocketLauncher.h"
#include "../GameObject/Laser.h"
#include "../GameObject/AstroHMG.h"
#include "../GameObject/PlayerHMG.h"
#include "../GameObject/SpaceIntro.h"
#include "../GameObject/JustPlay.h"
#include "../GameObject/BackObjects.h"
#include "../GameObject/Meteor.h"
#include "../GameObject/MeteorSmall.h"
#include "../GameObject/MeteorMid.h"
#include "../GameObject/MeteorBig.h"
#include "../GameObject/ProjectileEnemy.h"
#include "../GameObject/BackObjects.h"
#include "../GameObject/DaiManjiElite.h"
#include "../UI/WindowMissionClear.h"



//����ϴ� UI
#include "../UI/WindowSpace.h"
#include "../UI/WidgetFadeInOut.h"

#include "SceneResource.h"

//Camera ó��
#include "Camera.h"

//�ʱ�ȭ �� �ػ� �޾ƿ��� �뵵
#include "../GameManager.h"

#include "../GameObject/DaiManji.h"


CMainScene::CMainScene():
	m_BackGround(),
	m_PhaseFlags(),
	m_Counter(),
	m_Timer0(),
	m_Timer1(),
	m_Timer2(),
	m_KeyMonsterCounter(),
	m_FirstPhaseEnter(true),
	m_MiniUFOMax(4),
	m_InCutscene()
{
}

CMainScene::~CMainScene()
{
}

bool CMainScene::Init()
{
	CScene::Init();

	CScoreManager::GetInst()->InitScore();


	LoadResource();

	//ī�޶� �غ�
	GetCamera()->SetRes((float)ORIGINAL_GAME_RES_WIDTH, 
		(float)ORIGINAL_GAME_RES_HEIGHT);
	GetCamera()->SetWorldRes(ORIGINAL_GAME_RES_WIDTH, 
		ORIGINAL_GAME_RES_HEIGHT * 2.f);
	GetCamera()->SetTargetPivot(0.5f, 0.8f);

	//����
	CWindowSpace* WindowSpace = CreateWidgetWindow<CWindowSpace>("WindowSpace");



	//��׶��� 
	m_BackGround = CreateObject<CBackgroundSpace>("BackGround");
	


	CreateObject<CSpaceIntro>("SpaceIntro");


	
	//0������ �غ�
	m_Timer0.InitCooltime(0.3f);
	m_Timer0.EnterCooltime();
	m_Timer1.InitCooltime(0.5f);



	return true;
}

void CMainScene::Update(float DeltaTime)
{
	CScene::Update(DeltaTime);


	PhaseUpdate(DeltaTime);

	if (m_PhaseFlags)
	{
		//������ 0���� ���ּ� �ټ� ����
		if (m_PhaseFlags <= ESPhase5_UFOPhase2)
		{
			m_Timer1.UpdateCooltime(DeltaTime);
			if (m_Timer1.EnterCooltime())
			{
				int RandObjType = rand() % 4;
				CBackObjects* Obj = CreateObject<CBackObjects>("BackObj");
				Obj->SetEssential((EBackObjType)RandObjType);
			}
		}

		else if(m_PhaseFlags <= ESPhase11_BigMeteor3 && m_PhaseFlags > ESPhase5_UFOPhase2)
		{
			m_Timer1.UpdateCooltime(DeltaTime);
			if (m_Timer1.EnterCooltime())
			{
				CMeteorSmall* Meteor = CreateObject< CMeteorSmall>("MeteorSmall");
			}

			if (m_PhaseFlags < ESPhase8_BigMeteor0)
			{
				m_Timer2.UpdateCooltime(DeltaTime);
				if (m_Timer2.EnterCooltime())
				{
					CMeteorMid* Mid = CreateObject<CMeteorMid>("MeteorMid");
					++m_Counter;
					if (m_Counter % 5 == 0)
					{
						int RandItem = rand() % 15 + 6;
						Mid->SetItemDrop((EItemList)RandItem);
					}
				}
			}
		}


		else if (m_PhaseFlags >= ESPhase16_Meteor1 && m_PhaseFlags <= ESPhase18_Meteor3)
		{
			m_Timer1.UpdateCooltime(DeltaTime);
			if (m_Timer1.EnterCooltime())
			{
				CMeteorSmall* Meteor = CreateObject< CMeteorSmall>("MeteorSmall");
			}

			m_Timer2.UpdateCooltime(DeltaTime);
			if (m_Timer2.EnterCooltime())
			{
				CMeteorMid* Mid = CreateObject<CMeteorMid>("MeteorMid");
				++m_Counter;
				if (m_Counter == 6)
				{
					Mid->SetItemDrop(EItemList::Shotgun);
				}
				else if (m_Counter % 5 == 0)
				{
					Mid->SetItemDrop(RandItem());
				}
			}
		}
		

		else if (m_PhaseFlags >= ESPhase12_UFO0 && m_PhaseFlags <= ESPhase15_UFO3)
		{
			m_Timer1.UpdateCooltime(DeltaTime);
			if (m_Timer1.EnterCooltime())
			{
				int RandObjType = rand() % (int)EBackObjType::End;
				CBackObjects* Obj = CreateObject<CBackObjects>("BackObj");
				Obj->SetEssential((EBackObjType)RandObjType);
			}
		}

		else if(m_PhaseFlags <= ESPhase11_BigMeteor3 && m_PhaseFlags > ESPhase5_UFOPhase2)
		{
			m_Timer1.UpdateCooltime(DeltaTime);
			if (m_Timer1.EnterCooltime())
			{
				CMeteorSmall* Meteor = CreateObject< CMeteorSmall>("MeteorSmall");
			}

			if (m_PhaseFlags < ESPhase8_BigMeteor0)
			{
				m_Timer2.UpdateCooltime(DeltaTime);
				if (m_Timer2.EnterCooltime())
				{
					CMeteorMid* Mid = CreateObject<CMeteorMid>("MeteorMid");
					++m_Counter;
					if (m_Counter == 6)
					{
						Mid->SetItemDrop(EItemList::HMG);
					}
					else if (m_Counter % 5 == 0)
					{
						int RandItem = rand() % 15 + 6;
						Mid->SetItemDrop((EItemList)RandItem);
					}
				}
			}
		}


		else if (m_PhaseFlags >= ESPhase20_DaiManji1 && m_PhaseFlags <= ESPhase21_DaiManjiElite)
		{
			m_Timer1.UpdateCooltime(DeltaTime);
			if (m_Timer1.EnterCooltime())
			{
				int RandObjType = rand() % (int)EBackObjType::End;
				CBackObjects* Obj = CreateObject<CBackObjects>("BackObj");
				Obj->SetEssential((EBackObjType)RandObjType);
			}
		}

		else if (m_PhaseFlags >= ESPhase20_DaiManji1 && m_PhaseFlags <= ESPhase21_DaiManjiElite)
		{
			m_Timer1.UpdateCooltime(DeltaTime);
			if (m_Timer1.EnterCooltime())
			{
				int RandObjType = rand() % (int)EBackObjType::End;
				CBackObjects* Obj = CreateObject<CBackObjects>("BackObj");
				Obj->SetEssential((EBackObjType)RandObjType);
			}
		}

		else if (m_PhaseFlags >= ESPhase22_Ending1)
		{
			m_Timer1.UpdateCooltime(DeltaTime);
			if (m_Timer1.EnterCooltime())
		{
			int RandObjType = rand() % ((int)EBackObjType::BigAstro + 1);
			CBackObjects* Obj = CreateObject<CBackObjects>("BackObj");
			Obj->SetEssential((EBackObjType)RandObjType);
		}
		}
	}
}

void CMainScene::PostUpdate(float DeltaTime)
{
	CScene::PostUpdate(DeltaTime);
}

void CMainScene::Render(HDC hDC, float DeltaTime)
{
	CScene::Render(hDC, DeltaTime);
}


void CMainScene::PhaseUpdate(float DeltaTime)
{
	if (!m_PhaseFlags)
		return;

	switch (m_PhaseFlags)
	{
	case ESPhase1_Wait:
	{
		//��Ÿ�� ������Ʈ�� ���༭ ���� ȿ���� ����
		if (!m_Timer0.UpdateCooltime(DeltaTime))
		{

		}
		else//1������ �� �׽�Ʈ ����
		{

			GoNextPhase();
		}
			
		break;
	}
	case ESPhase2_Wait:
		m_Timer0.InitCooltime(3.f);
		break;
	case ESPhase3_Wait:
		if (m_Timer0.UpdateCooltime(DeltaTime))
			GoNextPhase();
		break;
	case ESPhase4_UFOSpawn1:
	{
		if (CheckFirstEnter())
		{
			//Ű���� 1����
			CUFO* UFO = CreateObject<CUFO>("UFO1");
			if (UFO)
			{
				UFO->SetKeyMonster();
				UFO->SetItemDrop(EItemList::HMG);
			}

			m_KeyMonsterCounter = 1;
		}
		break;
	}
	case ESPhase4_1_Wait:	//3�� ����� ���� ������
	{
		if (CheckFirstEnter())
			m_Timer0.InitCooltime(3.f);

		if (m_Timer0.UpdateCooltime(DeltaTime))
			GoNextPhase();

		break;
	}
	case ESPhase5_UFOPhase2://Ű���� ������ �Ѿ��
	{
		if (CheckFirstEnter())
		{
			//Ű���� 2���� �����ϰ� ���������� �̵�.
			CUFO* UFO = CreateObject<CUFO>("UFO1");
			if (UFO)
				UFO->SetKeyMonster();

			UFO = nullptr;
			UFO = CreateObject<CUFO>("UFO2");
			if (UFO)
				UFO->SetKeyMonster();

			m_KeyMonsterCounter = 2;
		}
	}
	break;
	case ESPhase6_1_Wait:
	{
		if (CheckFirstEnter())
			m_Timer0.InitCooltime(5.f);	

		if (m_Timer0.UpdateCooltime(DeltaTime))
			GoNextPhase();
	}
		break;
	case ESPhase7_Meteor://� ������
	{
		if (CheckFirstEnter())
		{
			//�̹� ������ Ÿ�̸� ����
			m_Timer0.InitCooltime(30.f);	//������ �Ѿ������ Ÿ�̸�
			m_Timer0.EnterCooltime();
			m_Timer1.InitCooltime(0.3f);	//����� ������ Ÿ�̸�
			m_Timer2.InitCooltime(0.5f);	//���� � ������ Ÿ�Ӹ�
			m_Counter = 0;					//Ư�� ��° ���� ��� ������ ���
		}
		
		if (m_Timer0.UpdateCooltime(DeltaTime))
		{
			GoNextPhase();
		}
	}
		break;

	case ESPhase8_BigMeteor0://���� � ������
	{
		if (CheckFirstEnter())
		{
			m_Timer0.InitCooltime(3.f);
			m_Timer0.EnterCooltime();

			CMeteorBig* Meteor = CreateObject< CMeteorBig>("Meteor");
			if (Meteor)
			{
				Meteor->SetEssential(150.f, 150.f);
				Meteor->SetItemDrop(EItemList::Laser);
			}
		}

		if (m_Timer0.UpdateCooltime(DeltaTime))
		{
			GoNextPhase();
		}
		break;
	}
	
	case ESPhase9_BigMeteor1://���� � ������
	{
		if (CheckFirstEnter())
		{
			m_Timer0.InitCooltime(5.f);
			m_Timer0.EnterCooltime();

			CMeteorBig* Meteor = CreateObject< CMeteorBig>("Meteor");
			if (Meteor)
			{
				Meteor->SetEssential(50.f, 50.f);
			}

			Meteor = nullptr;
			Meteor = CreateObject< CMeteorBig>("Meteor");
			if (Meteor)
			{
				Meteor->SetEssential(250.f, 250.f);
			}
		}

		if (m_Timer0.UpdateCooltime(DeltaTime))
		{
			GoNextPhase();
		}
		break;
	}
	case ESPhase10_BigMeteor2://���� � ������
	{
		if (CheckFirstEnter())
		{
			m_Timer0.InitCooltime(5.f);
			m_Timer0.EnterCooltime();

			CMeteorBig* Meteor = CreateObject< CMeteorBig>("Meteor");
			if (Meteor)
			{
				Meteor->SetEssential(150.f, 150.f);
				Meteor->SetItemDrop(EItemList::Gas);
			}
		}

		if (m_Timer0.UpdateCooltime(DeltaTime))
		{
			GoNextPhase();
		}
		break;
	}
	case ESPhase11_BigMeteor3://���� � ������ ������
	{
		if (CheckFirstEnter())
		{
			m_Timer0.InitCooltime(20.f);
			m_Timer0.EnterCooltime();

			CMeteorBig* Meteor = CreateObject< CMeteorBig>("Meteor");
			if (Meteor)
			{
				Meteor->SetEssential(50.f, 50.f);
				Meteor->SetItemDrop(EItemList::Gas);
			}

			Meteor = nullptr;
			Meteor = CreateObject< CMeteorBig>("Meteor");
			if (Meteor)
			{
				Meteor->SetEssential(150.f, 150.f);
				Meteor->SetItemDrop(EItemList::Rocket);
			}

			Meteor = nullptr;
			Meteor = CreateObject< CMeteorBig>("Meteor");
			if (Meteor)
			{
				Meteor->SetEssential(250.f, 250.f);
				Meteor->SetItemDrop(EItemList::Bomb);
			}
		}

		if (m_Timer0.UpdateCooltime(DeltaTime))
		{
			GoNextPhase();
		}
		break;
	}
	case ESPhase12_UFO0:
	{
		if (CheckFirstEnter())
		{
			//���� �� ���ּ� ������ ���� Ÿ�̸� �ʱ�ȭ
			m_Timer1.InitCooltime(0.2f);


			CUFO* UFO = CreateObject<CUFO>("UFO1");
			if (UFO)
			{
				UFO->SetKeyMonster();
			}

			UFO = nullptr;
			UFO = CreateObject<CUFO>("UFO2");
			if (UFO)
			{
				UFO->SetKeyMonster();
			}

			m_KeyMonsterCounter = 2;
		}
		break;
	}
	case ESPhase12_1_Wait:
	{
		if (CheckFirstEnter())
			m_Timer0.InitCooltime(3.f);
		if (m_Timer0.UpdateCooltime(DeltaTime))
			GoNextPhase();

		break;
	}
	case ESPhase13_UFO1:
	{
		if (CheckFirstEnter())
		{
			CUFO* UFO = CreateObject<CUFO>("UFO1");
			if (UFO)
			{
				UFO->SetKeyMonster();
			}


			UFO = nullptr;
			UFO = CreateObject<CUFO>("UFO2");
			if (UFO)
				UFO->SetKeyMonster();

			UFO = nullptr;
			UFO = CreateObject<CUFO>("UFO3");
			if (UFO)
				UFO->SetKeyMonster();

			UFO = nullptr;
			UFO = CreateObject<CUFO>("UFO4");

			UFO = nullptr;
			UFO = CreateObject<CUFO>("UFO5");

			UFO = nullptr;
			UFO = CreateObject<CUFO>("UFO6");

			m_KeyMonsterCounter = 2;
		}
		break;
	}
	case ESPhase13_1_Wait:
	{
		WaitPhase(3.f, DeltaTime);

		break;
	}
	case ESPhase14_UFO2:
	{
		if (CheckFirstEnter())
		{
			CUFO* UFO = CreateObject<CUFO>("UFO1");
			if (UFO)
				UFO->SetKeyMonster();


			UFO = nullptr;
			UFO = CreateObject<CUFO>("UFO2");
			if (UFO)
				UFO->SetKeyMonster();

			UFO = nullptr;
			UFO = CreateObject<CUFO>("UFO3");
			if (UFO)
				UFO->SetKeyMonster();


			UFO = nullptr;
			UFO = CreateObject<CUFO>("UFO4");

			UFO = nullptr;
			UFO = CreateObject<CUFO>("UFO5");

			UFO = nullptr;
			UFO = CreateObject<CUFO>("UFO6");

			m_KeyMonsterCounter = 3;
		}
		break;
	}
	case ESPhase14_1_Wait:
	{
		WaitPhase(3.f, DeltaTime);
		break;
	}
	case ESPhase15_UFO3:
	{
		if (CheckFirstEnter())
		{
			CUFO* UFO = CreateObject<CUFO>("UFO1");
			if (UFO)
				UFO->SetKeyMonster();

			UFO = nullptr;
			UFO = CreateObject<CUFO>("UFO2");
			if (UFO)
				UFO->SetKeyMonster();

			UFO = nullptr;
			UFO = CreateObject<CUFO>("UFO3");
			if (UFO)
				UFO->SetKeyMonster();

			UFO = nullptr;
			UFO = CreateObject<CUFO>("UFO4");
			if (UFO)
			{
				UFO->SetKeyMonster();
				UFO->SetItemDrop(EItemList::Shotgun);
			}
				

			UFO = nullptr;
			UFO = CreateObject<CUFO>("UFO5");

			UFO = nullptr;
			UFO = CreateObject<CUFO>("UFO6");

			UFO = nullptr;
			UFO = CreateObject<CUFO>("UFO7");

			UFO = nullptr;
			UFO = CreateObject<CUFO>("UFO8");

			m_KeyMonsterCounter = 4;
		}
		break;
	}
	
	case ESPhase15_1_Wait:
	{
		WaitPhase(5.f, DeltaTime);
		break;
	}

	case ESPhase16_Meteor1:
	{
		if(CheckFirstEnter())
		{
			m_Timer0.InitCooltime(10.f);
			m_Timer0.EnterCooltime();
			m_Timer1.InitCooltime(0.5f);
			m_Timer2.InitCooltime(0.3f);
		}

		if (m_Timer0.UpdateCooltime(DeltaTime))//�̰� ������ �����������
			GoNextPhase();

		break;
	}
	case ESPhase17_Meteor2:
	{
		if (CheckFirstEnter())
		{
			m_Timer0.InitCooltime(5.f);
			m_Timer0.EnterCooltime();

			CMeteorBig* Meteor = CreateObject< CMeteorBig>("Meteor");
			if (Meteor)
			{
				Meteor->SetEssential(250.f, 250.f);
				Meteor->SetItemDrop(EItemList::Rocket);
			}

			Meteor = nullptr;
			Meteor = CreateObject<CMeteorBig>("Meteor");
			if (Meteor)
			{
				Meteor->SetEssential(150.f, 150.f);
				Meteor->SetItemDrop(EItemList::Shotgun);
			}
		}

		if (m_Timer0.UpdateCooltime(DeltaTime))
		{
			GoNextPhase();
		}
		
		break;
	}
	case ESPhase18_Meteor3:
	{
		if (CheckFirstEnter())
		{
			m_Timer0.InitCooltime(5.f);
			m_Timer0.EnterCooltime();

			CMeteorBig* Meteor = CreateObject< CMeteorBig>("Meteor");
			if (Meteor)
			{
				Meteor->SetEssential(50.f, 50.f);
				Meteor->SetItemDrop(RandItem());
			}

			Meteor = nullptr;
			Meteor = CreateObject<CMeteorBig>("Meteor");
			if (Meteor)
			{
				Meteor->SetEssential(150.f, 150.f);
				Meteor->SetItemDrop(RandItem());
			}
		}
		if (m_Timer0.UpdateCooltime(DeltaTime))
		{
			GoNextPhase();
		}

		break;
	}
	case ESPhase19_Meteor4:
	{
		if (CheckFirstEnter())
		{
			m_Timer0.InitCooltime(15.f);
			m_Timer0.EnterCooltime();
			
			CMeteorBig* Meteor = CreateObject< CMeteorBig>("Meteor");
			if (Meteor)
			{
				Meteor->SetEssential(50.f, 50.f);
				Meteor->SetItemDrop(RandItem());
			}

			Meteor = nullptr;
			Meteor = CreateObject< CMeteorBig>("Meteor");
			if (Meteor)
				if (Meteor)
				{
					Meteor->SetEssential(100.f, 100.f);
					Meteor->SetItemDrop(RandItem());
				}

			Meteor = nullptr;
			Meteor = CreateObject<CMeteorBig>("Meteor");
			if (Meteor)
			{
				Meteor->SetEssential(150.f, 150.f);
				Meteor->SetItemDrop(EItemList::Laser);
			}

			Meteor = nullptr;
			Meteor = CreateObject<CMeteorBig>("Meteor");
			if (Meteor)
			{
				Meteor->SetEssential(200.f, 200.f);
				Meteor->SetItemDrop(RandItem());
			}

			Meteor = nullptr;
			Meteor = CreateObject<CMeteorBig>("Meteor");
			if (Meteor)
			{
				Meteor->SetEssential(250.f, 250.f);
				Meteor->SetItemDrop(RandItem());
			}
		}

		if (m_Timer0.UpdateCooltime(DeltaTime))
		{
			GoNextPhase();
		}
		break;
	}
	case ESPhase20_DaiManji1:
	{
		if (CheckFirstEnter())
		{
			//��� ���ּ�
			m_Timer1.InitCooltime(1.f);

			CDaiManji* Dai = CreateObject<CDaiManji>("DaiManji");
			if (!Dai)
				throw std::_Xout_of_range;
			Dai->SetEssential(70.f);
			Dai->SetKeyMonster();

			Dai = nullptr;
			Dai = CreateObject<CDaiManji>("DaiManji");
			Dai->SetEssential(260.f);
			Dai->SetKeyMonster();

			m_KeyMonsterCounter = 2;
		}

		break;
	}
	case ESPhase20_1_Wait:
	{
		WaitPhase(5.f, DeltaTime);

		break;
	}
	case ESPhase21_DaiManjiElite:
	{
		if (CheckFirstEnter())
		{


			CDaiManjiElite* Elite = CreateObject<CDaiManjiElite>("DaiManjiElite");
			Elite->SetEssential(100.f);
			Elite->SetKeyMonster();
			m_KeyMonsterCounter = 1;
		}

		break;
	}
	
	case ESPhase22_Ending1:
	{
		if (CheckFirstEnter())
		{
			if (m_Player)
			{

				m_SceneResource->SoundStop("MainTheme");
				m_Player->SetMaxSpeed(50.f);
				m_Player->ClearCollider();
				m_Player->MoveToDest(true, EMoveToMethod::MoveDir,
					Vector2((float)(ORIGINAL_GAME_RES_WIDTH / 2), m_Player->GetPos().y));
			}
				
		}
		if (m_Player->GetMoveToDone())
			GoNextPhase();

		break;
	}
	case ESPhase23_Ending2:
	{
		if (CheckFirstEnter())
		{
			m_SceneResource->SoundPlay("MissionComplete");
			CWindowMissionClear* MissionClear = CreateWidgetWindow<CWindowMissionClear>("CWindowMissionClear");
			MissionClear->RenderMissionComplete();

			if (m_Player)
			{
				m_Timer0.InitCooltime(10.f);
				m_Timer0.EnterCooltime();
				m_Player->MoveToDest(true, EMoveToMethod::MoveDir,
					Vector2((float)(ORIGINAL_GAME_RES_WIDTH / 2), -200.f));
				m_Player->SetSideColl(false);
				m_Player->SetCullingDelete(ECullingDelete::CullingOn, true);
				m_Player->SetCullingDelete(ECullingDelete::All, true);

				CAstroSlug* AstroSlug = dynamic_cast<CAstroSlug*>(m_Player);
				if (AstroSlug)
				{
					AstroSlug->SetTailBooster(true);
					m_BackGround->SetSpeedSoft(-400.f, 100.f);
				}
			}

		}

		if (m_Timer0.UpdateCooltime(DeltaTime))
			GoNextPhase();

		break;
	}
	case  ESPhase24_Ending3:
	{
		if (CheckFirstEnter())
		{
			CWidgetWindow* Window = FindWidgetWindow("WindowSpace");
			if (Window)
			{
				Window->FadeInOut(false);
			}
			CSceneManager::GetInst()->CreateScene<CGameOver>();
			CGameOver* GameOver = dynamic_cast<CGameOver*>(
				CSceneManager::GetInst()->GetNextScene());
			if (!GameOver)
				return;

			//�׾ ���ӿ����ƴٰ� �˷���.
			GameOver->SetEssential(false);

		}
		break;
	}
	}

}

void CMainScene::WaitPhase(float WaitTime, float DeltaTime)
{
	if (CheckFirstEnter())
	{
		m_Timer0.InitCooltime(WaitTime);
		m_Timer0.EnterCooltime();
	}
		
	if (m_Timer0.UpdateCooltime(DeltaTime))
		GoNextPhase();
}

EItemList CMainScene::RandItem()
{
	int Index = 0;
	int RandItem = 0;

	do {
		Index = (int)EItemList::End - 1 - (int)EItemList::Bomb;
		RandItem = rand() % Index + (int)EItemList::Bomb + 1;
	} while ((RandItem < (int)EItemList::End && RandItem >= (int)EItemList::HMG) == false);


	return (EItemList)RandItem;
}

EItemList CMainScene::RandWeapon()
{
	int Index = 0;
	int RandItem = 0;

	do {
		RandItem = rand() % (int)EItemList::Bomb + 1;
	} while ((RandItem < (int)EItemList::Bomb + 1 && RandItem >= (int)EItemList::HMG) == false);


	return (EItemList)RandItem;
}




void CMainScene::GoNextPhase()
{
	++m_PhaseFlags;
	m_FirstPhaseEnter = true;
	m_Counter = 0;
}

bool CMainScene::CheckFirstEnter()
{
	if (m_FirstPhaseEnter)
	{
		m_FirstPhaseEnter = false;
		return true;
	}

	return false;
}

void CMainScene::AddMiniUFOCounter(int Num)
{
	m_MiniUFOCounter += Num;
}

bool CMainScene::GetMiniUFOCounter() const
{
	if (m_MiniUFOCounter < m_MiniUFOMax)
		return true;

	return false;
}

void CMainScene::SetCutsceneMode(bool On)
{
	m_InCutscene = On;
}

bool CMainScene::CheckCutscenePlay() const
{
	return m_InCutscene;
}


void CMainScene::SetScrollMapSpeed(float Speed)
{
	if(m_BackGround)
		m_BackGround->SetSpeed(Speed);
}

void CMainScene::SetScrollMapSpeedSoft(float AdjustSpeed, float SpeedPerSec)
{
	if (m_BackGround)
		m_BackGround->SetSpeedSoft(AdjustSpeed, SpeedPerSec);
}

void CMainScene::KeyMonsterDead()
{
	--m_KeyMonsterCounter;
	if (m_KeyMonsterCounter <= 0)
		GoNextPhase();
}

void CMainScene::LoadResource()
{
	//���ҽ� �̸� �ε� - ���ӿ�����Ʈ
	CreateOriginalObj<CPlayerSpace>("PlayerSpace");
	CreateOriginalObj<CAstroSlug>("AstroSlug");
	CreateOriginalObj<CAstroSlugSFX>("AstroSlugSFX");
	CreateOriginalObj<CExplosion>("Explosion");
	CreateOriginalObj<CPistolHit>("PistolHit");
	CreateOriginalObj<CShotgun>("Shotgun");
	CreateOriginalObj<CPistol>("Pistol");
	CreateOriginalObj<CRocketLauncher>("RocketLauncher");
	CreateOriginalObj<CAstroBomb>("AstroBomb");
	CreateOriginalObj<CLaser>("Laser");
	CreateOriginalObj<CPlayerHMG>("PlayerHMG");
	CreateOriginalObj<CAstroHMG>("AstroHMG");
	CreateOriginalObj<CUFO>("UFO");
	CreateOriginalObj<CSpaceIntro>("SpaceIntro");
	CreateOriginalObj<CJustPlay>("JustPlay");
	CreateOriginalObj<CMeteor>("Meteor");
	CreateOriginalObj<CMeteorSmall>("Meteor");
	CreateOriginalObj<CMeteorMid>("MeteorMid");
	CreateOriginalObj<CMeteorBig>("MeteorBig");
	CreateOriginalObj<CProjectileEnemy>("ProjectileEnemy");
	CreateOriginalObj<CBackObjects>("BackObjects");
	CreateOriginalObj<CDaiManjiElite>("DaiManjiElite");

	//���ҽ� - ����

	GetSceneResource()->LoadSound("SFX", "MissionComplete", false, "MissionComplete.mp3");
	GetSceneResource()->LoadSound("UI", "OK", false, "OK.mp3");

	//INTRO
	GetSceneResource()->LoadSound("SFX", "Separation", false, "Intro/Separation.mp3");
	GetSceneResource()->LoadSound("SFX", "NoseOpen", false, "Intro/NoseOpen.mp3");
	GetSceneResource()->LoadSound("SFX", "AstroThrust", false, "Intro/AstroThrust.mp3");
	GetSceneResource()->LoadSound("SFX", "LowPitchThrust", false, "Intro/LowPitchThrust.mp3");
	GetSceneResource()->LoadSound("SFX", "MainTheme", true, "MS3_Into_The_Cosmos.mp3");


	//PLAYER
	GetSceneResource()->LoadSound("SFX", "AstroHatchOpen", false, "Player/AstroHatchOpen.mp3");
	GetSceneResource()->LoadSound("SFX", "TarmaDeath", false, "Player/TarmaDeath.mp3");
	GetSceneResource()->LoadSound("SFX", "AstroSelfDestruct", false, "Player/AstroSelfDestruct.mp3");
	GetSceneResource()->LoadSound("SFX", "AstroSlugOut", false, "Player/AstroSlugOut.mp3");
	GetSceneResource()->LoadSound("SFX", "AstroExplosion", false, "Player/AstroExplosion.mp3");
	GetSceneResource()->LoadSound("SFX", "AstroHit", false, "Player/AstroHit.mp3");



	//WEAPON
	GetSceneResource()->LoadSound("SFX", "PistolHit", false, "Weapon/PistolHit.mp3");
	GetSceneResource()->LoadSound("SFX", "BulletHitSound", false, "Weapon/BulletHitSound.mp3");
	GetSceneResource()->LoadSound("SFX", "LaserFire", true, "Weapon/LaserFire.mp3");
	GetSceneResource()->SoundSetNoDelay("LaserFire");

	GetSceneResource()->LoadSound("SFX", "Shotgun", false, "Weapon/Shotgun.mp3");
	GetSceneResource()->LoadSound("SFX", "Pistol", false, "Weapon/Pistol.mp3");
	GetSceneResource()->LoadSound("SFX", "RocketLauncherHit", false, "Weapon/RocketLauncherHit.mp3");
	GetSceneResource()->LoadSound("SFX", "RocketLauncherFire", false, "Weapon/RocketLauncherFire.mp3");
	GetSceneResource()->LoadSound("SFX", "AstroPistol", false, "Weapon/AstroPistol.mp3");
	GetSceneResource()->LoadSound("SFX", "AstroHMG", false, "Weapon/AstroPistol.mp3");
	GetSceneResource()->LoadSound("SFX", "HMGFire", false, "Weapon/HMGFire.mp3");

	//ENEMY
	GetSceneResource()->LoadSound("SFX", "BigMeteorCrack", false, "Enemy/BigMeteorCrack.mp3");
	GetSceneResource()->LoadSound("SFX", "DaiManjiDocking", false, "Enemy/DaiManjiDocking.mp3");
	GetSceneResource()->LoadSound("SFX", "MiniUFOLaser", false, "Enemy/MiniUFOLaser.mp3");
	GetSceneResource()->LoadSound("SFX", "UFOLaser", false, "Enemy/UFOLaser.mp3");
	GetSceneResource()->LoadSound("SFX", "MidExplosion", false, "Enemy/MidExplosion.mp3");
	GetSceneResource()->LoadSound("SFX", "SmallExplosion", false, "Enemy/SmallExplosion.mp3");
	GetSceneResource()->LoadSound("SFX", "BigExplosion", false, "Enemy/BigExplosion.mp3");

	GetSceneResource()->LoadSound("SFX", "Charge1", false, "Enemy/Charge1.mp3");
	GetSceneResource()->LoadSound("SFX", "Charge2", false, "Enemy/Charge2.mp3");
	GetSceneResource()->LoadSound("SFX", "DaiManjiDocking", false, "Enemy/DaiManjiDocking.mp3");
	GetSceneResource()->LoadSound("SFX", "MiniUFOSpawn", false, "Enemy/MiniUFOSpawn.mp3");
	GetSceneResource()->LoadSound("SFX", "MiniUFODestroy", false, "Enemy/MiniUFODestroy.mp3");
	GetSceneResource()->LoadSound("SFX", "RugName", false, "Enemy/RugName.mp3");


	GetSceneResource()->LoadSound("SFX", "MiscItemGet", false, "Item/MiscItemGet.mp3");
	GetSceneResource()->LoadSound("SFX", "GetHMG", false, "Item/GetHMG.mp3");
	GetSceneResource()->LoadSound("SFX", "GetLaser", false, "Item/GetLaser.mp3");
	GetSceneResource()->LoadSound("SFX", "GetRocket", false, "Item/GetRocket.mp3");
	GetSceneResource()->LoadSound("SFX", "GetShotgun", false, "Item/GetShotgun.mp3");

	
	//���ҽ� �̸� �ε� - �ؽ�ó
	m_SceneResource->LoadCSVPreset(TEXT("UI/BasicFontSilver.csv"));
	m_SceneResource->LoadCSVPreset(TEXT("UI/BasicFontGold.csv"));
	m_SceneResource->LoadCSVPreset(TEXT("UI/ScoreNumber.csv"));
	m_SceneResource->LoadCSVPreset(TEXT("UI/TimeNumber.csv"));
	m_SceneResource->LoadCSVPreset(TEXT("UI/MissionFont.csv"));
	m_SceneResource->LoadTexture("AmmoHUD", TEXT("UI/AmmoHUD.bmp"));
	m_SceneResource->SetColorKey("AmmoHUD");
	

}




