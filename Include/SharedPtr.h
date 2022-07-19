#pragma once

/*
[SharedPtr.h]
* Ref(���� ī����) Ŭ������ ���� Ȱ���ϴ� Ŭ����
* ������ ������ ������ ���� ����� ���, SharedPtr Ŭ������ �����Ѵ�.
*/

template <typename T>
class CSharedPtr
{
public:
	CSharedPtr() :
		m_Ptr(nullptr)
	{

	}
	//���� ������
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


	//������ �����͸� ������ �ִ� �ּҿ� ���ο� �ּҸ� '����'
	//Ŭ���� ���·� �񱳸� �� ���� �ְ�
	//�ƿ� ������ �ּҸ� ���� ���� �����Ƿ�
	void operator = (const CSharedPtr<T>& ptr)
	{
		//������ �����ϰ� �ִ� ��ü�� ���� ��� ī��Ʈ�� 1 �����Ѵ�.
		if (m_Ptr)
			m_Ptr->Release();


		//���ο� �ּҸ� ���
		m_Ptr = ptr.m_Ptr;

		//m_Ptr�� nullPtr�� �ƴ� ��� �ڽ��� ���۷��� ī��Ʈ�� �÷��ش�.
		if (m_Ptr)
		{
			m_Ptr->AddRef();
		}
	}
	//��ó�� SharedPtr ��ü�� �ƴ϶� ������ ������ ���Կ����� ���� �� �ִ�.
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

