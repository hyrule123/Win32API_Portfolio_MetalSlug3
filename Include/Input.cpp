#include "Input.h"

//커서 애니메이션 저장용
#include "GameObject/GameObject.h"

//텍스처 로드용
#include "Resource/ResourceManager.h"

//현재 에디터 모드인지 확인용
#include "GameManager.h"

DEFINITION_SINGLETON(CInput)

CInput::CInput() :
	m_MouseLDown(false),
	m_MouseLPush(false),
	m_MouseLUp(false),
	m_Ctrl(false),
	m_Alt(false),
	m_Shift(false),
	m_ShowDefaultCursor(false),
	m_CursorShowTimeSet(3.f)
	//m_CursorAnim(nullptr)
{
	m_CursorShowTimeLeft = m_CursorShowTimeSet;
}

CInput::~CInput()
{
	{
		std::unordered_map<unsigned char, KeyState*>::iterator iter = m_mapKeyState.begin();
		std::unordered_map<unsigned char, KeyState*>::iterator iterEnd = m_mapKeyState.end();

		while (iter != iterEnd)
		{
			SAFE_DELETE(iter->second);

			++iter;
		}

		m_mapKeyState.clear();
	}

	{
		std::unordered_map<std::string, BindKey*>::iterator iter = m_mapBindKey.begin();
		std::unordered_map<std::string, BindKey*>::iterator iterEnd = m_mapBindKey.end();

		while (iter != iterEnd)
		{
			//BindKey를 제거하기 전 그 안의 VecFunction 배열의 동적할당을 먼저 제거한다.
			//이차원 배열이므로 알맞게 삭제해준다.
			for (size_t i = 0; i < (int)EInput_Type::End; ++i)
			{
				size_t size = iter->second->VecFunction[i].size();
				for (size_t j = 0; j < size; ++j)
				{
					SAFE_DELETE(iter->second->VecFunction[i][j]);
				}

			}

			SAFE_DELETE(iter->second);

			++iter;
		}

		m_mapBindKey.clear();
	}
}

bool CInput::Init(HWND hWnd)
{
	m_hWnd = hWnd;


	AddBindKey("MoveUp", VK_UP);
	AddBindKey("MoveDown", VK_DOWN);
	AddBindKey("MoveLeft", VK_LEFT);
	AddBindKey("MoveRight", VK_RIGHT);
	AddBindKey("Bomb", 'Z');
	AddBindKey("GunFire", 'X');
	AddBindKey("Jump", 'C');
	AddBindKey("SelfDestruct", 'V');
	AddBindKey("PlayGame", '1');

	AddBindKey("Debug3", '3');
	AddBindKey("Debug4", '4');
	AddBindKey("Debug5", '5');
	AddBindKey("Debug6", '6');
	AddBindKey("Debug7", '7');
	AddBindKey("Debug8", '8');
	AddBindKey("Debug9", '9');
	AddBindKey("Debug0", '0');
	
	AddBindKey("DebugRenderToggle", 'G');
	SetKeyCtrl("DebugRenderToggle");
	AddBindKey("EditMode", 'E');
	SetKeyCtrl("EditMode");
	AddBindKey("TimeScaleUp", VK_OEM_PLUS);
	SetKeyCtrl("TimeScaleUp");
	AddBindKey("TimeScaleDown", VK_OEM_MINUS);
	SetKeyCtrl("TimeScaleDown");


	//에딧 모드에서 사용. 마우스
	AddBindKey("MouseLButton", VK_LBUTTON);
	AddBindKey("MouseRButton", VK_RBUTTON);



	//에딧씬 다이얼로그 호출 단축키 Ctrl + T
	AddBindKey("OpenEditor", 'T');
	SetKeyCtrl("OpenEditor");


	/////////////변수 초기화/////////
	m_Ctrl = false;
	m_Alt = false;
	m_Shift = false;


	////커서용 애니메이션을 저장한 게임오브젝트 클래스 생성
	//m_CursorAnim = new CGameObject;
	//m_CursorAnim->SetOwnerScene(nullptr);
	//m_CursorAnim->SetName("CursorAnim");
	//if (!m_CursorAnim->Init())
	//{
	//	SAFE_DELETE(m_CursorAnim);
	//	return false;
	//}
	//
	////텍스처 로드 및 애니메이션화
	//CResourceManager::GetInst()->LoadTextureByTextureNumbers
	//(
	//	"Cursor",
	//	TEXT("Mouse/"),
	//	10
	//);
	//CResourceManager::GetInst()->SetColorKey("Cursor");
	//CResourceManager::GetInst()->CreateAnimationSequence("Cursor", "Cursor");
	//CResourceManager::GetInst()->AddAnimationFrameFrame("Cursor");
	//m_CursorAnim->CreateAnimation();
	//m_CursorAnim->AddAnimationInfo("Cursor", 0.2f, ELoopFlag::Loop);
	//m_CursorAnim->SetPivot(0.f, 0.f);

	//ShowCursor(FALSE);

	return true;
}

void CInput::Update(float DeltaTime)
{
	UpdateMouse(DeltaTime);
	UpdateKeyState(DeltaTime);
	UpdateBindKey(DeltaTime);

	//m_CursorAnim->Update(DeltaTime);	
}

void CInput::PostUpdate(float DeltaTime)
{
	//m_CursorAnim->PostUpdate(DeltaTime);
}

void CInput::Render(HDC hDC, float DeltaTime)
{
	//m_CursorAnim->Render(hDC, DeltaTime);
}

const Vector2& CInput::GetMousePos() const
{
	return m_MousePos;
}

const Vector2& CInput::GetMouseWorldPos() const
{
	return m_MouseWorldPos;
}

const Vector2& CInput::GetMouseMove() const
{
	return m_MouseMove;
}

bool CInput::GetMouseLDown() const
{
	return m_MouseLDown;
}

bool CInput::GetMouseLPush() const
{
	return m_MouseLPush;
}

bool CInput::GetMouseLUp() const
{
	return m_MouseLUp;
}

void CInput::ComputeWorldPos(const Vector2& CamPos)
{
	m_MouseWorldPos = m_MousePos + CamPos;
}

void CInput::UpdateMouse(float DeltaTime)
{
	//마우스 위치 업데이트
	POINT PTMouse;
	GetCursorPos(&PTMouse);
	ScreenToClient(m_hWnd, &PTMouse);

	//화면을 n배 확대했으므로 
	//좌표값은 n배 줄여줘야 실제 마우스 위치와 일치하게 움직임.

	PTMouse.x /= SCREEN_SCALE;
	PTMouse.y /= SCREEN_SCALE;

	//이동량 구해놓고
	m_MouseMove.x = PTMouse.x - m_MousePos.x;
	m_MouseMove.y = PTMouse.y - m_MousePos.y;

	//마우스 좌표 업데이트
	m_MousePos.x = (float)PTMouse.x;
	m_MousePos.y = (float)PTMouse.y;

	RECT ClientRC;
	GetClientRect(m_hWnd, &ClientRC);
	


	//에디트 모드일때는 마우스 무조건 표시한다.
	if (CGameManager::GetInst()->GetEditMode())
	{
		m_ShowDefaultCursor = true;
		ShowCursor(TRUE);
	}
	else
	{
		bool isCursorInside = true;
		do {
			if (PTMouse.x < ClientRC.left)
			{
				isCursorInside = false;
				break;
			}
			if (PTMouse.x > ClientRC.right)
			{
				isCursorInside = false;
				break;
			}
			if (PTMouse.y < ClientRC.top)
			{
				isCursorInside = false;
				break;
			}
			if (PTMouse.y > ClientRC.bottom)
			{
				isCursorInside = false;
				break;
			}
		} while (false);

		if (isCursorInside)
		{
			if (!m_ShowDefaultCursor)
			{
				if (fabs(m_MouseMove.x) > 1.f || fabs(m_MouseMove.y) > 1.f)
				{
					m_CursorShowTimeLeft = m_CursorShowTimeSet;
					m_ShowDefaultCursor = true;
					ShowCursor(TRUE);
				}
			}
			else
			{
				if (m_CursorShowTimeLeft > 0.f)
				{
					m_CursorShowTimeLeft -= DeltaTime;
				}
				else
				{
					m_ShowDefaultCursor = false;
					ShowCursor(FALSE);
				}
			}
		}




		//마우스 애니메이션 관련
		////커서가 창 안에 있는데 커서가 보이도록 설정되어있다면 false로 바꿔줌
		//if (isCursorInside && m_ShowDefaultCursor)
		//{
		//	m_ShowDefaultCursor = false;
		//	ShowCursor(FALSE);
		//}

		////커서가 창 밖에 있는데 커서가 안보이도록 설정되어있다면 true로 바꿔줌
		//else if (!isCursorInside && !m_ShowDefaultCursor)
		//{
		//	m_ShowDefaultCursor = true;
		//	ShowCursor(TRUE);
		//}
	}



	//m_CursorAnim->SetPos(GetMouseWorldPos());



	//마우스 클릭 업데이트

	//만약 마우스가 눌렸는데
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		//Down도 아니고 Push도 아니면 처음으로 눌린것
		if (!m_MouseLDown && !m_MouseLPush)
		{
			m_MouseLDown = true;
			m_MouseLPush = true;
		}
		//둘중 하나라도 false이면 현재 무조건 누르고 있다는 뜻임
		else
		{
			m_MouseLDown = false;
		}
	}

	//마우스가 안눌렸는데 Push는 true이다 -> 방금 뗀것
	else if (m_MouseLPush)
	{
		m_MouseLDown = false;
		m_MouseLPush = false;
		m_MouseLUp = true;
	}

	//마우스가 안눌렸는데 m_MouseUP이 true이다 -> 버튼 뗐다
	else if(m_MouseLUp)
	{
		m_MouseLUp = false;
	}
}

void CInput::UpdateKeyState(float DeltaTime)
{
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		m_Ctrl = true;
	else
		m_Ctrl = false;
	if (GetAsyncKeyState(VK_MENU) & 0x8000)
		m_Alt = true;
	else
		m_Alt = false;
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		m_Shift = true;
	else
		m_Shift = false;

	std::unordered_map<unsigned char, KeyState*>::iterator iter = m_mapKeyState.begin();
	std::unordered_map<unsigned char, KeyState*>::iterator iterEnd = m_mapKeyState.end();

	while (iter != iterEnd)
	{
		bool isDown = false;

		if (GetAsyncKeyState(iter->first) & 0x8000)
		{
			isDown = true;
		}

		if (isDown)
		{
			KeyState* State = iter->second;

			//방금 키가 눌렸는데
			//방금 전 프레임에서 눌리거나 누르고 있지 않았다?
			//그럼 새로 눌린거
			if (!State->Down && !State->Push)
			{
				State->Down = true;
				State->Push = true;
			}

			//방금 키가 눌렸는데
			//Down Push 둘 중 하나라도 참이다?
			//지금 계속 누르는 중이라는 뜻!
			//Push는 처음 눌렀을 떄 true로 바꿔 주었으므로
			//Down만 false로 바꿔주면 된다.
			else
			{
				State->Down = false;
			}

		}
		else
		{
			KeyState* State = iter->second;

			//전 프레임까지 방금 눌렀던, 누르고 있었던
			// 아무튼 누르고 있었으면 Push는 무.조.건 참이다
			//근데 지금 프레임에서 누르지 않았다?(else 진입)
			//그럼 지금 막 뗀거네?
			//Up을 true 로
			if (State->Push)
			{
				State->Down = false;
				State->Push = false;
				State->Up = true;
			}


			//이번 프레임에서 안 눌렀는데
			//전 프레임에서 눌렀다?
			//Up를 다시 원위치로(Up는 방금 뗀 한 프레임만 동작한다)
			else if (State->Up)
			{
				State->Up = false;
			}
		}

		++iter;
	}
}



void CInput::UpdateBindKey(float DeltaTime)
{
	std::unordered_map<std::string, BindKey*>::iterator iter = m_mapBindKey.begin();
	std::unordered_map<std::string, BindKey*>::iterator iterEnd = m_mapBindKey.end();

	while (iter != iterEnd)
	{
		BindKey* bind = iter->second;

		

		//Down이 눌렸고, 기능 키들의 입력도 조건과 모두 동일할 때
		if (bind->BindKeyState->Down &&
			bind->Ctrl == m_Ctrl &&
			bind->Alt == m_Alt &&
			bind->Shift == m_Shift)
		{
			size_t size = bind->VecFunction[(int)EInput_Type::Down].size();

			for(size_t i = 0; i < size; ++i)
			{ 
				//Down시 실행 등록한 함수를 전부 실행한다.
				//없으면 실행 안되니까 등록 안했으면 그냥 넘어가짐
				bind->VecFunction[(int)EInput_Type::Down][i]->func();
			}
			
		}



		if (bind->BindKeyState->Push &&
			bind->Ctrl == m_Ctrl &&
			bind->Alt == m_Alt &&
			bind->Shift == m_Shift)
		{
			size_t size = bind->VecFunction[(int)EInput_Type::Push].size();

			for (size_t i = 0; i < size; ++i)
			{
				//Down시 실행 등록한 함수를 전부 실행한다.
				//없으면 실행 안되니까 등록 안했으면 그냥 넘어가짐
				bind->VecFunction[(int)EInput_Type::Push][i]->func();
			}

		}


		if (bind->BindKeyState->Up &&
			bind->Ctrl == m_Ctrl &&
			bind->Alt == m_Alt &&
			bind->Shift == m_Shift)
		{
			size_t size = bind->VecFunction[(int)EInput_Type::Up].size();

			for (size_t i = 0; i < size; ++i)
			{
				//Down시 실행 등록한 함수를 전부 실행한다.
				//없으면 실행 안되니까 등록 안했으면 그냥 넘어가짐
				bind->VecFunction[(int)EInput_Type::Up][i]->func();
			}

		}



		++iter;
	}
}

void CInput::SetKeyCtrl(const std::string& Name, bool ctrl)
{
	BindKey* bind = FindBindKey(Name);

	if (!bind)
		return;

	bind->Ctrl = ctrl;
}

void CInput::SetKeyAlt(const std::string& Name, bool alt)
{
	BindKey* bind = FindBindKey(Name);

	if (!bind)
		return;

	bind->Alt = alt;
}

void CInput::SetKeyShift(const std::string& Name, bool shift)
{
	BindKey* bind = FindBindKey(Name);

	if (!bind)
		return;

	bind->Shift = shift;
}

KeyState* CInput::FindKeyState(unsigned char key)
{
	std::unordered_map<unsigned char, KeyState*>::iterator iter = m_mapKeyState.find(key);

	if(iter == m_mapKeyState.end())
		return nullptr;

	return iter->second;
}

BindKey* CInput::FindBindKey(const std::string Name)
{
	std::unordered_map<std::string, BindKey*>::iterator iter = m_mapBindKey.find(Name);

	if (iter == m_mapBindKey.end())
		return nullptr;

	return iter->second;
}

bool CInput::AddBindKey(const std::string& Name, unsigned char key)
{
	if (FindBindKey(Name))
		return false;

	BindKey* NewKeyBind = new BindKey;

	KeyState* State = FindKeyState(key);

	if (!State)
	{
		State = new KeyState;
		State->Key = key;

		m_mapKeyState.insert(std::make_pair(key, State));
	}

	NewKeyBind->BindKeyState = State;
	NewKeyBind->Name = Name;

	m_mapBindKey.insert(std::make_pair(Name, NewKeyBind));
	

	return true;
}

void CInput::ClearCallback()
{
	auto	iter = m_mapBindKey.begin();
	auto	iterEnd = m_mapBindKey.end();

	for (; iter != iterEnd; ++iter)
	{
		for (int i = 0; i < (int)EInput_Type::End; ++i)
		{
			size_t	Size = iter->second->VecFunction[i].size();

			for (size_t j = 0; j < Size; ++j)
			{
				SAFE_DELETE(iter->second->VecFunction[i][j]);
			}

			iter->second->VecFunction[i].clear();
		}
	}
}
