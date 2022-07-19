//Class CGameManager
//GameManager.h
#pragma once

#include "GameInfo.h"
#include "Singleton.h"

struct DebugText
{
	float Duration;
	float DurationMax;
	std::string Text;
	bool AddMode;	//�� ����Ŭ�� ���� �̸����� ������ ���Խ� �ڿ� �߰�

	DebugText() :
		DurationMax(3.f),
		AddMode()
	{
		Duration = DurationMax;
	}
};


class CGameManager
{
private:
	//�÷��̾� ��� ���Խ�Ű�� �ʰ� ���漱��(�ڵ岿�� ����)
	//�÷��̾�� ���� ���̹Ƿ� ������ ���� ��Ƴ���, 
	//�ٸ� ��ü���� ��û �� �ּҸ� ��ȯ���ش�.
	class CPlayer* m_Player;
public:
	void SetPlayer(class CPlayer* Player);
	void DeletePlayer(class CPlayer* Player);
	class CPlayer* GetPlayer()	const;

private:
	//GDI PLUS
	ULONG_PTR m_gpToken;
	Gdiplus::GdiplusStartupInput m_gpsi;

	//������ �ν��Ͻ�
	HINSTANCE m_hInst;
	HWND m_hWnd;
	HDC m_hDC;

	//����ۿ�
	HDC m_BackhDC;
	HBITMAP m_BackhBmp;
	HBITMAP m_BackhBmpPrev;

	//������ â ������ �����
	Resolution m_WindowSize;

	//������ ���� �ػ�
	Resolution m_GameRes;

	//static �޼ҵ��� WinProc�� ���� static ������ ó�������ϹǷ�
	static bool m_Loop;

	class CTimer* m_Timer;
	float m_DeltaTime;
	float m_TimeScale;
	int		m_RandNum;

	//����׿�
	bool	m_DebugRender;
	HBRUSH	m_Brush[(int)EBrushType::Max];
	HPEN	m_Pen[(int)EBrushType::Max];
	std::list<DebugText*> m_listDebugText;
	std::string m_Blank;
public:
	void DebugTextWipe();
	void DebugTextOut(const std::string& KeyName, const std::string& txt);
	void DebugTextOut(const std::string& KeyName, int Num);
	void DebugTextOut(const std::string& KeyName, float Flt);
	void DebugTextOut(const std::string& KeyName, Vector2 Vec);
	std::list<DebugText*>::iterator FindKeyName(const std::string& KeyName);
	void DebugTextRender();
	void ToggleDebugRender();
	bool GetDebugRender() const;
	
private:
	//������� üũ�� ����
	bool m_EditMode;
public:
	void SetEditMode(bool Enable = true);
	bool GetEditMode() const;

private:
	int m_WhiteOut;
public:
	void SetWhiteOut(int Frame);

public:
	bool Init(HINSTANCE hInstance);
	int Run();

private:
	void Logic();
	void Input(float DeltaTime);
	bool Update(float DeltaTime);
	bool PostUpdate(float DeltaTime);

	void Collision(float DeltaTime);
	void Render(float DeltaTime);
	void ShowBasicStatus();

	void Register();
	bool Create();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void TimeScaleUp();
	void TimeScaleDown();

public:
	

	HBRUSH GetBrush(EBrushType Type) const;
	HPEN GetPen(EBrushType Type) const;



	float GetTimeScale()	const;
	void SetTimeScale(float TimeScale);

	int GetRandNum()	const;

	HINSTANCE GetWindowInstance()	const;

	HDC GetWindowDC()	const;
	HDC GetWindowBackDC() const;

	HWND GetWindowHandle()	const;

	float GetDeltaTime()	const;
				
	Resolution GetWindowSize() const;

	DECLARE_SINGLETON(CGameManager)
};