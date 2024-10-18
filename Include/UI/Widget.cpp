#include "Widget.h"

CWidget::CWidget() :
	m_Scene(nullptr),
	m_Owner(nullptr),
	m_ZOrder(0),
	m_MouseHovered(false),
	m_Size(100.f, 100.f),
	m_Offset(),
	m_SkipCollisionCheck(),
	m_Start()
{
}

CWidget::CWidget(const CWidget& widget):
	CRef(widget),
	m_ZOrder(widget.m_ZOrder),
	m_MouseHovered(widget.m_MouseHovered),
	m_Offset(widget.m_Offset),
	m_SkipCollisionCheck(widget.m_SkipCollisionCheck),
	m_Start()
{
}

CWidget::~CWidget()
{
}

bool CWidget::Init()
{
	return true;
}

void CWidget::Start()
{
	m_Start = true;
}

int CWidget::GetZOrder() const
{
	return m_ZOrder;
}

void CWidget::SetZOrder(int ZOrder)
{
	m_ZOrder = ZOrder;
}


const Vector2& CWidget::GetPos() const
{
	return m_Pos;
}

void CWidget::SetPos(float _x, float _y)
{
	m_Pos.x = _x;
	m_Pos.y = _y;
}

void CWidget::SetPos(const Vector2& Pos)
{
	m_Pos = Pos;
}

void CWidget::SetOffset(float _x, float _y)
{
	m_Offset.x = _x;
	m_Offset.y = _y;
}

void CWidget::SetSize(float _x, float _y)
{
	m_Size.x = _x;
	m_Size.y = _y;
}

void CWidget::SetSize(const Vector2& Size)
{
	m_Size = Size;
}

const Vector2& CWidget::GetSize() const
{
	return m_Size;
}

void CWidget::SetSkipCollisionCheck(bool SkipCollisionCheck)
{
	m_SkipCollisionCheck = SkipCollisionCheck;
}

bool CWidget::CollisionMouse(const Vector2& Pos)
{
	Vector2 ButtonLT = m_Pos;

	if (m_SkipCollisionCheck)
		return false;

	if (Pos.x < ButtonLT.x)
		return false;
	if (Pos.x > (ButtonLT.x + m_Size.x))
		return false;
	if (Pos.y < ButtonLT.y)
		return false;
	if (Pos.y > (ButtonLT.y + m_Size.y))
		return false;

	//첫 충돌이라면 콜백함수를 호출하고 true 리턴
	if (!m_MouseHovered)
		CollisionMouseHoveredCallback(Pos);

	
	return true;
}

void CWidget::CollisionMouseHoveredCallback(const Vector2& Pos)
{
	m_MouseHovered = true;
}

void CWidget::CollisionMouseReleaseCallback()
{
	m_MouseHovered = false;
}

void CWidget::Update(float DeltaTime)
{
	if (!m_Start)
		Start();
}

void CWidget::PostUpdate(float DeltaTime)
{
	if (!m_Start)
		Start();
}

void CWidget::Render(HDC hDC, float DeltaTime)
{
	if (!m_Start)
		Start();
}

void CWidget::Render(HDC hDC, const Vector2& Pos, float DeltaTime)
{
	if (!m_Start)
		Start();
}
