#pragma once

/*
[SharedPtr.h]
* Ref(참조 카운팅) 클래스와 같이 활용하는 클래스
* 앞으로 포인터 변수를 직접 만드는 대신, SharedPtr 클래스를 생성한다.
*/

template <typename T>
class CSharedPtr
{
public:
	CSharedPtr() :
		m_Ptr(nullptr)
	{

	}
	//복사 생성자
	CSharedPtr(const CSharedPtr<T>& ptr)
	{
		m_Ptr = ptr.m_Ptr;

		if (m_Ptr)
			m_Ptr->AddRef();
	}

	CSharedPtr(T* ptr)
	{
		m_Ptr = ptr;

		if (m_Ptr)
			m_Ptr->AddRef();
	}


	~CSharedPtr()
	{
		if (m_Ptr)
			m_Ptr->Release();
	}

private:
	T* m_Ptr;


public:


	//기존에 포인터를 가지고 있던 주소에 새로운 주소를 '대입'
	//클래스 형태로 비교를 할 수도 있고
	//아예 포인터 주소를 비교할 수도 있으므로
	void operator = (const CSharedPtr<T>& ptr)
	{
		//기존에 참조하고 있던 객체가 있을 경우 카운트를 1 감소한다.
		if (m_Ptr)
			m_Ptr->Release();


		//새로운 주소를 등록
		m_Ptr = ptr.m_Ptr;

		//m_Ptr이 nullPtr이 아닐 경우 자신을 레퍼런스 카운트를 늘려준다.
		if (m_Ptr)
		{
			m_Ptr->AddRef();
		}
	}
	//위처럼 SharedPtr 객체가 아니라 포인터 변수로 대입연산이 들어올 수 있다.
	void operator = (T* ptr)
	{
		if (m_Ptr)
			m_Ptr->Release();

		m_Ptr = ptr;

		if (m_Ptr)
			m_Ptr->AddRef();
	}

	bool operator == (const CSharedPtr<T>& ptr) const
	{
		return m_Ptr == ptr.m_Ptr;
	}
	bool operator == (T* ptr) const
	{
		return m_Ptr == ptr;
	}

	bool operator != (const CSharedPtr<T>& ptr) const
	{
		return m_Ptr != ptr.m_Ptr;
	}
	bool operator != (T* ptr) const
	{
		return m_Ptr != ptr;
	}


	operator T* () const
	{
		return m_Ptr;
	}

	T* operator ->() const
	{
		return m_Ptr;
	}

	T* operator * () const
	{
		return m_Ptr;
	}

	T* Get() const
	{
		return m_Ptr;
	}
};

