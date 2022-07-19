#pragma once

#define DECLARE_SINGLETON(Type)	\
private:\
	static Type* m_Inst;\
public:\
	static Type* GetInst()\
	{\
		if(!m_Inst)\
			m_Inst = new Type;\
		return m_Inst;\
	}\
	static void DestroyInst()\
	{\
		if(m_Inst)\
		{\
			delete m_Inst;\
			m_Inst = nullptr;\
		}\
	}\
private:\
	Type();\
	~Type();

#define DEFINITION_SINGLETON(Type)	Type* Type::m_Inst = nullptr;

