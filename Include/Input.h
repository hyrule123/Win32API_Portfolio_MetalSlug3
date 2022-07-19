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
	void* Obj; //�Լ��� ȣ���� ��ġ
	std::function<void()> func;	//bind�� �Լ��� �� ����

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

	//�迭�� 3�� -> EInput_Type::End�� ��ŭ ����(0 -> Down,1 -> Push ,2 -> Up)
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

	//����Ű ���ȴ��� ���� �����
	bool m_Ctrl;
	bool m_Alt;
	bool m_Shift;

	



private:	//////////////////���콺 ����/////////////////
	Vector2 m_MousePos;
	Vector2 m_MouseWorldPos;
	Vector2 m_MouseMove;
	HWND m_hWnd;
	bool m_MouseLDown;
	bool m_MouseLPush;
	bool m_MouseLUp;
	float m_CursorShowTimeLeft;	//�ð��� ������ ���콺 Ŀ�� ǥ�ø� ���� �ʴ´�.
	float m_CursorShowTimeSet;

	//���콺 �ִϸ��̼��� ������� CGameObject�� ���� �����Ѵ�.
	//class CSharedPtr<class CGameObject> m_CursorAnim;
	bool m_ShowDefaultCursor;
public:
	const Vector2& GetMousePos() const;
	const Vector2& GetMouseWorldPos() const;
	const Vector2& GetMouseMove() const;
	bool GetMouseLDown() const;
	bool GetMouseLPush() const;
	bool GetMouseLUp() const;

	//�ΰ��� ���콺 ��ǥ ������Ʈ�� �޼ҵ�
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
		//�ϴ� �Լ��� ����ϰ��� �ϴ� bindkey�� �ִ������� Ȯ��
		BindKey* bindkey = FindBindKey(Name);

		//������ ����
		if (!bindkey)
			return;

		//ã������ �Լ� ������� ���� -> ��Ͽ� ����ü ����
		BindFunction* bindfunc = new BindFunction;

		//ȣ���� �ּ� ���
		bindfunc->Obj = classptr;
		
		//�Լ��� ���ڸ� bind
		bindfunc->func = std::bind(Func, classptr);

		//�Ϸ�Ǿ����� VecFunction�� ����
		bindkey->VecFunction[(int)Type].push_back(bindfunc);

	}


	//�ƿ� �ݹ��Լ��� �� �����ع����� �޼ҵ�
	void ClearCallback();

	//�ش� Ŭ���� �����Ϳ� �ش��ϴ� �Է� ���ε��� ���� �����ϴ� �޼ҵ�
	//�Է��� �Ҵ��� Ŭ������ �Ҹ��ڿ� �ش� �޼ҵ带 �ڽ��� �ּҸ� ���ڷ� �����ϸ� �ȴ�.
	template <typename T>
	void DeleteBindClass(T* ClassPtr)
	{
		std::unordered_map<std::string, BindKey*>::iterator iter = m_mapBindKey.begin();
		std::unordered_map<std::string, BindKey*>::iterator iterEnd = m_mapBindKey.end();

		while (iter != iterEnd)
		{

			//������ �迭�� ���� ��ȸ�Ѵ�.
			for (size_t i = 0; i < (size_t)EInput_Type::End; ++i)
			{

				//������ �迭�� ���� iterator�� ��ȸ�Ѵ�(erase �Լ� ����ϱ� ����)
				std::vector<BindFunction*>::iterator VecIter = iter->second->VecFunction[i].begin();
				std::vector<BindFunction*>::iterator VecIterEnd = iter->second->VecFunction[i].end();

				while (VecIter != VecIterEnd)
				{
					//���� ���ڿ� �Լ������Ϳ� ��ϵ� ��ü �ּҿ� ������
					if ((*VecIter)->Obj == ClassPtr)
					{
						//���� �Ҵ��� �켱 �����ϰ�
						SAFE_DELETE((*VecIter));

						//�ش� �ڸ��� erase()�� ���� ���� + ������ �б� ���ش�.
						VecIter = iter->second->VecFunction[i].erase(VecIter);

						//���� iterator�ǳ��ڸ��� �޴´�.
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

