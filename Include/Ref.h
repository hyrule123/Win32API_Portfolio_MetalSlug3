#pragma once

#include "GameInfo.h"

class CRef
{
public:
	CRef();
	CRef(const CRef& ref);
	virtual ~CRef();

	void AddRef();
	void Release();

protected:
	int m_RefCount;
	size_t m_TypeID;
	std::string m_TypeName;
	std::string m_Name;


	//�̰� false�� �Ǹ� ���� ��⿡ ��
	bool m_Active;
	//�̰� false�� �Ǹ� �Ͻ������θ� ��Ȱ��ȭ
	bool m_Enable;

public:
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

	int GetRefCount()	const;
	void SetName(const std::string& Name);
	const std::string& GetName() const;
	size_t GetTypeID() const;
	const std::string& GetTypeName() const;
	void SetActive(bool Active);
	void SetEnable(bool Enable);
	bool GetActive() const;
	bool GetEnable() const;

	//T �۸������� ���� Ÿ�԰� ���� Ÿ������ üũ
	template <typename T>
	bool CheckTypeID() const
	{
		return m_TypeID == typeid(T).hash_code();
	}

	template <typename T>
	void SetTypeID()
	{
		m_TypeName = typeid(T).name();
		m_TypeID = typeid(T).hash_code();
	}

};

