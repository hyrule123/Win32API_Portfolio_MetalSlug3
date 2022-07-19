#include "BackgroundSpace.h"

//텍스처 주소 들고있을 용도
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Texture/Texture.h"


CBackgroundSpace::CBackgroundSpace():
	m_ScrollPos(),
	m_ScrollingSpeed(),
	m_AdjustSpeed()
{
	SetTypeID<CBackgroundSpace>();

}

CBackgroundSpace::CBackgroundSpace(const CBackgroundSpace& Obj):
	CBackground(Obj),
	m_ScrollPos(Obj.m_ScrollPos),
	m_ScrollingSpeed(Obj.m_ScrollingSpeed),
	m_AdjustSpeed(Obj.m_AdjustSpeed)
{
}

CBackgroundSpace::~CBackgroundSpace()
{
}

bool CBackgroundSpace::Init(CGameObject* Obj)
{
	SetTexture("BackgroundSpace", TEXT("Background/SpaceBackground.bmp"));
	m_Texture = m_Scene->GetSceneResource()->FindTexture("BackgroundSpace");
	SetSize(m_Texture->GetTextureSize());

	m_ScrollingSpeed = -100.f;

	m_RenderLayer = ERenderLayer::ScrollMap;

	return true;
}

void CBackgroundSpace::Start()
{
	CBackground::Start();
}

void CBackgroundSpace::Update(float DeltaTime)
{
	CBackground::Update(DeltaTime);

	m_ScrollPos += m_ScrollingSpeed * DeltaTime;
	if (m_ScrollingSpeed < 0.f)
	{
		if (m_ScrollPos < 0.f)
			m_ScrollPos = (float)m_Size.y;
	}
	else
	{
		if (m_ScrollPos > (float)m_Size.y)
			m_ScrollPos = 0.f;
	}


	if (m_AdjustSpeed != 0.f)
	{
		float Adjust = m_MaxSpeed * DeltaTime;
		if (m_AdjustSpeed < 0.f)
		{
			m_ScrollingSpeed -= Adjust;
			m_AdjustSpeed += Adjust;
		}
		else if (m_AdjustSpeed > 0.f)
		{
			m_ScrollingSpeed += Adjust;
			m_AdjustSpeed -= Adjust;
		}

		if (fabs(m_AdjustSpeed) < fabs(Adjust) * 2.f)
		{
			m_AdjustSpeed = 0.f;
		}
	}

	

}

void CBackgroundSpace::Render(HDC hDC, float DeltaTime)
{
	if (!m_Start)
		Start();

	if (m_Texture)
	{

		//출력할 이미지의 '부분' 계산
		int Top = (int)m_ScrollPos % (int)m_Size.y;
		int Bottom = (int)(m_ScrollPos + ORIGINAL_GAME_RES_HEIGHT) % (int)m_Size.y;
		int Split = (int)ORIGINAL_GAME_RES_HEIGHT - Bottom;


		//카메라가 이미지 밖으로 나가지 않았을 경우는 한번만 출력
		if (Split < 0)
		{
			if (m_Texture->GetColorKeyEnable())
			{
				TransparentBlt(hDC,
					0, 0,
					(int)ORIGINAL_GAME_RES_WIDTH, Bottom,
					m_Texture->GetDC(),
					0, Top,
					(int)ORIGINAL_GAME_RES_WIDTH, Bottom,
					m_Texture->GetColorKey());

			}
			else
			{
				StretchBlt(hDC,
					0, 0,
					(int)ORIGINAL_GAME_RES_WIDTH, Bottom,
					m_Texture->GetDC(),
					0, Top,
					(int)ORIGINAL_GAME_RES_WIDTH, Bottom,
					SRCCOPY);
			}
		}

		//카메라가 이미지 밖으로 나갔을 경우는 잘린 부분만큼 위에서 뜯어와서
		//2번 출력
		else
		{
			if (m_Texture->GetColorKeyEnable())
			{
				TransparentBlt(hDC,
					0, 0,
					(int)ORIGINAL_GAME_RES_WIDTH, Split,
					m_Texture->GetDC(),
					0, Top,
					(int)ORIGINAL_GAME_RES_WIDTH, Split,
					m_Texture->GetColorKey());

				TransparentBlt(hDC,
					0, Split,
					(int)ORIGINAL_GAME_RES_WIDTH, Bottom,
					m_Texture->GetDC(),
					0, 0,
					(int)ORIGINAL_GAME_RES_WIDTH, Bottom,
					m_Texture->GetColorKey());
			}
			else
			{
				StretchBlt(hDC,
					0, 0,
					(int)ORIGINAL_GAME_RES_WIDTH, Split,
					m_Texture->GetDC(),
					0, Top,
					(int)ORIGINAL_GAME_RES_WIDTH, Split,
					SRCCOPY);

				StretchBlt(hDC,
					0, Split,
					(int)ORIGINAL_GAME_RES_WIDTH, Bottom,
					m_Texture->GetDC(),
					0, 0,
					(int)ORIGINAL_GAME_RES_WIDTH, Bottom,
					SRCCOPY);
			}
		}
	}
}

void CBackgroundSpace::SetSpeed(float ScrollingSpeed)
{
	m_ScrollingSpeed = ScrollingSpeed;
}

void CBackgroundSpace::SetSpeedSoft(float AdjustSpeed, float SpeedPerSec)
{
	m_AdjustSpeed = AdjustSpeed;
	m_MaxSpeed = SpeedPerSec;
}


