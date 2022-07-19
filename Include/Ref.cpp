#include "Ref.h"

CRef::CRef():
	m_RefCount(0),
	m_TypeID(0),
	m_Active(true),
	m_Enable(true)
{
}

CRef::CRef(const CRef& ref):
	m_RefCount(0),
	m_Name(ref.m_Name),
	m_TypeID(ref.m_TypeID),
	m_TypeName(ref.m_TypeName),
	m_Enable(ref.m_Enable),
	m_Active(ref.m_Active)
{
}

CRef::~CRef()
{
}

void CRef::AddRef()
{
	++m_RefCount;
}

void CRef::Release()
{
	--m_RefCount;
	if (m_RefCount <= 0)
	{
		delete this;
		return;
	}
}

void CRef::Save(FILE* File)
{
	int	Length = (int)m_Name.length();

	fwrite(&Length, sizeof(int), 1, File);
	fwrite(m_Name.c_str(), sizeof(char), Length, File);

	Length = (int)m_TypeName.length();

	fwrite(&Length, sizeof(int), 1, File);
	fwrite(m_TypeName.c_str(), sizeof(char), Length, File);

	fwrite(&m_TypeID, sizeof(size_t), 1, File);
}

void CRef::Load(FILE* File)
{
	int	Length = 0;
	char	Text[256] = {};

	fread(&Length, sizeof(int), 1, File);
	fread(Text, sizeof(char), Length, File);
	m_Name = Text;

	memset(Text, 0, 256);

	fread(&Length, sizeof(int), 1, File);
	fread(Text, sizeof(char), Length, File);
	m_TypeName = Text;

	fread(&m_TypeID, sizeof(size_t), 1, File);
}

int CRef::GetRefCount() const
{
	return m_RefCount;
}

void CRef::SetName(const std::string& Name)
{
	m_Name = Name;
}

const std::string& CRef::GetName() const
{
	return m_Name;
}

size_t CRef::GetTypeID() const
{
	return m_TypeID;
}

const std::string& CRef::GetTypeName() const
{
	return m_TypeName;
}

void CRef::SetActive(bool Active)
{
	m_Active = Active;
}

void CRef::SetEnable(bool Enable)
{
	m_Enable = Enable;
}

bool CRef::GetActive() const
{
	return m_Active;
}

bool CRef::GetEnable() const
{
	return m_Enable;
}


