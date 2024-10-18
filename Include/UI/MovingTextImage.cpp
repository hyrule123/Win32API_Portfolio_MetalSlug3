#include "MovingTextImage.h"

//텍스처 로드용
#include "../Resource/ResourceManager.h"
#include "../Resource/Texture/Texture.h"
#include "../Resource/Animation/AnimationSequence.h"


#include "WidgetWindow.h"


CMovingTextImage::CMovingTextImage():
	m_MoveToOn(),
	m_MoveToDest(),
	m_Dir(),
	m_MaxSpeed()
{
}

CMovingTextImage::CMovingTextImage(const CMovingTextImage& widget) :
	CWidgetTextImage(widget),
	m_MoveToOn(widget.m_MoveToOn),
	m_MoveToDest(widget.m_MoveToDest),
	m_Dir(widget.m_Dir),
	m_MaxSpeed(widget.m_MaxSpeed)
{
}

CMovingTextImage::~CMovingTextImage()
{
}

bool CMovingTextImage::Init()
{
	return true;
}

void CMovingTextImage::Update(float DeltaTime)
{
	CWidgetTextImage::Update(DeltaTime);

	if (m_MoveToOn)
	{
		Vector2 CurrDir = (m_MoveToDest - m_Pos).Normalize();

		m_Dir = CurrDir;

		MoveDir(DeltaTime);

		//목표 위치에서 현재 위치를 뺀 거리가 최대 이동 가능거리보다 작을 경우 이동 종료
		Vector2 Sub = m_MoveToDest - m_Pos;
		if (hypotf(Sub.x, Sub.y) < m_MaxSpeed * DeltaTime)
		{
			m_MoveToOn = false;
			m_Pos = m_MoveToDest;
		}
	}
}

void CMovingTextImage::MoveDir(float DeltaTime)
{
	m_Pos += m_Dir * m_MaxSpeed* DeltaTime;
}

void CMovingTextImage::SetEssential(float MaxSpeed, const Vector2& Dest)
{
	m_MoveToOn = true;
	m_MaxSpeed = MaxSpeed;
	m_MoveToDest = Dest;
}

bool CMovingTextImage::GetMoveDone() const
{
	return !m_MoveToOn;
}


