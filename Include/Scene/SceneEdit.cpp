#include "SceneEdit.h"

//csv ����
#include "../CSVEditor.h"

//���̾�α� ���� �� ��ŸƮ������ ���ư�. �� ��ȯ ��
#include "SceneManager.h"
#include "StartScene.h"

//ī�޶� �̵�, �ε� �� ī�޶� ��ġ �缳��
#include "Camera.h"

//���ӸŴ����� ������� ������ �˷��ֱ� ����
#include "../GameManager.h"

#include "../Resource/Texture/Texture.h"
#include "../Input.h"

#include "SceneEdit.h"


//������ �ν��Ͻ� �޾ƿ��� �뵵
#include "../GameManager.h"

//��� ������
#include "../PathManager.h"

#include "../Resource/Texture/Texture.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/SceneResource.h"


//���ҽ� ���� ��ȣ �������� ����
#include "../resource.h"
#include "../Resource/Animation/AnimationSequence.h"
#include "../UI/WindowEdit.h"

//�ִϸ��̼� �������� ���ӿ�����Ʈ
#include "../GameObject/EditViewer.h"


CSceneEdit* CSceneEdit::s_Dlg = nullptr;

CSceneEdit::CSceneEdit() :
	m_Hide(false),
	m_SelectFrameIndex(-1),
	m_SelectAnimSeqIndex(-1),
	m_SelectTextureIndex(-1),
	m_SelectPlayIndex(-1),
	m_Viewer(nullptr)

{
	//������ ������� �˷���.
	CGameManager::GetInst()->SetEditMode();

	//���������� �ּҸ� ���
	s_Dlg = this;
}


CSceneEdit::~CSceneEdit()
{
	CGameManager::GetInst()->SetEditMode(false);


	s_Dlg = nullptr;

	for (size_t i = 0; i < m_vecCSVEditor.size(); ++i)
	{
		SAFE_DELETE(m_vecCSVEditor[i]);
	}

	//�� ���� ���õ� �Է¹��ε��� ��� �����Ѵ�.
	CInput::GetInst()->DeleteBindClass<CSceneEdit>(this);
	
}

bool CSceneEdit::Init()
{
	//����Ű��, ȣ���� �ݹ� �޼ҵ� ���.
	CInput::GetInst()->AddBindFunction<CSceneEdit>("OpenEditor",
		EInput_Type::Down, this, &CSceneEdit::ChangeVisibleStatus);

	CreateWidgetWindow<CWindowEdit>("WindowEdit");

	CreateObject<CGameObject>("Model");



	//â ����
	m_hDlg = CreateDialog(CGameManager::GetInst()->GetWindowInstance(), MAKEINTRESOURCE(IDD_DIALOG_ANIMEDIT), CGameManager::GetInst()->GetWindowHandle(), CSceneEdit::WndProc);

	CGameManager* Mgr = CGameManager::GetInst();

	RECT rcmain;
	GetWindowRect(Mgr->GetWindowHandle(), &rcmain);

	RECT rcdlg;
	GetClientRect(m_hDlg, &rcdlg);

	Resolution Res = Mgr->GetWindowSize();
	rcmain.left = rcmain.left + (long)Res.Width - rcdlg.right;
	rcmain.top = rcmain.top + (long)Res.Height - rcdlg.bottom;




	//�������� ��ġ ����. ������� �������� �ʰ�(SWP_NOSIZE) ��ġ��
	SetWindowPos(m_hDlg, 0, rcmain.left, rcmain.top, 0, 0, SWP_NOSIZE);

	//â ���
	ShowWindow(m_hDlg, SW_SHOW);
	m_Hide = false;

	//����Ʈ�� �ڵ��� ���´�.
	m_hTextureListBox = GetDlgItem(m_hDlg, IDC_LIST_TEXTURENAME);
	m_hAnimSeqListBox = GetDlgItem(m_hDlg, IDC_LIST_ANIMSEQ);
	m_hFrameListBox = GetDlgItem(m_hDlg, IDC_LIST_SPRITE_FRAME);
	m_hPlayListBox = GetDlgItem(m_hDlg, IDC_LIST_PLAYLIST);

	//üũ�ڽ� �ڵ� ���´�.
	m_hCheckReverse = GetDlgItem(m_hDlg, IDC_CHECK_REVERSE);


	//�������� ���ӿ�����Ʈ
	m_Viewer = CreateObject<CEditViewer>("Viewer");
	m_Viewer->SetPos(100.f, 100.f);


	//GameObject�� ��ϵ� ����׿� Ű�� ��� �����Ѵ�.
	CInput::GetInst()->DeleteBindClass<CGameManager>(CGameManager::GetInst());



	CInput::GetInst()->AddBindFunction<CSceneEdit>("EditMode", EInput_Type::Down, this, &CSceneEdit::ReturnStartScene);



	return true;
}


bool CSceneEdit::LoadCSV()
{
	//Ǯ��θ� �޾ƿ� �ּ� ������ ������ְ�
	TCHAR FullPath[MAX_PATH] = {};


	//���� ���� â�� ������ ���� ����ü�� �������ش�.
	OPENFILENAME OpenFile = {};


	OpenFile.lStructSize = sizeof(OPENFILENAME);	//����ü ũ��
	OpenFile.hwndOwner = m_hDlg;	//���� �ڵ�
	OpenFile.lpstrFilter = TEXT("*.csv File\0*.csv\0��� ����\0*.*"); 	//���� ����
	OpenFile.lpstrFile = FullPath;	//��ΰ� ����� ���� �ּ�
	OpenFile.nMaxFile = MAX_PATH;	//�ִ� ��� �ڸ���

#ifdef _UNICODE
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path;
#else
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->PathMultiByte;
#endif // _UNICODE

	//������� Ǯ ��θ� FullPath�� ������.
	if (GetOpenFileName(&OpenFile))
	{
		bool Result = LoadCSVFullPath(FullPath);
		if (Result)
		{
			m_SelectTextureIndex = (int)m_vecCSVEditor.size() - 1;
			SendMessage(m_hTextureListBox, LB_SETCURSEL,
				(WPARAM)m_SelectTextureIndex, 0);
		}

		return Result;
	}
	return false;
}


bool CSceneEdit::LoadCSVFullPath(const TCHAR* FullPath, int index)
{
	std::string FullPathA;

#ifdef _UNICODE
	char FullPathATemp[MAX_PATH] = {};
	int FullPathALen = WideCharToMultiByte(CP_ACP, 0, FullPath, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, FullPath, -1, FullPathATemp, FullPathALen, 0, 0);
	FullPathA = FullPathATemp;
#else
	FullPathA = FullPath;
#endif	//_UNICODE


	CCSVEditor* CSVEditor = new CCSVEditor;
	if (!CSVEditor->ReadFile(FullPathA))
	{
		SAFE_DELETE(CSVEditor);
		return false;
	}
	ClearSelect(ERange::AnimSeq);

	//csv�� �ι�°�� ù��° ������ �ؽ�ó �̸��� �����ִ�.
	std::string TexName = CSVEditor->GetCell((int)ERowInfo::TexInfo, 0);

	//ù�� �ι�°�� �ι�° ������ �ִϸ��̼� ���ϸ��� �����ִ�.
	std::tstring FileName;
#ifdef _UNICODE
	FileName.assign(
		CSVEditor->GetCell((int)ERowInfo::TexInfo, 1).begin(),
		CSVEditor->GetCell((int)ERowInfo::TexInfo, 1).end()
	);
#else
	FileName = m_CSVEditor->GetCell((int)ERowInfo::TexInfo, 1);
#endif
	if (GetSceneResource()->LoadTexture(TexName, FileName))
	{
		//�÷�Ű�� ����ִ� ��
		std::string src = CSVEditor->GetCell(1, 2);

		int R = 255;
		int G = 0;
		int B = 255;

		int SlashCount = 0;
		for (size_t k = 0; k < src.size(); ++k)
		{
			if (src[k] == '/')
				++SlashCount;
		}
		//�÷�Ű RGB���� ���������� �ԷµǾ��� ���
		if (SlashCount == 2)
		{
			sscanf_s(src.c_str(), "%d/%d/%d", &R, &G, &B);
		}
		GetSceneResource()->SetColorKey(TexName, R, G, B);

		//�ؽ�ó�� �ε�Ǹ� ���̾�α׿� �ؽ�ó �̸��� ���� �̸��� ����
		SendMessageA(m_hTextureListBox, LB_ADDSTRING, 0, (LPARAM)TexName.c_str());
	}
	else
	{
		SAFE_DELETE(CSVEditor);
		return false;
	}

	//�� ��° �� ù���� ������ ������ �� �ִϸ��̼� ������ �̸��� �����ִ�.
	for (int i = (int)ERowInfo::AnimSeqInfoStart; i < CSVEditor->GetRowCount(); i++)
	{
		std::string AnimName;

		auto& row = CSVEditor->GetRow(i);
		for (int j = 0; j < row.size(); j++)
		{
			std::string src = CSVEditor->GetCell(i, j);

			//���� ��ȣ�� ������� ���Դ����� ī��Ʈ.
			int ParamCount = 0;
			//���� �ƹ� ���뵵 �ԷµǾ����� �ʴٸ� continue
			//AnimInfo ���ڰ����� ������ ���� �� �����Ƿ� �ش� ���׵� üũ
			if (src == "")
			{
				continue;
			}


			//���� ù��° ���� ���� �������� �̸��� ���� �ִ�.
			if (j == (int)EColInfo::AnimSeqName)
			{
				AnimName = src;

				//�ִϸ��̼� ������ ������ �����ϸ� ���̾�α׿� �̸� ����
				if (GetSceneResource()->
					CreateAnimationSequence(AnimName, TexName))
				{
					//����Ʈ�� �ִϸ��̼� ������ �̸��� �߰�
					SendMessageA(m_hAnimSeqListBox, LB_ADDSTRING, 0, (LPARAM)AnimName.c_str());
				}
				else
				{
					SAFE_DELETE(CSVEditor);
					return false;
				}
			}
			else if (j >= (int)EColInfo::FramePosStart)//�ش� ���� ��ǥ �κ� �ε�
			{
				Vector2 Start;
				Vector2 End;
				Vector2 Offset;

				//�������� 4�� �̻��̸� Offset�� ��ϵǾ��ִٴ� ���̹Ƿ� Offset���� �����Ͽ� �����Ѵ�.
				int SlashCount = 0;
				for (size_t k = 0; k < src.size(); ++k)
				{
					if (src[k] == '/')
						++SlashCount;
				}

				//�Է� �ȵ� �κ��� ������ return;
				if (SlashCount < 3)
					break;
				//��������Ʈ ��ǥ���� �ԷµǾ������
				else if (SlashCount == 3)
				{
					sscanf_s(src.c_str(), "%f/%f/%f/%f", &Start.x, &Start.y, &End.x, &End.y);
				}
				//������ ������ �Էµ������
				else if (SlashCount > 3)
				{
					sscanf_s(src.c_str(), "%f/%f/%f/%f/%f/%f",
						&Start.x, &Start.y, &End.x, &End.y, &Offset.x, &Offset.y);
				}


				//������ ������ �߰�
				if (!GetSceneResource()->AddAnimationSpriteFrame(AnimName,
					Start, End, Offset))
				{
					SAFE_DELETE(CSVEditor);
					return false;
				}
			}


		}
	}


	if (index == -1)
	{
		//������� �� ������ CSVEditor �ּҸ� �ʿ� ����
		m_vecCSVEditor.push_back(CSVEditor);
	}
	else if(index >= 0 && index < m_vecCSVEditor.size())
	{
		m_vecCSVEditor[index] = CSVEditor;
	}
	else
	{
		SAFE_DELETE(CSVEditor);
		return false;
	}


	return true;
}

bool CSceneEdit::ReloadCSV(const std::string& PathName)
{
	//��������� return
	if (m_vecCSVEditor.empty())
		return false;

	//����Ʈ �� �����
	ClearSelect(ERange::All);

	//����� �ִϸ��̼ǵ� ��� �����ְ�
	m_Viewer->ResetAnimation();


	//�ε�� CAnimationSeq�� �� ��������
	m_SceneResource->ClearAnimSeq();


	//��θ� �޾ƿ´�.
	std::tstring BasicPath;
#ifdef _UNICODE
	BasicPath = CPathManager::GetInst()->FindPath(PathName)->Path;
#else
	BasicPath = CPathManager::GetInst()->FindPath(PathName)->PathMultiByte;
#endif // _UNICODE

	size_t size = m_vecCSVEditor.size();
	for (size_t i = 0; i < size; ++i)
	{
		//���ϸ��� �� csv ������ Row 1 Col 2�� ����Ǿ� ����
		std::string FileNameA;
		FileNameA = m_vecCSVEditor[i]->GetCell(1, 3);

		//CSVEditor�� �� �����Ƿ� �����Ѵ�.
		SAFE_DELETE(m_vecCSVEditor[i]);
		m_vecCSVEditor[i] = nullptr;
		
		//tŸ�� ���ڿ��� �ٲ��ش�.
		std::tstring FileNameT;
#ifdef _UNICODE
		FileNameT.assign(FileNameA.begin(), FileNameA.end());
#else
		FileNameT = FileNameA;
#endif

		//Ǯ ��θ� ������ְ� �ε� �޼ҵ忡 �ε��� ��ȣ(i)�� �Բ� ����.
		std::tstring FullPath = BasicPath + FileNameT;

		//�ε� �õ�. ���� �� false ����
		if (!LoadCSVFullPath(FullPath.c_str(), (int)i))
			return false;
	}

	m_SelectTextureIndex = (int)m_vecCSVEditor.size() - 1;
	SendMessage(m_hTextureListBox, LB_SETCURSEL,
		(WPARAM)m_SelectTextureIndex, 0);

	//��� ������ ������ true ����
	return true;
}







void CSceneEdit::SelectTex()
{
	//���� ����Ʈ�ڽ����� � ���� ���õǾ��ִ��� Ȯ��
	m_SelectTextureIndex = (int)SendMessage(m_hTextureListBox, LB_GETCURSEL, 0, 0);

	//���õ��� �ʾ������� ��ŵ
	if(SelectIndexRangeCheck(ERange::Texture))
	{
		//���� ����Ʈ�� �� �����ش�.
		ClearSelect(ERange::AnimSeq);


		//���� ���� ���� �޾ƿ´�.
		size_t size = m_vecCSVEditor[m_SelectTextureIndex]->GetRowCount();

		//�̸��� ���� ���� ��ȸ�ϸ鼭 ����Ʈ�� �̸��� ����Ѵ�.
		for (size_t i = (int)ERowInfo::AnimSeqInfoStart; i < size; ++i)
		{
			SendMessageA(m_hAnimSeqListBox, LB_ADDSTRING, 0,
				(LPARAM)m_vecCSVEditor[m_SelectTextureIndex]->GetCell((int)i, 0).c_str());
		}
	}
}


void CSceneEdit::SelectAnimSeq()
{
	//���� ����Ʈ�ڽ����� � ���� ���õǾ��ִ��� Ȯ��
	m_SelectAnimSeqIndex = (int)SendMessage(m_hAnimSeqListBox, LB_GETCURSEL, 0, 0);

	//���� �׸��� �ε����� ���������� ���õǾ����� Ȯ��
	if (SelectIndexRangeCheck(ERange::AnimSeq))
	{
		//������ �ִϸ��̼� ������ ����� �ִ� �ุŭ ���ؼ� ����Ѵ�.
		m_SelectAnimSeqIndex += (int)ERowInfo::AnimSeqInfoStart;

		//���� ����Ʈ�� �� �����ش�.
		ClearSelect(ERange::Frame);



		size_t size = m_vecCSVEditor[m_SelectTextureIndex]
			->GetRow(m_SelectAnimSeqIndex).size();

		//�̸��� ���� ���� ��ȸ�ϸ鼭 ����Ʈ�� �̸��� ����Ѵ�.
		//���� 8�� ���ں��� ��ǥ�� �Է¹޴� ��
		int count = 0;
		for (size_t i = (size_t)EColInfo::FramePosStart; i < size; ++i)
		{
			//����ִ� ���̸� return
			if (m_vecCSVEditor[m_SelectTextureIndex]->GetCell(m_SelectAnimSeqIndex, (int)i) == "")
				return;

			char index[10] = {};
			sprintf_s(index, 10, "%d", (int)count);

			SendMessageA(m_hFrameListBox, LB_ADDSTRING, 0,
				(LPARAM)index);

			++count;
		}
	}
}




void CSceneEdit::SelectFrame()
{
	//���� ����Ʈ�ڽ����� � ���� ���õǾ��ִ��� Ȯ��
	m_SelectFrameIndex = (int)SendMessage(m_hFrameListBox, LB_GETCURSEL, 0, 0);

	//0�� �ε����� -1�� ���� ��ŵ
	if (SelectIndexRangeCheck(ERange::Frame))
	{
		//������ ����Ʈ���� �������� ��� PlayList�� ������ �����Ѵ�.
		m_SelectPlayIndex = -1;
		SendMessage(m_hPlayListBox, LB_SETCURSEL,
			(WPARAM)m_SelectPlayIndex, 0);


		//���� ��ǥ�� �Է¹޴� csv �κ��� �� ��ȣ�� ������.
		int CSVIndex = m_SelectFrameIndex + (int)EColInfo::FramePosStart;
		

		std::string src = m_vecCSVEditor[m_SelectTextureIndex]->
			GetCell(m_SelectAnimSeqIndex, CSVIndex);

		if (src == "")
			return;

		Vector2 Start;
		Vector2 End;
		Vector2 Offset;

		//�������� 4�� �̻��̸� Offset�� ��ϵǾ��ִٴ� ���̹Ƿ� Offset���� �����Ͽ� �����Ѵ�.
		int SlashCount = 0;
		for (size_t k = 0; k < src.size(); ++k)
		{
			if (src[k] == '/')
				++SlashCount;
		}

		//��������Ʈ ��ǥ���� �ԷµǾ������
		if (SlashCount == 3)
		{
			sscanf_s(src.c_str(), "%f/%f/%f/%f", &Start.x, &Start.y, &End.x, &End.y);
		}
		//������ ������ �Էµ������
		else if (SlashCount > 3)
		{
			sscanf_s(src.c_str(), "%f/%f/%f/%f/%f/%f",
				&Start.x, &Start.y, &End.x, &End.y, &Offset.x, &Offset.y);
		}
		else
		{
		}

		SetDlgItemInt(m_hDlg, IDC_EDIT_FRAME_STARTX, (int)Start.x, TRUE);
		SetDlgItemInt(m_hDlg, IDC_EDIT_FRAME_STARTY, (int)Start.y, TRUE);
		SetDlgItemInt(m_hDlg, IDC_EDIT_FRAME_ENDX, (int)End.x, TRUE);
		SetDlgItemInt(m_hDlg, IDC_EDIT_FRAME_ENDY, (int)End.y, TRUE);
		SetDlgItemInt(m_hDlg, IDC_EDIT_FRAME_OFFSETX, (int)Offset.x, TRUE);
		SetDlgItemInt(m_hDlg, IDC_EDIT_FRAME_OFFSETY, (int)Offset.y, TRUE);


		//�ش� �ؽ�ó�� ǥ���Ѵ�.
		LoadAnimInfo();
		std::string Name = m_vecCSVEditor[m_SelectTextureIndex]->
			GetCell(m_SelectAnimSeqIndex, (int)EColInfo::AnimSeqName);

		m_Viewer->SetAnimation(Name);
		m_Viewer->SetFrame(Name, m_SelectFrameIndex);
		m_Viewer->SetPlayScale(Name, 0.f);

		m_NeedReturnSetting = Name;

	}

}

void CSceneEdit::SelectPlayList()
{
	//���� ����Ʈ�ڽ����� � ���� ���õǾ��ִ��� Ȯ��
	m_SelectPlayIndex = (int)SendMessage(m_hPlayListBox, LB_GETCURSEL, 0, 0);
	

	//0�� �ε����� -1�� ���� ��ŵ
	if (m_SelectPlayIndex >= 0)
	{
		//������ ����Ʈ�� ���þ������� �����ϰ� �������� �����־��ٸ�
		//������ ���󺹱� ���ش�.
		m_SelectFrameIndex = -1;
		SendMessage(m_hFrameListBox, LB_SETCURSEL,
			(WPARAM)m_SelectFrameIndex, 0);
		if (!m_NeedReturnSetting.empty())
		{
			CAnimationInfo* Info = m_Viewer->m_Animation->FindAnimInfo(m_NeedReturnSetting);
			if (Info)
			{
				m_Viewer->SetPlayScale(m_NeedReturnSetting, 1.f);
				Info->Init();
			}
			m_NeedReturnSetting.clear();
		}


		//�ش� �ؽ�ó�� ����ִ� �̸��� �޾ƿ´�.
		char AnimName[MAX_PATH] = {};

		SendMessageA(m_hPlayListBox, LB_GETTEXT, m_SelectPlayIndex, (LPARAM)AnimName);

		float PlayTime = 1.f;
		int LoopFlag = (int)ELoopFlag::NoLoop;
		bool Reverse = false;
		float PlayScale = 1.f;
		int Layer = 0;
		float OffsetX = 0.f;
		float OffsetY = 0.f;
		float PivotX = 0.5f;
		float PivotY = 1.f;


		m_Viewer->GetAnimInfoValue(AnimName,
			PlayTime, LoopFlag, Reverse, PlayScale, Layer,
			OffsetX, OffsetY, PivotX, PivotY);

		{
			//�����͸� MFC�� �Ű��ش�.
			SetDlgItemInt(m_hDlg, IDC_EDIT_LOOPFLAG, LoopFlag, TRUE);
			SendMessage(m_hCheckReverse, BM_SETCHECK, (WPARAM)Reverse, 0);
			SetDlgItemInt(m_hDlg, IDC_EDIT_LAYER, Layer, TRUE);

			char Val[32] = {};
			sprintf_s(Val, "%.2f", PlayTime);
			SetDlgItemTextA(m_hDlg, IDC_EDIT_PLAYTIME, Val);

			memset(Val, 0, sizeof(Val));
			sprintf_s(Val, "%.2f", PlayScale);
			SetDlgItemTextA(m_hDlg, IDC_EDIT_PLAYSCALE, Val);

			memset(Val, 0, sizeof(Val));
			sprintf_s(Val, "%.0f", OffsetX);
			SetDlgItemTextA(m_hDlg, IDC_EDIT_OFFSETX, Val);

			memset(Val, 0, sizeof(Val));
			sprintf_s(Val, "%.0f", OffsetY);
			SetDlgItemTextA(m_hDlg, IDC_EDIT_OFFSETY, Val);

			memset(Val, 0, sizeof(Val));
			sprintf_s(Val, "%.2f", PivotX);
			SetDlgItemTextA(m_hDlg, IDC_EDIT_PIVOTX, Val);

			memset(Val, 0, sizeof(Val));
			sprintf_s(Val, "%.2f", PivotY);
			SetDlgItemTextA(m_hDlg, IDC_EDIT_PIVOTY, Val);
		}


		//�ִϸ��̼��� �����Ų��.
		m_Viewer->GetAnimation()->SetAnimation(AnimName);
	}
}

void CSceneEdit::ClearSelect(ERange Range)
{
	int range = (int)Range;
	if (range <= (int)ERange::All)
	{
		ClearList();
	}

	if (range <= (int)ERange::Texture)
	{
		m_SelectTextureIndex = -1;
		SendMessage(m_hTextureListBox, LB_RESETCONTENT, 0, 0);
		SendMessage(m_hTextureListBox, LB_SETCURSEL,
			(WPARAM)m_SelectTextureIndex, 0);
	}

	if (range <= (int)ERange::AnimSeq)
	{
		m_SelectAnimSeqIndex = -1;
		SendMessage(m_hAnimSeqListBox, LB_RESETCONTENT, 0, 0);
		SendMessage(m_hAnimSeqListBox, LB_SETCURSEL,
			(WPARAM)m_SelectAnimSeqIndex, 0);
	}

	if (range <= (int)ERange::Frame)
	{
		m_SelectFrameIndex = -1;
		SendMessage(m_hFrameListBox, LB_RESETCONTENT, 0, 0);
		SendMessage(m_hFrameListBox, LB_SETCURSEL,
			(WPARAM)m_SelectFrameIndex, 0);
	}

}

bool CSceneEdit::SelectIndexRangeCheck(ERange Range)
{
	int range = (int)Range;

	if (range >= (int)ERange::All)
	{

	}

	if (range >= (int)ERange::Texture)
	{
		if (m_SelectTextureIndex < 0)
			return false;

		size_t size = m_vecCSVEditor.size();
		if (m_SelectTextureIndex >= size)
			return false;
	}

	if (range >= (int)ERange::AnimSeq)
	{
		if (m_SelectAnimSeqIndex < 0)
			return false;

		//���� ó��
		size_t size = m_vecCSVEditor[m_SelectTextureIndex]
			->GetRowCount();

		if (m_SelectAnimSeqIndex >= size)
			return false;
	}

	if (range >= (int)ERange::Frame)
	{
		size_t size = m_vecCSVEditor[m_SelectTextureIndex]->
			GetRow(m_SelectAnimSeqIndex).size();
		if (m_SelectFrameIndex >= size)
			return false;
	}


	return true;
}

HWND CSceneEdit::GetHWND() const
{
	return m_hDlg;
}

void CSceneEdit::ChangeVisibleStatus()
{
	if (!m_Hide)
	{
		ShowWindow(m_hDlg, SW_HIDE);
		m_Hide = true;
	}
	else
	{
		ShowWindow(m_hDlg, SW_SHOW);
		m_Hide = false;
	}
}


bool CSceneEdit::LoadAnimInfo()
{
	//���� ����Ʈ�ڽ����� � ���� ���õǾ��ִ��� Ȯ��
	m_SelectAnimSeqIndex = (int)SendMessage(m_hAnimSeqListBox, LB_GETCURSEL, 0, 0);

	//���� �׸��� �ε����� ���������� ���õǾ����� Ȯ��
	if (SelectIndexRangeCheck(ERange::AnimSeq))
	{
		m_SelectAnimSeqIndex += (int)ERowInfo::AnimSeqInfoStart;

		//AnimationInfo�� �����ϱ� ���� ������ ����
		std::string AnimSeqName;
		int LoopFlag = (int)ELoopFlag::NoLoop;
		float PlayTime = 1.f;
		float PlayScale = 1.f;
		bool Reverse = false;
		int Layer = 0;
		float OffsetX = 0.f;
		float OffsetY = 0.f;
		float PivotX = 0.5f;
		float PivotY = 1.f;


		//���� �׸��� �ε����� ���������� ���õǾ����� Ȯ��
		if (SelectIndexRangeCheck(ERange::AnimSeq))
		{
			//����ü �߰��� �� �����ֱ� �����ϵ��� ��ǥ �Է����� ������ ������ �������ش�.
			//csv������ AnimInfo ������ ������ �̸� �ں��� ��ǥ �Է����� �ٷ� ������
			for (int j = (int)EColInfo::AnimSeqName; j <= (int)EColInfo::FramePosStart - 1; ++j)
			{
				std::string src = m_vecCSVEditor[m_SelectTextureIndex]->GetCell(m_SelectAnimSeqIndex, j);
				if (j == (int)EColInfo::AnimSeqName)
				{
					AnimSeqName = src;
				}
				else
				{
					int temp = 0;

					switch (j)
					{
					case (int)EColInfo::LoopFlag://Loop: ���ۿ����� ������ ���� �߰� ó��
						sscanf_s(src.c_str(), "%d", &LoopFlag);
						break;
					case (int)EColInfo::PlayTime://PlayTime
						sscanf_s(src.c_str(), "%f", &PlayTime);
						break;
					case (int)EColInfo::PlayScale://PlayScale
						sscanf_s(src.c_str(), "%f", &PlayScale);
						break;
					case (int)EColInfo::Reverse://Reverse:���ۿ����� ������ ���� byte Ÿ������
						sscanf_s(src.c_str(), "%d", &temp);
						Reverse = temp;
						break;
					case (int)EColInfo::Layer://Layer
						sscanf_s(src.c_str(), "%d", &Layer);
						break;
					case (int)EColInfo::OffsetX://OffsetX
						sscanf_s(src.c_str(), "%f", &OffsetX);
						break;
					case (int)EColInfo::OffsetY://OffsetY
						sscanf_s(src.c_str(), "%f", &OffsetY);
						break;
					case (int)EColInfo::PivotX:
						sscanf_s(src.c_str(), "%f", &PivotX);
						break;
					case (int)EColInfo::PivotY:
						sscanf_s(src.c_str(), "%f", &PivotY);
						break;
					default:
						break;
					}
				}
			}

			//�� �࿡ ���� ��ȸ�� �Ϸ�Ǹ� �ش� �ִϸ��̼ǿ� ���� ������ �߰�
			m_Viewer->ViewAnimation(AnimSeqName,
				PlayTime,
				(ELoopFlag)LoopFlag,
				Reverse,
				PlayScale,
				Layer,
				OffsetX,
				OffsetY,
				PivotX,
				PivotY);



			//m_Viewer->AddAnimationInfo(
			//	AnimSeqName,
			//	PlayTime,
			//	(ELoopFlag)LoopFlag,
			//	Reverse,
			//	PlayScale,
			//	Layer,
			//	OffsetX,
			//	OffsetY,
			//	PivotX,
			//	PivotY
			//);

			return true;
		}
	}

	return false;
}


void CSceneEdit::AddPlayList()
{
	if (!LoadAnimInfo())
		return;

	std::string AnimSeqName = m_vecCSVEditor[m_SelectTextureIndex]->GetCell(m_SelectAnimSeqIndex, (int)EColInfo::AnimSeqName);

	SendMessageA(m_hPlayListBox, LB_ADDSTRING, 0,
			(LPARAM)AnimSeqName.c_str());

	m_Viewer->ChangeAnimation(AnimSeqName);

}

void CSceneEdit::PlayAnimation()
{
	m_Viewer->Play();
}

void CSceneEdit::ClearList()
{
	m_SelectFrameIndex = -1;
	SendMessage(m_hPlayListBox, LB_RESETCONTENT, 0, 0);
	SendMessage(m_hPlayListBox, LB_SETCURSEL,
		(WPARAM)m_SelectFrameIndex, 0);

	m_Viewer->ResetAnimation();
}

void CSceneEdit::ReturnStartScene()
{
	CSceneManager::GetInst()->CreateScene<CStartScene>();
	DestroyWindow(m_hDlg);
}

void CSceneEdit::InfoValueSet()
{
	//���� ����Ʈ�ڽ����� � ���� ���õǾ��ִ��� Ȯ��
	m_SelectPlayIndex = (int)SendMessage(m_hPlayListBox, LB_GETCURSEL, 0, 0);

	//-1�� ���� ��ŵ
	if (m_SelectPlayIndex >= 0)
	{
		//�ش� �ؽ�ó�� ����ִ� �̸��� �޾ƿ´�.
		char AnimName[MAX_PATH] = {};
		SendMessageA(m_hPlayListBox, LB_GETTEXT, m_SelectPlayIndex, (LPARAM)AnimName);


		int LoopFlag = 0;
		float PlayTime = 1.f;
		float PlayScale = 1.f;
		bool Reverse = false;
		int Layer = 0;
		float OffsetX = 0.f;
		float OffsetY = 0.f;
		float PivotX = 0.5f;
		float PivotY = 1.f;


		{
			//�����͸� MFC�κ��� �޾ƿ´�.
			LoopFlag = GetDlgItemInt(m_hDlg, IDC_EDIT_LOOPFLAG, NULL, TRUE);
			Reverse = (bool)SendMessage(m_hCheckReverse, BM_GETCHECK, 0, 0);
			Layer = GetDlgItemInt(m_hDlg, IDC_EDIT_LAYER, NULL, TRUE);

			char Val[32] = {};

			GetDlgItemTextA(m_hDlg, IDC_EDIT_PLAYTIME, Val, 32);
			sscanf_s(Val, "%f", &PlayTime);

			memset(Val, 0, sizeof(Val));
			GetDlgItemTextA(m_hDlg, IDC_EDIT_PLAYSCALE, Val, 32);
			sscanf_s(Val, "%f", &PlayScale);

			memset(Val, 0, sizeof(Val));
			GetDlgItemTextA(m_hDlg, IDC_EDIT_OFFSETX, Val, 32);
			sscanf_s(Val, "%f", &OffsetX);

			memset(Val, 0, sizeof(Val));
			GetDlgItemTextA(m_hDlg, IDC_EDIT_OFFSETY, Val, 32);
			sscanf_s(Val, "%f", &OffsetY);

			memset(Val, 0, sizeof(Val));
			GetDlgItemTextA(m_hDlg, IDC_EDIT_PIVOTX, Val, 32);
			sscanf_s(Val, "%f", &PivotX);

			memset(Val, 0, sizeof(Val));
			GetDlgItemTextA(m_hDlg, IDC_EDIT_PIVOTY, Val, 32);
			sscanf_s(Val, "%f", &PivotY);
		}

		m_Viewer->ViewAnimation(AnimName,
			PlayTime, (ELoopFlag)LoopFlag, Reverse, PlayScale, Layer,
			OffsetX, OffsetY, PivotX, PivotY);

		//�ִϸ��̼��� �����Ų��.
		m_Viewer->GetAnimation()->ChangeAnimation(AnimName);
	}
}




void CSceneEdit::Reset()
{
	//����Ʈ �� �����
	ClearSelect(ERange::All);

	//����� �ִϸ��̼ǵ� ��� �����ְ�
	m_Viewer->ResetAnimation();

	//�ε�� CAnimationSeq�� �� ��������
	m_SceneResource->ClearAnimSeq();

	//�ε�� csv�鵵 �� �����ش�.
	for (size_t i = 0; i < m_vecCSVEditor.size(); ++i)
	{
		SAFE_DELETE(m_vecCSVEditor[i]);
	}
	m_vecCSVEditor.clear();
}

LRESULT CSceneEdit::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		//CLOSE �޽��� -> â �ݱ� �޽��� ����
	case WM_CLOSE:
		s_Dlg->ChangeVisibleStatus();
		break;
		//â �ݱ� �޽��� -> �޽��� ������ false�� �ٲ㼭 �ߴܽ�Ŵ
	case WM_DESTROY:
		s_Dlg->ChangeVisibleStatus();
		break;
	case WM_QUIT:
		PostQuitMessage(0);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

			//��ư ����
		case IDC_BUTTON_QUIT:
			s_Dlg->ChangeVisibleStatus();
			break;
		case IDC_BUTTON_LOADCSV:
			s_Dlg->LoadCSV();
			break;
		case IDC_BUTTON_ADDPLAYLIST:
			s_Dlg->AddPlayList();
			break;
		case IDC_BUTTON_PLAY:
			s_Dlg->PlayAnimation();
			break;
		case IDC_BUTTON_CLEARLIST:
			s_Dlg->ClearList();
			break;
		case IDC_BUTTON_INFOVALUESET:
			s_Dlg->InfoValueSet();
			break;
		case IDC_BUTTON_RESET:
			s_Dlg->Reset();
			break;
		case IDC_BUTTON_RELOADCSV:
			s_Dlg->ReloadCSV();
			break;




			//����Ʈ ����
		case IDC_LIST_TEXTURENAME:
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				s_Dlg->SelectTex();
				break;
			}
			break;

		case IDC_LIST_ANIMSEQ:
			switch (HIWORD(wParam))
			{
				//����Ʈ�� ���� ������ ��
			case LBN_SELCHANGE:
				s_Dlg->SelectAnimSeq();
				break;
			}
		case IDC_LIST_SPRITE_FRAME:
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				s_Dlg->SelectFrame();
				break;
			}
		}
	case IDC_LIST_PLAYLIST:
		switch (HIWORD(wParam))
		{
		case LBN_SELCHANGE:
			s_Dlg->SelectPlayList();
			break;
		}



		break;
	default:
		break;
	}


	return 0;
}







