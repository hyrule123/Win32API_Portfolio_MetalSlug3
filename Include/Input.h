#pragma once

#include "GameInfo.h"
#include "Singleton.h"
#include "Ref.h"

enum class EInput_Type
{
	Down,
	Push,
	Up,
	End
};


struct KeyState
{
	unsigned char Key;
	
	bool Down;
	bool Push;
	bool Up;

	KeyState() :
		Key(0),
		Down(false),
		Push(false),
		Up(false)
	{}

};

struct BindFunction
{
	void* Obj; //함수를 호출한 위치
	std::function<void()> func;	//bind된 함수가 들어갈 변수

	BindFunction():
		Obj(nullptr)
	{}
};

struct BindKey
{
	std::string Name;

	KeyState* BindKeyState;

	bool Ctrl;
	bool Alt;
	bool Shift;

	//배열을 3개 -> EInput_Type::End개 만큼 생성(0 -> Down,1 -> Push ,2 -> Up)
	std::vector<BindFunction*> VecFunction[(int)EInput_Type::End];
	

	BindKey() :
		Ctrl(false),
		Alt(false),
		Shift(false),
		BindKeyState(nullptr)
	{
	}
};


class CInput
{
public:
	bool Init(HWND hWnd);
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	void Render(HDC hDC, float DeltaTime);


private:
	std::unordered_map<unsigned char, KeyState*> m_mapKeyState;
	std::unordered_map<std::string, BindKey*> m_mapBindKey;

	//조합키 눌렸는지 여부 저장용
	bool m_Ctrl;
	bool m_Alt;
	bool m_Shift;

	



private:	//////////////////마우스 관련/////////////////
	Vector2 m_MousePos;
	Vector2 m_MouseWorldPos;
	Vector2 m_MouseMove;
	HWND m_hWnd;
	bool m_MouseLDown;
	bool m_MouseLPush;
	bool m_MouseLUp;
	float m_CursorShowTimeLeft;	//시간이 지나면 마우스 커서 표시를 하지 않는다.
	float m_CursorShowTimeSet;

	//마우스 애니메이션을 들고있을 CGameObject를 따로 생성한다.
	//class CSharedPtr<class CGameObject> m_CursorAnim;
	bool m_ShowDefaultCursor;
public:
	const Vector2& GetMousePos() const;
	const Vector2& GetMouseWorldPos() const;
	const Vector2& GetMouseMove() const;
	bool GetMouseLDown() const;
	bool GetMouseLPush() const;
	bool GetMouseLUp() const;

	//인게임 마우스 좌표 업데이트용 메소드
	void ComputeWorldPos(const Vector2& CamPos);
	////////////////////////////////////////

private:
	void UpdateMouse(float DeltaTime);
	void UpdateKeyState(float DeltaTime);
	void UpdateBindKey(float DeltaTime);

public:

	void SetKeyCtrl(const std::string& Name, bool Ctrl = true);
	void SetKeyAlt(const std::string& Name, bool Alt = true);
	void SetKeyShift(const std::string& Name, bool Shift = true);

	KeyState* FindKeyState(unsigned char key);
	BindKey* FindBindKey(const std::string Name);


	bool AddBindKey(const std::string& Name, unsigned char key);


	template <typename T>
	void AddBindFunction(const std::string& Name,
		EInput_Type Type,
		T* classptr,
		void (T::* Func)())
	{
		//일단 함수를 등록하고자 하는 bindkey가 있는지부터 확인
		BindKey* bindkey = FindBindKey(Name);

		//없으면 리턴
		if (!bindkey)
			return;

		//찾았으면 함수 등록절차 진행 -> 등록용 구조체 생성
		BindFunction* bindfunc = new BindFunction;

		//호출자 주소 등록
		bindfunc->Obj = classptr;
		
		//함수에 인자를 bind
		bindfunc->func = std::bind(Func, classptr);

		//완료되었으면 VecFunction에 삽입
		bindkey->VecFunction[(int)Type].push_back(bindfunc);

	}


	//아예 콜백함수를 싹 제거해버리는 메소드
	void ClearCallback();

	//해당 클래스 포인터에 해당하는 입력 바인딩을 전부 제거하는 메소드
	//입력을 할당한 클래스의 소멸자에 해당 메소드를 자신의 주소를 인자로 전달하면 된다.
	template <typename T>
	void DeleteBindClass(T* ClassPtr)
	{
		std::unordered_map<std::string, BindKey*>::iterator iter = m_mapBindKey.begin();
		std::unordered_map<std::string, BindKey*>::iterator iterEnd = m_mapBindKey.end();

		while (iter != iterEnd)
		{

			//이차원 배열의 행을 순회한다.
			for (size_t i = 0; i < (size_t)EInput_Type::End; ++i)
			{

				//이차원 배열의 열을 iterator로 순회한다(erase 함수 사용하기 위함)
				std::vector<BindFunction*>::iterator VecIter = iter->second->VecFunction[i].begin();
				std::vector<BindFunction*>::iterator VecIterEnd = iter->second->VecFunction[i].end();

				while (VecIter != VecIterEnd)
				{
					//들어온 인자와 함수포인터에 등록된 객체 주소와 같으면
					if ((*VecIter)->Obj == ClassPtr)
					{
						//동적 할당을 우선 해제하고
						SAFE_DELETE((*VecIter));

						//해당 자리를 erase()를 통해 제거 + 앞으로 밀기 해준다.
						VecIter = iter->second->VecFunction[i].erase(VecIter);

						//새로 iterator의끝자리를 받는다.
						VecIterEnd = iter->second->VecFunction[i].end();						
						continue;
					}
					++VecIter;
				}
			}
			++iter;
		}
	}

	DECLARE_SINGLETON(CInput)
};

