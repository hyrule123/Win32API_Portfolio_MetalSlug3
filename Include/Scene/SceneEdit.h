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


//���̾�α� ����
private:
	//���̾�α� �ڵ�
	HWND	m_hDlg;
public:
	HWND GetHWND() const;

private:
	//â ����⸦ �ߴ��� ���� Ȯ�ο�
	bool m_Hide;
public:
	void ChangeVisibleStatus();

	//Ŭ���̾�Ʈ ������ ���������� CGameObject
	CSharedPtr<class CEditViewer> m_Viewer;

	//��� ������ ���� ������ ���⿡
	std::vector<class CCSVEditor*> m_vecCSVEditor;

private://����Ʈ ������ �ٲ� ��� �ٲ���� �ϴ� ������
	HWND	m_hTextureListBox;
	int		m_SelectTextureIndex;

	HWND	m_hAnimSeqListBox;
	int		m_SelectAnimSeqIndex;

	HWND	m_hFrameListBox;
	int		m_SelectFrameIndex;
	std::string m_NeedReturnSetting;


	std::vector<Sprite_FrameData> m_vecFrame;


public:
	//����Ʈ ���� �������� ������� �� ȣ��� �޼ҵ�.
	void SelectTex();
	void SelectAnimSeq();
	void SelectFrame();
	

private://�ִϸ��̼� �÷��̸���Ʈ ���� ��ҵ�
	HWND	m_hPlayListBox;
	HWND	m_hCheckReverse;
	int		m_SelectPlayIndex;

	void	SelectPlayList();
	void	InfoValueSet();
	bool	LoadAnimInfo();
	void	AddPlayList();
	void	PlayAnimation();
	void	ClearList(); //�ִϸ��̼� �÷��̸���Ʈ ����
	void	ReturnStartScene();	//���� ��ŸƮ������ ����


private: 
	//����Ʈ ����� ��� clear ���ְ� ��ĭ �ϳ��� �����Ѵ�(�ε��� ��ȣ�� ����Ʈ ��ȣ ���߱� ����)
	void ClearSelect(ERange Range);

	//������ �������� ���� �׸��� �ε��� ��ȣ�� ���� �ȿ� ���õǾ��ִ��� Ȯ���Ѵ�.
	bool SelectIndexRangeCheck(ERange Range);


	//�ε� ����
	bool LoadCSV(); //CSV ���� �ε�
	//Ǯ ��θ� �޾Ƽ� �ε� ������ ó���ϴ� �޼ҵ�.
	//���� index�� �⺻�� -1�� �ƴ� Ư�� �ε��� ���� ������ �ش� �ε����� ����.
	//->ReloadCSV��
	bool LoadCSVFullPath(const TCHAR* FullPath, int index = -1);
	bool ReloadCSV(const std::string& PathName = DATA_PATH);
	void Reset();

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public://���� �޼ҵ��� WndProc�� �ּҸ� �������ֱ� ���� ���� ����
	static CSceneEdit* s_Dlg;

};

