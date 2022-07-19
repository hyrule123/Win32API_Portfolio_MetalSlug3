#include "Camera.h"

#include "../GameObject/GameObject.h"

#include "../Input.h"

#include "../GameManager.h"

CCamera::CCamera():
	m_Target(nullptr)
{
	m_Resolution.x = (float)ORIGINAL_GAME_RES_WIDTH;
	m_Resolution.y = (float)ORIGINAL_GAME_RES_HEIGHT;
	m_WorldResolution = m_Resolution;
}

CCamera::~CCamera()
{
}

void CCamera::Update(float DeltaTime)
{
	if (m_Target)
	{
		if (!m_Target->GetActive())
		{
			m_Target = nullptr;
		}
		else
		{
			//카메라의 좌측 상단 위치(그리기 시작할 곳)
			m_Pos = m_Target->GetPos() - m_TargetPivot * m_Resolution + m_TargetOffset;
		}

	}

	if (m_Pos.x < 0.f)
	{
		m_Pos.x = 0.f;
	}
	else if (m_Pos.x + m_Resolution.x > m_WorldResolution.x)
	{
		m_Pos.x = m_WorldResolution.x - m_Resolution.x;
	}

	if (m_Pos.y < 0.f)
	{
		m_Pos.y = 0.f;
	}
	else if (m_Pos.y + m_Resolution.y > m_WorldResolution.y)
	{
		m_Pos.y = m_WorldResolution.y - m_Resolution.y;
	}

	//카메라 위치 업데이트 완료되었으므로 마우스 인게임 위치 업데이트
	CInput::GetInst()->ComputeWorldPos(m_Pos);
}

void CCamera::SetPos(const Vector2& Pos)
{
	m_Pos = Pos;
}

void CCamera::SetPos(float _x, float _y)
{
	m_Pos.x = _x;
	m_Pos.y = _y;
}

void CCamera::AddPos(const Vector2& Pos)
{
	m_Pos += Pos;
}

void CCamera::SetRes(const Vector2& Res)
{
	m_Resolution = Res;
}

void CCamera::SetRes(float _x, float _y)
{
	m_Resolution.x = _x;
	m_Resolution.y = _y;
}

void CCamera::SetWorldRes(const Vector2& WorldRes)
{
	m_WorldResolution = WorldRes;
}

void CCamera::SetWorldRes(float _x, float _y)
{
	m_WorldResolution.x = _x;
	m_WorldResolution.y = _y;
}

void CCamera::SetTargetObj(CGameObject* Target)
{
	m_Target = Target;
}

void CCamera::SetTargetPivot(const Vector2& Pivot)
{
	m_TargetPivot = Pivot;
}

void CCamera::SetTargetPivot(float _x, float _y)
{
	m_TargetPivot.x = _x;
	m_TargetPivot.y = _y;
}

void CCamera::SetTargetOffset(const Vector2& Offset)
{
	m_TargetOffset = Offset;
}

void CCamera::SetTargetOffset(float _x, float _y)
{
	m_TargetOffset.x = _x;
	m_TargetOffset.y = _y;
}

Vector2 CCamera::GetPos() const
{
	return m_Pos;
 }

Vector2 CCamera::GetRes() const
{
	return m_Resolution;
}

Vector2 CCamera::GetWorldRes() const
{
	return m_WorldResolution;
}

CGameObject* CCamera::GetTargetObj() const
{
	return m_Target;
}

Vector2 CCamera::GetTargetPivot() const
{
	return m_TargetPivot;
}

Vector2 CCamera::GetTargetOffset() const
{
	return m_TargetOffset;
}
