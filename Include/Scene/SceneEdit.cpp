#include "SceneEdit.h"

//csv 리더
#include "../CSVEditor.h"

//다이얼로그 종료 시 스타트씬으로 돌아감. 씬 전환 용
#include "SceneManager.h"
#include "StartScene.h"

//카메라 이동, 로드 후 카메라 위치 재설정
#include "Camera.h"

//게임매니저에 에딧모드 변경을 알려주기 위함
#include "../GameManager.h"

#include "../Resource/Texture/Texture.h"
#include "../Input.h"

#include "SceneEdit.h"


//윈도우 인스턴스 받아오는 용도
#include "../GameManager.h"

//경로 관리자
#include "../PathManager.h"

#include "../Resource/Texture/Texture.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/SceneResource.h"


//리소스 정의 번호 가져오기 위함
#include "../resource.h"
#include "../Resource/Animation/AnimationSequence.h"
#include "../UI/WindowEdit.h"

//애니메이션 렌더링용 게임오브젝트
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
	//에디터 모드임을 알려줌.
	CGameManager::GetInst()->SetEditMode();

	//정적변수에 주소를 등록
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

	//이 씬과 관련된 입력바인딩을 모두 제거한다.
	CInput::GetInst()->DeleteBindClass<CSceneEdit>(this);
	
}

bool CSceneEdit::Init()
{
	//단축키와, 호출할 콜백 메소드 등록.
	CInput::GetInst()->AddBindFunction<CSceneEdit>("OpenEditor",
		EInput_Type::Down, this, &CSceneEdit::ChangeVisibleStatus);

	CreateWidgetWindow<CWindowEdit>("WindowEdit");

	CreateObject<CGameObject>("Model");



	//창 생성
	m_hDlg = CreateDialog(CGameManager::GetInst()->GetWindowInstance(), MAKEINTRESOURCE(IDD_DIALOG_ANIMEDIT), CGameManager::GetInst()->GetWindowHandle(), CSceneEdit::WndProc);

	CGameManager* Mgr = CGameManager::GetInst();

	RECT rcmain;
	GetWindowRect(Mgr->GetWindowHandle(), &rcmain);

	RECT rcdlg;
	GetClientRect(m_hDlg, &rcdlg);

	Resolution Res = Mgr->GetWindowSize();
	rcmain.left = rcmain.left + (long)Res.Width - rcdlg.right;
	rcmain.top = rcmain.top + (long)Res.Height - rcdlg.bottom;




	//윈도우의 위치 지정. 사이즈는 변경하지 않고(SWP_NOSIZE) 위치만
	SetWindowPos(m_hDlg, 0, rcmain.left, rcmain.top, 0, 0, SWP_NOSIZE);

	//창 띄움
	ShowWindow(m_hDlg, SW_SHOW);
	m_Hide = false;

	//리스트의 핸들을 얻어온다.
	m_hTextureListBox = GetDlgItem(m_hDlg, IDC_LIST_TEXTURENAME);
	m_hAnimSeqListBox = GetDlgItem(m_hDlg, IDC_LIST_ANIMSEQ);
	m_hFrameListBox = GetDlgItem(m_hDlg, IDC_LIST_SPRITE_FRAME);
	m_hPlayListBox = GetDlgItem(m_hDlg, IDC_LIST_PLAYLIST);

	//체크박스 핸들 얻어온다.
	m_hCheckReverse = GetDlgItem(m_hDlg, IDC_CHECK_REVERSE);


	//렌더링용 게임오브젝트
	m_Viewer = CreateObject<CEditViewer>("Viewer");
	m_Viewer->SetPos(100.f, 100.f);


	//GameObject에 등록된 디버그용 키를 등록 해제한다.
	CInput::GetInst()->DeleteBindClass<CGameManager>(CGameManager::GetInst());



	CInput::GetInst()->AddBindFunction<CSceneEdit>("EditMode", EInput_Type::Down, this, &CSceneEdit::ReturnStartScene);



	return true;
}


bool CSceneEdit::LoadCSV()
{
	//풀경로를 받아올 주소 변수를 만들어주고
	TCHAR FullPath[MAX_PATH] = {};


	//파일 열기 창에 전달할 설정 구조체를 설정해준다.
	OPENFILENAME OpenFile = {};


	OpenFile.lStructSize = sizeof(OPENFILENAME);	//구조체 크기
	OpenFile.hwndOwner = m_hDlg;	//관리 핸들
	OpenFile.lpstrFilter = TEXT("*.csv File\0*.csv\0모든 파일\0*.*"); 	//파일 형식
	OpenFile.lpstrFile = FullPath;	//경로가 저장될 변수 주소
	OpenFile.nMaxFile = MAX_PATH;	//최대 경로 자릿수

#ifdef _UNICODE
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path;
#else
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->PathMultiByte;
#endif // _UNICODE

	//만들어진 풀 경로를 FullPath에 보낸다.
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

	//csv의 두번째행 첫번째 열에는 텍스처 이름이 적혀있다.
	std::string TexName = CSVEditor->GetCell((int)ERowInfo::TexInfo, 0);

	//첫행 두번째행 두번째 열에는 애니메이션 파일명이 적혀있다.
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
		//컬러키가 들어있는 셀
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
		//컬러키 RGB값이 정상적으로 입력되었을 경우
		if (SlashCount == 2)
		{
			sscanf_s(src.c_str(), "%d/%d/%d", &R, &G, &B);
		}
		GetSceneResource()->SetColorKey(TexName, R, G, B);

		//텍스처가 로드되면 다이얼로그에 텍스처 이름과 파일 이름을 전송
		SendMessageA(m_hTextureListBox, LB_ADDSTRING, 0, (LPARAM)TexName.c_str());
	}
	else
	{
		SAFE_DELETE(CSVEditor);
		return false;
	}

	//네 번째 행 첫번쨰 열부터 만들어야 할 애니메이션 시퀀스 이름이 적혀있다.
	for (int i = (int)ERowInfo::AnimSeqInfoStart; i < CSVEditor->GetRowCount(); i++)
	{
		std::string AnimName;

		auto& row = CSVEditor->GetRow(i);
		for (int j = 0; j < row.size(); j++)
		{
			std::string src = CSVEditor->GetCell(i, j);

			//인자 번호가 몇번까지 들어왔는지를 카운트.
			int ParamCount = 0;
			//만약 아무 내용도 입력되어있지 않다면 continue
			//AnimInfo 인자값에도 공란이 들어올 수 있으므로 해당 사항도 체크
			if (src == "")
			{
				continue;
			}


			//가장 첫번째 열은 만들 시퀀스의 이름이 적혀 있다.
			if (j == (int)EColInfo::AnimSeqName)
			{
				AnimName = src;

				//애니메이션 시퀀스 생성에 성공하면 다이얼로그에 이름 전송
				if (GetSceneResource()->
					CreateAnimationSequence(AnimName, TexName))
				{
					//리스트에 애니메이션 시퀀스 이름을 추가
					SendMessageA(m_hAnimSeqListBox, LB_ADDSTRING, 0, (LPARAM)AnimName.c_str());
				}
				else
				{
					SAFE_DELETE(CSVEditor);
					return false;
				}
			}
			else if (j >= (int)EColInfo::FramePosStart)//해당 행의 좌표 부분 로드
			{
				Vector2 Start;
				Vector2 End;
				Vector2 Offset;

				//슬래쉬가 4개 이상이면 Offset도 등록되어있다는 뜻이므로 Offset값도 전달하여 생성한다.
				int SlashCount = 0;
				for (size_t k = 0; k < src.size(); ++k)
				{
					if (src[k] == '/')
						++SlashCount;
				}

				//입력 안된 부분이 나오면 return;
				if (SlashCount < 3)
					break;
				//스프라이트 좌표값만 입력되었을경우
				else if (SlashCount == 3)
				{
					sscanf_s(src.c_str(), "%f/%f/%f/%f", &Start.x, &Start.y, &End.x, &End.y);
				}
				//오프셋 값까지 입력됐을경우
				else if (SlashCount > 3)
				{
					sscanf_s(src.c_str(), "%f/%f/%f/%f/%f/%f",
						&Start.x, &Start.y, &End.x, &End.y, &Offset.x, &Offset.y);
				}


				//프레임 데이터 추가
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
		//여기까지 다 끝나면 CSVEditor 주소를 맵에 삽입
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
	//비어있으면 return
	if (m_vecCSVEditor.empty())
		return false;

	//리스트 다 지우고
	ClearSelect(ERange::All);

	//뷰어의 애니메이션도 모두 지워주고
	m_Viewer->ResetAnimation();


	//로드된 CAnimationSeq도 다 지워준후
	m_SceneResource->ClearAnimSeq();


	//경로를 받아온다.
	std::tstring BasicPath;
#ifdef _UNICODE
	BasicPath = CPathManager::GetInst()->FindPath(PathName)->Path;
#else
	BasicPath = CPathManager::GetInst()->FindPath(PathName)->PathMultiByte;
#endif // _UNICODE

	size_t size = m_vecCSVEditor.size();
	for (size_t i = 0; i < size; ++i)
	{
		//파일명은 각 csv 파일의 Row 1 Col 2에 저장되어 있음
		std::string FileNameA;
		FileNameA = m_vecCSVEditor[i]->GetCell(1, 3);

		//CSVEditor를 다 썼으므로 제거한다.
		SAFE_DELETE(m_vecCSVEditor[i]);
		m_vecCSVEditor[i] = nullptr;
		
		//t타입 문자열로 바꿔준다.
		std::tstring FileNameT;
#ifdef _UNICODE
		FileNameT.assign(FileNameA.begin(), FileNameA.end());
#else
		FileNameT = FileNameA;
#endif

		//풀 경로를 만들어주고 로드 메소드에 인덱스 번호(i)와 함께 전달.
		std::tstring FullPath = BasicPath + FileNameT;

		//로드 시도. 실패 시 false 리턴
		if (!LoadCSVFullPath(FullPath.c_str(), (int)i))
			return false;
	}

	m_SelectTextureIndex = (int)m_vecCSVEditor.size() - 1;
	SendMessage(m_hTextureListBox, LB_SETCURSEL,
		(WPARAM)m_SelectTextureIndex, 0);

	//모든 과정이 끝나면 true 리턴
	return true;
}







void CSceneEdit::SelectTex()
{
	//현재 리스트박스에서 어떤 것이 선택되어있는지 확인
	m_SelectTextureIndex = (int)SendMessage(m_hTextureListBox, LB_GETCURSEL, 0, 0);

	//선택되지 않았을때는 스킵
	if(SelectIndexRangeCheck(ERange::Texture))
	{
		//하위 리스트를 다 지워준다.
		ClearSelect(ERange::AnimSeq);


		//행의 수를 전부 받아온다.
		size_t size = m_vecCSVEditor[m_SelectTextureIndex]->GetRowCount();

		//이름이 적힌 행을 순회하면서 리스트에 이름을 등록한다.
		for (size_t i = (int)ERowInfo::AnimSeqInfoStart; i < size; ++i)
		{
			SendMessageA(m_hAnimSeqListBox, LB_ADDSTRING, 0,
				(LPARAM)m_vecCSVEditor[m_SelectTextureIndex]->GetCell((int)i, 0).c_str());
		}
	}
}


void CSceneEdit::SelectAnimSeq()
{
	//현재 리스트박스에서 어떤 것이 선택되어있는지 확인
	m_SelectAnimSeqIndex = (int)SendMessage(m_hAnimSeqListBox, LB_GETCURSEL, 0, 0);

	//상위 항목의 인덱스가 정상적으로 선택되었는지 확인
	if (SelectIndexRangeCheck(ERange::AnimSeq))
	{
		//엑셀의 애니메이션 시퀀스 목록이 있는 행만큼 더해서 계산한다.
		m_SelectAnimSeqIndex += (int)ERowInfo::AnimSeqInfoStart;

		//하위 리스트를 다 지워준다.
		ClearSelect(ERange::Frame);



		size_t size = m_vecCSVEditor[m_SelectTextureIndex]
			->GetRow(m_SelectAnimSeqIndex).size();

		//이름이 적힌 행을 순회하면서 리스트에 이름을 등록한다.
		//현재 8번 인자부터 좌표를 입력받는 중
		int count = 0;
		for (size_t i = (size_t)EColInfo::FramePosStart; i < size; ++i)
		{
			//비어있는 셀이면 return
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
	//현재 리스트박스에서 어떤 것이 선택되어있는지 확인
	m_SelectFrameIndex = (int)SendMessage(m_hFrameListBox, LB_GETCURSEL, 0, 0);

	//0번 인덱스와 -1일 때는 스킵
	if (SelectIndexRangeCheck(ERange::Frame))
	{
		//프레임 리스트에서 선택했을 경우 PlayList는 선택을 해제한다.
		m_SelectPlayIndex = -1;
		SendMessage(m_hPlayListBox, LB_SETCURSEL,
			(WPARAM)m_SelectPlayIndex, 0);


		//현재 좌표를 입력받는 csv 부분의 열 번호를 더해줌.
		int CSVIndex = m_SelectFrameIndex + (int)EColInfo::FramePosStart;
		

		std::string src = m_vecCSVEditor[m_SelectTextureIndex]->
			GetCell(m_SelectAnimSeqIndex, CSVIndex);

		if (src == "")
			return;

		Vector2 Start;
		Vector2 End;
		Vector2 Offset;

		//슬래쉬가 4개 이상이면 Offset도 등록되어있다는 뜻이므로 Offset값도 전달하여 생성한다.
		int SlashCount = 0;
		for (size_t k = 0; k < src.size(); ++k)
		{
			if (src[k] == '/')
				++SlashCount;
		}

		//스프라이트 좌표값만 입력되었을경우
		if (SlashCount == 3)
		{
			sscanf_s(src.c_str(), "%f/%f/%f/%f", &Start.x, &Start.y, &End.x, &End.y);
		}
		//오프셋 값까지 입력됐을경우
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


		//해당 텍스처를 표시한다.
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
	//현재 리스트박스에서 어떤 것이 선택되어있는지 확인
	m_SelectPlayIndex = (int)SendMessage(m_hPlayListBox, LB_GETCURSEL, 0, 0);
	

	//0번 인덱스와 -1일 때는 스킵
	if (m_SelectPlayIndex >= 0)
	{
		//프레임 리스트는 선택안함으로 변경하고 프레임을 보고있었다면
		//설정을 원상복구 해준다.
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


		//해당 텍스처에 들어있는 이름을 받아온다.
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
			//데이터를 MFC로 옮겨준다.
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


		//애니메이션을 재생시킨다.
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

		//예외 처리
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
	//현재 리스트박스에서 어떤 것이 선택되어있는지 확인
	m_SelectAnimSeqIndex = (int)SendMessage(m_hAnimSeqListBox, LB_GETCURSEL, 0, 0);

	//상위 항목의 인덱스가 정상적으로 선택되었는지 확인
	if (SelectIndexRangeCheck(ERange::AnimSeq))
	{
		m_SelectAnimSeqIndex += (int)ERowInfo::AnimSeqInfoStart;

		//AnimationInfo를 생성하기 위한 변수들 선언
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


		//상위 항목의 인덱스가 정상적으로 선택되었는지 확인
		if (SelectIndexRangeCheck(ERange::AnimSeq))
		{
			//열거체 중간에 뭘 끼워넣기 용이하도록 좌표 입력지점 이전을 범위로 지정해준다.
			//csv에서의 AnimInfo 데이터 범위는 이름 뒤부터 좌표 입력지점 바로 전까지
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
					case (int)EColInfo::LoopFlag://Loop: 버퍼오버런 방지를 위해 추가 처리
						sscanf_s(src.c_str(), "%d", &LoopFlag);
						break;
					case (int)EColInfo::PlayTime://PlayTime
						sscanf_s(src.c_str(), "%f", &PlayTime);
						break;
					case (int)EColInfo::PlayScale://PlayScale
						sscanf_s(src.c_str(), "%f", &PlayScale);
						break;
					case (int)EColInfo::Reverse://Reverse:버퍼오버런 방지를 위해 byte 타입으로
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

			//한 행에 대한 순회가 완료되면 해당 애니메이션에 대한 데이터 추가
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
	//현재 리스트박스에서 어떤 것이 선택되어있는지 확인
	m_SelectPlayIndex = (int)SendMessage(m_hPlayListBox, LB_GETCURSEL, 0, 0);

	//-1일 때는 스킵
	if (m_SelectPlayIndex >= 0)
	{
		//해당 텍스처에 들어있는 이름을 받아온다.
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
			//데이터를 MFC로부터 받아온다.
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

		//애니메이션을 재생시킨다.
		m_Viewer->GetAnimation()->ChangeAnimation(AnimName);
	}
}




void CSceneEdit::Reset()
{
	//리스트 다 지우고
	ClearSelect(ERange::All);

	//뷰어의 애니메이션도 모두 지워주고
	m_Viewer->ResetAnimation();

	//로드된 CAnimationSeq도 다 지워준후
	m_SceneResource->ClearAnimSeq();

	//로드된 csv들도 싹 지워준다.
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
		//CLOSE 메시지 -> 창 닫기 메시지 생성
	case WM_CLOSE:
		s_Dlg->ChangeVisibleStatus();
		break;
		//창 닫기 메시지 -> 메시지 루프를 false로 바꿔서 중단시킴
	case WM_DESTROY:
		s_Dlg->ChangeVisibleStatus();
		break;
	case WM_QUIT:
		PostQuitMessage(0);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

			//버튼 모음
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




			//리스트 모음
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
				//리스트의 선택 변경일 떄
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







