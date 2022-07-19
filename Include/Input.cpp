#include "Input.h"

//Ŀ�� �ִϸ��̼� �����
#include "GameObject/GameObject.h"

//�ؽ�ó �ε��
#include "Resource/ResourceManager.h"

//���� ������ ������� Ȯ�ο�
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
			//BindKey�� �����ϱ� �� �� ���� VecFunction �迭�� �����Ҵ��� ���� �����Ѵ�.
			//������ �迭�̹Ƿ� �˸°� �������ش�.
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


	//���� ��忡�� ���. ���콺
	AddBindKey("MouseLButton", VK_LBUTTON);
	AddBindKey("MouseRButton", VK_RBUTTON);



	//������ ���̾�α� ȣ�� ����Ű Ctrl + T
	AddBindKey("OpenEditor", 'T');
	SetKeyCtrl("OpenEditor");


	/////////////���� �ʱ�ȭ/////////
	m_Ctrl = false;
	m_Alt = false;
	m_Shift = false;


	////Ŀ���� �ִϸ��̼��� ������ ���ӿ�����Ʈ Ŭ���� ����
	//m_CursorAnim = new CGameObject;
	//m_CursorAnim->SetOwnerScene(nullptr);
	//m_CursorAnim->SetName("CursorAnim");
	//if (!m_CursorAnim->Init())
	//{
	//	SAFE_DELETE(m_CursorAnim);
	//	return false;
	//}
	//
	////�ؽ�ó �ε� �� �ִϸ��̼�ȭ
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
	//���콺 ��ġ ������Ʈ
	POINT PTMouse;
	GetCursorPos(&PTMouse);
	ScreenToClient(m_hWnd, &PTMouse);

	//ȭ���� n�� Ȯ�������Ƿ� 
	//��ǥ���� n�� �ٿ���� ���� ���콺 ��ġ�� ��ġ�ϰ� ������.

	PTMouse.x /= SCREEN_SCALE;
	PTMouse.y /= SCREEN_SCALE;

	//�̵��� ���س���
	m_MouseMove.x = PTMouse.x - m_MousePos.x;
	m_MouseMove.y = PTMouse.y - m_MousePos.y;

	//���콺 ��ǥ ������Ʈ
	m_MousePos.x = (float)PTMouse.x;
	m_MousePos.y = (float)PTMouse.y;

	RECT ClientRC;
	GetClientRect(m_hWnd, &ClientRC);
	


	//����Ʈ ����϶��� ���콺 ������ ǥ���Ѵ�.
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




		//���콺 �ִϸ��̼� ����
		////Ŀ���� â �ȿ� �ִµ� Ŀ���� ���̵��� �����Ǿ��ִٸ� false�� �ٲ���
		//if (isCursorInside && m_ShowDefaultCursor)
		//{
		//	m_ShowDefaultCursor = false;
		//	ShowCursor(FALSE);
		//}

		////Ŀ���� â �ۿ� �ִµ� Ŀ���� �Ⱥ��̵��� �����Ǿ��ִٸ� true�� �ٲ���
		//else if (!isCursorInside && !m_ShowDefaultCursor)
		//{
		//	m_ShowDefaultCursor = true;
		//	ShowCursor(TRUE);
		//}
	}



	//m_CursorAnim->SetPos(GetMouseWorldPos());



	//���콺 Ŭ�� ������Ʈ

	//���� ���콺�� ���ȴµ�
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		//Down�� �ƴϰ� Push�� �ƴϸ� ó������ ������
		if (!m_MouseLDown && !m_MouseLPush)
		{
			m_MouseLDown = true;
			m_MouseLPush = true;
		}
		//���� �ϳ��� false�̸� ���� ������ ������ �ִٴ� ����
		else
		{
			m_MouseLDown = false;
		}
	}

	//���콺�� �ȴ��ȴµ� Push�� true�̴� -> ��� ����
	else if (m_MouseLPush)
	{
		m_MouseLDown = false;
		m_MouseLPush = false;
		m_MouseLUp = true;
	}

	//���콺�� �ȴ��ȴµ� m_MouseUP�� true�̴� -> ��ư �ô�
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

			//��� Ű�� ���ȴµ�
			//��� �� �����ӿ��� �����ų� ������ ���� �ʾҴ�?
			//�׷� ���� ������
			if (!State->Down && !State->Push)
			{
				State->Down = true;
				State->Push = true;
			}

			//��� Ű�� ���ȴµ�
			//Down Push �� �� �ϳ��� ���̴�?
			//���� ��� ������ ���̶�� ��!
			//Push�� ó�� ������ �� true�� �ٲ� �־����Ƿ�
			//Down�� false�� �ٲ��ָ� �ȴ�.
			else
			{
				State->Down = false;
			}

		}
		else
		{
			KeyState* State = iter->second;

			//�� �����ӱ��� ��� ������, ������ �־���
			// �ƹ�ư ������ �־����� Push�� ��.��.�� ���̴�
			//�ٵ� ���� �����ӿ��� ������ �ʾҴ�?(else ����)
			//�׷� ���� �� ���ų�?
			//Up�� true ��
			if (State->Push)
			{
				State->Down = false;
				State->Push = false;
				State->Up = true;
			}


			//�̹� �����ӿ��� �� �����µ�
			//�� �����ӿ��� ������?
			//Up�� �ٽ� ����ġ��(Up�� ��� �� �� �����Ӹ� �����Ѵ�)
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

		

		//Down�� ���Ȱ�, ��� Ű���� �Էµ� ���ǰ� ��� ������ ��
		if (bind->BindKeyState->Down &&
			bind->Ctrl == m_Ctrl &&
			bind->Alt == m_Alt &&
			bind->Shift == m_Shift)
		{
			size_t size = bind->VecFunction[(int)EInput_Type::Down].size();

			for(size_t i = 0; i < size; ++i)
			{ 
				//Down�� ���� ����� �Լ��� ���� �����Ѵ�.
				//������ ���� �ȵǴϱ� ��� �������� �׳� �Ѿ��
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
				//Down�� ���� ����� �Լ��� ���� �����Ѵ�.
				//������ ���� �ȵǴϱ� ��� �������� �׳� �Ѿ��
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
				//Down�� ���� ����� �Լ��� ���� �����Ѵ�.
				//������ ���� �ȵǴϱ� ��� �������� �׳� �Ѿ��
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
