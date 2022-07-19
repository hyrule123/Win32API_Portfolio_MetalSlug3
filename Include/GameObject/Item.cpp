#include "Item.h"

#include "../Scene/Scene.h"

//�������� �÷��̾ ��밡��
#include "Player.h"

#include "../Collision/ColliderBox.h"

//����� ȹ��� ����Ʈ
#include "JustPlay.h"

//���� ǥ��
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

	//�������� �ݻ�Ǵ� ���� ������ �浹ü�� �������ָ� �ȴ�
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
		ClearCollider();//�ø� ���� ��ɶ����� �ڵ����� �����
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
	//1. ������� ���Ϳ� �浹�� �κ��� �������Ͱ� �ʿ��ϴ�.

	//2. ������� ������ ������(-p)�� ���Ѵ�.
	Vector2 InverseVec = m_Dir * -1.f;

	//3. ������(-p)�� ���� ������ (n) ���� (dot)�� ���Ѵ�.
	float Dot = InverseVec.Dot(NormalVector);

	//4. ���� ���͸� 2��� (2 * n) �ø���.
	Vector2 TwiceNormalVec = NormalVector * 2.f;

	//5. 2�� ������ �������͸� ������ ���� ����ŭ �������� �����Ѵ�. ( 2 * n * dot(-p, n) )
	TwiceNormalVec *= Dot;

	//6. ���� ������� ���� (p)�� 5������ ���� ���� (������ ������ ���� ����) �� ���Ѵ�. ( p + 5�� ��� ���� )
	m_Dir += TwiceNormalVec;

	//7. ������ �ݻ纤�͸� ����ȭ��Ų��. ( normalize reflect )
	m_Dir = m_Dir.Normalize();

	//8. 2d Screen�� Ư���� ����(y���� ������ ������ ����) �����͸� ���ϸ� �ϼ�(negate reflect)
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




	//�������� enum ��ȣ�� ���� �Ҹ��� �ٸ��� ����Ұ�
	if (m_ItemType <= EItemList::Bomb)
	{
		CJustPlay* Ammo = m_Scene->CreateObject<CJustPlay>("GetAmmo");
		Ammo->SetRenderLayer(ERenderLayer::EffectHigh);
		Ammo->SetEssential(m_Pos);
	}
	else
	{
		//������ �Դ¼Ҹ�
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
