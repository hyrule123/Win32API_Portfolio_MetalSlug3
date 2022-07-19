#include "Item.h"

#include "../Scene/Scene.h"

//아이템은 플레이어만 사용가능
#include "Player.h"

#include "../Collision/ColliderBox.h"

//무기류 획득시 이펙트
#include "JustPlay.h"

//점수 표시
#include "../ScoreManager.h"
#include "../UI/WindowSpace.h"
#include "../UI/WidgetScore.h"

CItem::CItem():
	m_ItemType(),
	m_vecAnimName{},
	m_SideCollCheck(),
	m_LeftTime(10.f)
{
	SetTypeID<CItem>();


}

CItem::CItem(const CItem& Obj) :
	CGameObject(Obj),
	m_ItemType(Obj.m_ItemType),
	m_SideCollCheck(Obj.m_SideCollCheck),
	m_LeftTime(Obj.m_LeftTime)
{
	int size = (int)EItemList::End;
	for (int i = 0; i < size; ++i)
	{
		m_vecAnimName[i] = Obj.m_vecAnimName[i];
	}


}

CItem::~CItem()
{
}



bool CItem::LoadResource()
{
	if (!LoadCSVPreset(TEXT("UI/Items.csv")))
		return false;

	m_vecAnimName[(int)EItemList::HMG] = "ItemHMG";
	m_vecAnimName[(int)EItemList::Rocket] = "ItemRocket";
	m_vecAnimName[(int)EItemList::Shotgun] = "ItemShotgun";
	m_vecAnimName[(int)EItemList::Laser] = "ItemLaser";
	m_vecAnimName[(int)EItemList::Bomb] = "ItemBomb";
	m_vecAnimName[(int)EItemList::Gas] = "ItemGas";
	m_vecAnimName[(int)EItemList::Carrot] = "ItemCarrot";
	m_vecAnimName[(int)EItemList::Apple ] = 	"ItemApple";
	m_vecAnimName[(int)EItemList::BananaOne] = 	"ItemBananaOne";
	m_vecAnimName[(int)EItemList::BananaThree ] = 	"ItemBananaThree";
	m_vecAnimName[(int)EItemList::Avocado ] = 	"ItemAvocado";
	m_vecAnimName[(int)EItemList::Cabbage ] = 	"ItemCabbage";
	m_vecAnimName[(int)EItemList::WaterMelon ] = 	"ItemWaterMelon";
	m_vecAnimName[(int)EItemList::Pig ] = 	"ItemPig";
	m_vecAnimName[(int)EItemList::Medal1 ] = 	"ItemMedal1";
	m_vecAnimName[(int)EItemList::Medal2 ] = 	"ItemMedal2";
	m_vecAnimName[(int)EItemList::Letter ] = 	"ItemLetter";
	m_vecAnimName[(int)EItemList::DollBlue ] = 	"ItemDollBlue";
	m_vecAnimName[(int)EItemList::DollRed ] = 	"ItemDollRed";
	m_vecAnimName[(int)EItemList::Coin ] = 	"ItemCoin";
	m_vecAnimName[(int)EItemList::Key] = "ItemKey";



	return true;
}

bool CItem::Init(CGameObject* Obj)
{
	CGameObject::Init(Obj);

	//아이템이 반사되는 것이 끝나면 충돌체만 삭제해주면 된다
	SetCullingDelete(ECullingDelete::All, true);

	m_RenderLayer = ERenderLayer::EffectHigh;

	m_MaxSpeed = 200.f;

	return true;
}

void CItem::SetEssential(const Vector2& Pos, const Vector2& Dir, EItemList ItemType)
{
	m_Pos = Pos;
	m_Dir = Dir;


	m_ItemType = ItemType;
	SetAnimation(m_vecAnimName[(int)m_ItemType]);

	switch (m_ItemType)
	{
	case EItemList::HMG:

		break;
	case EItemList::Rocket:

		break;
	case EItemList::Laser:

		break;
	case EItemList::Shotgun:

		break;
	case EItemList::Bomb:

		break;
	case EItemList::Gas:
		
		break;
	default:
		break;
	}

	if (!m_Animation)
		return;
	m_Size = m_Animation->GetAnimSize();
	m_SideCollCheck.LT = m_Size * m_Pivot;
	m_SideCollCheck.RB = m_Size - m_SideCollCheck.LT;

	if (m_Pos.x - m_SideCollCheck.LT.x < 0.f)
		m_Pos.x = m_SideCollCheck.LT.x + 0.1f;
	else if (m_Pos.x + m_SideCollCheck.RB.x > (float)ORIGINAL_GAME_RES_WIDTH)
		m_Pos.x = (float)ORIGINAL_GAME_RES_WIDTH - m_SideCollCheck.RB.x - 0.1f;
	else if (m_Pos.y + m_SideCollCheck.LT.y < 0.f)
		m_Pos.y = m_SideCollCheck.LT.y + 0.1f;
	else if (m_Pos.y - m_SideCollCheck.RB.y > (float)ORIGINAL_GAME_RES_HEIGHT)
		m_Pos.y = (float)ORIGINAL_GAME_RES_HEIGHT - m_SideCollCheck.RB.y;

	CColliderBox* Coll = AddCollider<CColliderBox>("ItemColl");
	Coll->SetSize(m_Size);
	Coll->SetPivot(m_Pivot);
	Coll->SetCollisionProfile(ECollisionChannel::EnemyAttack);
	Coll->SetCollisionBeginFunc<CItem>(this, &CItem::CollisionBegin);
}

void CItem::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	MoveDir(m_Dir);

	m_LeftTime -= DeltaTime;
	if (m_LeftTime < 0.f)
	{
		ClearCollider();//컬링 삭제 기능때문에 자동으로 사라짐
		return;
	}


	if (m_Pos.x - m_SideCollCheck.LT.x < 0.f)
	{
		m_Pos.x = m_SideCollCheck.LT.x + 0.1f;
		Reflect(Vector2(1.f, 0.f));
	}
	else if (m_Pos.y - m_SideCollCheck.LT.y < 0.f)
	{
		m_Pos.y = m_SideCollCheck.LT.y + 0.1f;
		Reflect(Vector2(0.f, 1.f));
	}
	else if (m_Pos.x + m_SideCollCheck.RB.x > (float)ORIGINAL_GAME_RES_WIDTH)
	{
		m_Pos.x = (float)ORIGINAL_GAME_RES_WIDTH - m_SideCollCheck.RB.x;
		Reflect(Vector2(-1.f, 0.f));
	}
	else if (m_Pos.y + m_SideCollCheck.RB.y > (float)ORIGINAL_GAME_RES_HEIGHT)
	{
		m_Pos.y = (float)ORIGINAL_GAME_RES_HEIGHT - m_SideCollCheck.RB.y;
		Reflect(Vector2(0.f, -1.f));
	}

	


}

void CItem::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);

}

void CItem::Render(HDC hDC, float DeltaTime)
{
	CGameObject::Render(hDC, DeltaTime);
}

void CItem::Reflect(const Vector2& NormalVector)
{
	//1. 진행방향 벡터와 충돌한 부분의 법선벡터가 필요하다.

	//2. 진행방향 벡터의 역벡터(-p)를 구한다.
	Vector2 InverseVec = m_Dir * -1.f;

	//3. 역벡터(-p)와 법선 벡터의 (n) 내적 (dot)을 구한다.
	float Dot = InverseVec.Dot(NormalVector);

	//4. 법선 벡터를 2배로 (2 * n) 늘린다.
	Vector2 TwiceNormalVec = NormalVector * 2.f;

	//5. 2배 길이의 법선벡터를 내적이 계산된 값만큼 스케일을 조절한다. ( 2 * n * dot(-p, n) )
	TwiceNormalVec *= Dot;

	//6. 원래 진행방향 벡터 (p)에 5번에서 계산된 벡터 (스케일 조절된 법선 벡터) 를 더한다. ( p + 5번 결과 벡터 )
	m_Dir += TwiceNormalVec;

	//7. 구해진 반사벡터를 정규화시킨다. ( normalize reflect )
	m_Dir = m_Dir.Normalize();

	//8. 2d Screen의 특성에 따라(y값이 밑으로 갈수록 증가) 역벡터를 구하면 완성(negate reflect)
	//?
}

void CItem::CollisionBegin(CCollider* Src, CCollider* Dest)
{
	CPlayer* Player = dynamic_cast<CPlayer*>(Dest->GetOwnerObj());
	if (!Player)
		return;
	
	switch (m_ItemType)
	{
	case EItemList::HMG:
		Player->ChangeWeapon(EWeapon::HMG);
		break;
	case EItemList::Rocket:
		Player->ChangeWeapon(EWeapon::Rocket);
		break;
	case EItemList::Laser:
		Player->ChangeWeapon(EWeapon::Laser);
		break;
	case EItemList::Shotgun:
		Player->ChangeWeapon(EWeapon::Shotgun);
		break;
	case EItemList::Gas:
		Player->GetGas();
		break;
	case EItemList::Bomb:
		Player->ChangeWeapon(EWeapon::Bomb);
		break;

	case EItemList::Carrot:
		RenderScoreNum(100);
		break;
	case EItemList::Apple:
		RenderScoreNum(100);
		break;
	case EItemList::BananaOne:
		RenderScoreNum(100);
		break;
	case EItemList::BananaThree:
		RenderScoreNum(500);
		break;
	case EItemList::Avocado:
		RenderScoreNum(500);
		break;
	case EItemList::Cabbage:
		RenderScoreNum(500);
		break;
	case EItemList::WaterMelon:
		RenderScoreNum(500);
		break;
	case EItemList::Pig:
		RenderScoreNum(1000);
		break;
	case EItemList::Medal1:
		RenderScoreNum(100);
		break;
	case EItemList::Medal2:
		RenderScoreNum(100);
		break;
	case EItemList::Letter:
		RenderScoreNum(100);
		break;
	case EItemList::DollBlue:
		RenderScoreNum(5000);
		break;
	case EItemList::DollRed:
		RenderScoreNum(5000);
		break;
	case EItemList::Coin:
		RenderScoreNum(10);
		break;
	case EItemList::Key:
		RenderScoreNum(100);
		break;
	case EItemList::End:
		break;
	}




	//아이템의 enum 번호에 따라 소리를 다르게 재생할것
	if (m_ItemType <= EItemList::Bomb)
	{
		CJustPlay* Ammo = m_Scene->CreateObject<CJustPlay>("GetAmmo");
		Ammo->SetRenderLayer(ERenderLayer::EffectHigh);
		Ammo->SetEssential(m_Pos);
	}
	else
	{
		//아이템 먹는소리
	}

	SetActive(false);
}

void CItem::RenderScoreNum(int Score)
{
	CWidgetWindow* SpaceWindow = m_Scene->FindWidgetWindow("WindowSpace");

	if (SpaceWindow)
	{
		CWidgetScore* ScoreWind = SpaceWindow->CreateWidget<CWidgetScore>("Score");
		if (ScoreWind)
		{
			ScoreWind->SetText(Score);
			ScoreWind->SetPos(m_Pos);
		}
	}
		
	SoundPlay("MiscItemGet");
	CScoreManager::GetInst()->AddScore(Score);
}
