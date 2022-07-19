#include "LaserDebris.h"

#include "../GameManager.h"
#include "../Collision/ColliderLine.h"

#include "../Scene/Camera.h"
#include "../Scene/SceneManager.h"
#include "../Resource/Texture/Texture.h"

//�����ѹ� �޾ƿ�������
#include "../GameManager.h"




CLaserDebris::CLaserDebris() :
	m_ColorNum{},
	m_Index(),
	m_ChangeIndex(),
	m_Force()
{
	SetTypeID<CLaserDebris>();
}

CLaserDebris::CLaserDebris(const CLaserDebris& Obj) :
	CJustMovingSFX(Obj),
	m_ColorNum{},	//Init���� �ʱ�ȭ
	m_Index(Obj.m_Index),
	m_ChangeIndex(Obj.m_ChangeIndex),
	m_Force(Obj.m_Force)
{
}

CLaserDebris::~CLaserDebris()
{
}

bool CLaserDebris::Init(CGameObject* Obj)
{
	CJustMovingSFX::Init(Obj);

	SetAnimation("LasetDebris");

	m_ColorNum[0] = (UINT8)EReactionChannel::Normal;
	m_ColorNum[1] = (UINT8)EReactionChannel::Laser1;
	m_ColorNum[2] = (UINT8)EReactionChannel::Laser2;
	m_ColorNum[3] = (UINT8)EReactionChannel::Laser3;



	return true;
}

void CLaserDebris::Start()
{
	m_Start = true;

	m_PrevPos = m_Pos;
}



void CLaserDebris::Update(float DeltaTime)
{
	if (!m_Start)
		Start();

	CJustMovingSFX::Update(DeltaTime);

	AddImpact(m_Force);
}

void CLaserDebris::PostUpdate(float DeltaTime)
{
	if (!m_Start)
		Start();


	CJustMovingSFX::PostUpdate(DeltaTime);
}




void CLaserDebris::SetEssential(const Vector2& Dir, const Vector2& Pos, const Vector2& Offset, float Force)
{
	m_isReady = true;

	m_Dir = Dir;
	m_Pos = Pos;

	m_Offset = Offset;

	if (m_MasterObject)
		m_Pos = m_MasterObject->GetPos() + m_Offset;

	Trajectory(Force);

}

void CLaserDebris::SetEssential(float DirX, float DirY, float PosX, float PosY, float OffsetX, float OffsetY, float Force)
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

	Trajectory(Force);
}




void CLaserDebris::Render(HDC hDC, float DeltaTime)
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

			RenderSize = FrameData.End - FrameData.Start;

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
		if (m_ChangeIndex)
			m_Index = rand() % 4;
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

bool CLaserDebris::LoadResource()
{
	if (!LoadCSVPreset(TEXT("SFX/LaserDebris.csv")))
		return false;


	return true;
}

void CLaserDebris::Trajectory(float Force)
{
	m_Force = Force;
	SetMaxSpeed(m_Force);
	SetDeAccel(0.9f);
	SetGravityAccel(15.f);
	SetPhysicsSimulate(true);
	Jump();
}