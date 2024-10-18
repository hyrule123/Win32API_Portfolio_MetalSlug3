#include "WidgetButton.h"


//자신이 들어갈 위젯윈도우 저장용
#include "WidgetWindow.h"


//리소스 로딩용
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

//리소스
#include "../Resource/Sound/Sound.h"
#include "../Resource/Texture/Texture.h"

//텍스처 없을 경우 도형 그리기용(hPen, hBrush)
#include "../GameManager.h"

//마우스 입력 받아오기
#include "../Input.h"


CWidgetButton::CWidgetButton() :
	m_Texture(nullptr),
	m_SoundState{},
	m_ButtonStateData{},
	m_ButtonState(EButtonState::Normal)
{
}

CWidgetButton::CWidgetButton(const CWidgetButton& widget):
	CWidget(widget)
{
}

CWidgetButton::~CWidgetButton()
{
}

bool CWidgetButton::Init()
{
	return true;
}


//텍스처 관련, 텍스처를 로드한 뒤 사용할것.
bool CWidgetButton::SetTexture(const std::string& Name)
{
	CTexture* Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (!Texture)
		return false;

	m_Texture = Texture;
	return true;
}
void CWidgetButton::SetButtonStateData(EButtonState State, const Vector2& Start, const Vector2& End)
{
	m_ButtonStateData[(int)State].Start = Start;
	m_ButtonStateData[(int)State].End = End;
}
void CWidgetButton::EnableButton(bool Enable)
{
	m_ButtonState = Enable ? EButtonState::Normal : EButtonState::Disabled;
}
//=================================================================================


//사운드 관련. 사운드를 로드한뒤 사용할것
bool CWidgetButton::SetSound(EButtonSoundState State, const std::string& Name)
{
	CSound* Sound = m_Scene->GetSceneResource()->FindSound(Name);

	if (!Sound)
		return false;

	m_SoundState[(int)State] = Sound;
	return true;
}
//========================================================================



void CWidgetButton::Update(float DeltaTime)
{
	//버튼의 사이즈 = 현재 버튼의 상태에 해당하는 이미지의 크기.
	if(m_Texture)
	{ 
		//m_Size = m_ButtonStateData[(int)m_ButtonState].End - m_ButtonStateData[(int)m_ButtonState].Start;
	}

	//버튼 상태가 Disabled가 아닐 경우에만 처리한다.
	if (m_ButtonState != EButtonState::Disabled)
	{
		//인풋 클래스 주소를 미리 받아놓음
		CInput* Input = CInput::GetInst();

		//마우스가 해당 버튼 위로 올라와있을 때
		if (m_MouseHovered)
		{
			//만약 버튼 클릭이 있었다면
			if (Input->GetMouseLDown())
			{
				m_ButtonState = EButtonState::Click;
			}
			//만약 계속 누르고 있는 중이라면 클릭상태 유지
			else if (
				m_ButtonState == EButtonState::Click &&
				Input->GetMouseLPush()
				)
			{
				m_ButtonState = EButtonState::Click;
			}
			//클릭 상태에서 버튼이 올라간 게 감지되었다면 콜백함수와 효과음 호출
			else if (
				m_ButtonState == EButtonState::Click &&
				Input->GetMouseLUp()
				)
			{
				if (m_Callback[(int)EButtonSoundState::Click])
					m_Callback[(int)EButtonSoundState::Click]();
				if (m_SoundState[(int)EButtonSoundState::Click])
					m_SoundState[(int)EButtonSoundState::Click]->Play();

				//그리고 기능을 호출했으므로 다시 원상복귀(마우스 올라온상태)
				m_ButtonState = EButtonState::MouseHovered;
			}

			//버튼 클릭이 없으면 그냥 그대로 냅둠
			else
			{
				m_ButtonState = EButtonState::MouseHovered;
			}
			

		}
		//전부 아니면 일반 상태로 복귀
		else
		{
			m_ButtonState = EButtonState::Normal;
		}
	}

	
}

void CWidgetButton::PostUpdate(float DeltaTime)
{
	
}

void CWidgetButton::Render(HDC hDC, float DeltaTime)
{

	if (m_Texture)
	{
		
		Vector2 RenderLT = m_Pos + m_Owner->GetPos();

		int index = (int)m_ButtonState;

		Vector2 TextureSize = m_ButtonStateData[index].End - m_ButtonStateData[index].Start;

		if (m_Texture->GetColorKeyEnable())
		{
			TransparentBlt(hDC,
				(int)RenderLT.x, (int)RenderLT.y,
				(int)m_Size.x, (int)m_Size.y,
				m_Texture->GetDC(), 
				(int)m_ButtonStateData[index].Start.x, (int)m_ButtonStateData[index].Start.y,
				(int)TextureSize.x, (int)TextureSize.y,
				m_Texture->GetColorKey());

		}
		else
		{
			StretchBlt(hDC,
				(int)RenderLT.x, (int)RenderLT.y,
				(int)m_Size.x, (int)m_Size.y,
				m_Texture->GetDC(), 
				(int)m_ButtonStateData[index].Start.x, (int)m_ButtonStateData[index].Start.y,
				(int)TextureSize.x, (int)TextureSize.y,
				SRCCOPY);
		}
	}
	else //텍스처가 없을때
	{
		HBRUSH Brush = CGameManager::GetInst()->GetBrush(EBrushType::Green);

		//마우스가 올라와있으면 빨간색으로 변환
		if (m_ButtonState == EButtonState::MouseHovered)
		{
			Brush = CGameManager::GetInst()->GetBrush(EBrushType::Red);
		}
		//클릭하면 blue
		else if (m_ButtonState == EButtonState::Click)
		{
			Brush = CGameManager::GetInst()->GetBrush(EBrushType::Blue);
		}
		//비활성화 시에는 검정
		else if (m_ButtonState == EButtonState::Disabled)
		{
			Brush = CGameManager::GetInst()->GetBrush(EBrushType::Black);
		}

		HBRUSH PrevBrush = (HBRUSH)SelectObject(hDC, Brush);


		//WidgetWindow의 위치에 더해 표시될 상대 위치를 구한다.
		Vector2 PosLT = m_Pos + m_Owner->GetPos();

		Rectangle(hDC,
			(int)PosLT.x,
			(int)PosLT.y,
			(int)(PosLT.x + m_Size.x),
			(int)(PosLT.y + m_Size.y));

		SelectObject(hDC, PrevBrush);
	}
}

void CWidgetButton::Render(HDC hDC, const Vector2& Pos, float DeltaTime)
{
}

void CWidgetButton::CollisionMouseHoveredCallback(const Vector2& Pos)
{
	CWidget::CollisionMouseHoveredCallback(Pos);

	if (m_SoundState[(int)EButtonSoundState::MouseHovered])
	{
		m_SoundState[(int)EButtonSoundState::MouseHovered]->Play();
	}

}

void CWidgetButton::CollisionMouseReleaseCallback()
{
	CWidget::CollisionMouseReleaseCallback();
}
