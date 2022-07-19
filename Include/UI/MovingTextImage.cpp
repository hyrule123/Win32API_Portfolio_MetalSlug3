#include "MovingTextImage.h"

//�ؽ�ó �ε��
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

		//��ǥ ��ġ���� ���� ��ġ�� �� �Ÿ��� �ִ� �̵� ���ɰŸ����� ���� ��� �̵� ����
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


