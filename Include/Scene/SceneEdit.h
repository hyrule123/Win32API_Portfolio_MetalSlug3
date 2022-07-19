#pragma once
#include "Scene.h"

enum class ERange
{
	All,
	Texture,
	AnimSeq,
	Frame
};



class CSceneEdit :
    public CScene
{
	friend class CSceneManager;
	friend class CSceneEdit;

protected:
	CSceneEdit();
	virtual ~CSceneEdit();
public:
	bool Init();


//다이얼로그 관련
private:
	//다이얼로그 핸들
	HWND	m_hDlg;
public:
	HWND GetHWND() const;

private:
	//창 숨기기를 했는지 여부 확인용
	bool m_Hide;
public:
	void ChangeVisibleStatus();

	//클라이언트 영역에 렌더링해줄 CGameObject
	CSharedPtr<class CEditViewer> m_Viewer;

	//모든 데이터 최종 저장은 여기에
	std::vector<class CCSVEditor*> m_vecCSVEditor;

private://리스트 선택이 바뀔 경우 바꿔줘야 하는 변수들
	HWND	m_hTextureListBox;
	int		m_SelectTextureIndex;

	HWND	m_hAnimSeqListBox;
	int		m_SelectAnimSeqIndex;

	HWND	m_hFrameListBox;
	int		m_SelectFrameIndex;
	std::string m_NeedReturnSetting;


	std::vector<Sprite_FrameData> m_vecFrame;


public:
	//리스트 선택 프레임이 변경됐을 때 호출될 메소드.
	void SelectTex();
	void SelectAnimSeq();
	void SelectFrame();
	

private://애니메이션 플레이리스트 관련 요소들
	HWND	m_hPlayListBox;
	HWND	m_hCheckReverse;
	int		m_SelectPlayIndex;

	void	SelectPlayList();
	void	InfoValueSet();
	bool	LoadAnimInfo();
	void	AddPlayList();
	void	PlayAnimation();
	void	ClearList(); //애니메이션 플레이리스트 제거
	void	ReturnStartScene();	//도로 스타트씬으로 복귀


private: 
	//리스트 목록을 모두 clear 해주고 빈칸 하나만 생성한다(인덱스 번호와 리스트 번호 맞추기 위함)
	void ClearSelect(ERange Range);

	//지정한 범위부터 하위 항목의 인덱스 번호가 범위 안에 세팅되어있는지 확인한다.
	bool SelectIndexRangeCheck(ERange Range);


	//로드 관련
	bool LoadCSV(); //CSV 파일 로드
	//풀 경로를 받아서 로드 과정을 처리하는 메소드.
	//만약 index에 기본값 -1이 아닌 특정 인덱스 값이 들어오면 해당 인덱스에 삽입.
	//->ReloadCSV용
	bool LoadCSVFullPath(const TCHAR* FullPath, int index = -1);
	bool ReloadCSV(const std::string& PathName = DATA_PATH);
	void Reset();

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public://정적 메소드인 WndProc에 주소를 전달해주기 위한 정적 변수
	static CSceneEdit* s_Dlg;

};

