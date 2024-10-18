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
	bool AddMode;	//한 사이클에 같은 이름으로 여러번 삽입시 뒤에 추가

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
	//플레이어 헤더 포함시키지 않고 전방선언(코드꼬임 방지)
	//플레이어는 자주 쓰이므로 변수로 따로 잡아놓고, 
	//다른 객체에서 요청 시 주소를 반환해준다.
	class CPlayer* m_Player;
public:
	void SetPlayer(class CPlayer* Player);
	void DeletePlayer(class CPlayer* Player);
	class CPlayer* GetPlayer()	const;

private:
	//GDI PLUS
	ULONG_PTR m_gpToken;
	Gdiplus::GdiplusStartupInput m_gpsi;

	//윈도우 인스턴스
	HINSTANCE m_hInst;
	HWND m_hWnd;
	HDC m_hDC;

	//백버퍼용
	HDC m_BackhDC;
	HBITMAP m_BackhBmp;
	HBITMAP m_BackhBmpPrev;

	//윈도우 창 사이즈 저장용
	Resolution m_WindowSize;

	//게임의 원래 해상도
	Resolution m_GameRes;

	//static 메소드인 WinProc은 같은 static 변수만 처리가능하므로
	static bool m_Loop;

	class CTimer* m_Timer;
	float m_DeltaTime;
	float m_TimeScale;
	int		m_RandNum;

	//디버그용
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
	//에딧모드 체크용 변수
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