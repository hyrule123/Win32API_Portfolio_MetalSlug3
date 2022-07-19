#include "AnimationManager.h"

#include "AnimationSequence.h"

#include "../Texture/Texture.h"

CAnimationManager::CAnimationManager()
{
}

CAnimationManager::~CAnimationManager()
{

}

bool CAnimationManager::Init()
{



	return true;
}

CAnimationSequence* CAnimationManager::FindAnimSequence(const std::string& Name)
{
	auto iter = m_mapSequence.find(Name);

	if (iter == m_mapSequence.end())
		return nullptr;


	return iter->second;
}

void CAnimationManager::ReleaseAnimation(const std::string& Name)
{
	auto iter = m_mapSequence.find(Name);

	if (iter == m_mapSequence.end())
		return;

	if (iter->second->GetRefCount() == 1)
		m_mapSequence.erase(iter);
}

bool CAnimationManager::CreateAnimationSequence(const std::string& Name, CTexture* Tex)
{
	CAnimationSequence* Seq = FindAnimSequence(Name);

	if (Seq)
		return false;

	Seq = new CAnimationSequence;

	Seq->m_Texture = Tex;
	Seq->SetName(Name);

	m_mapSequence.insert(std::make_pair(Name, Seq));

	return true;
}



void CAnimationManager::SetAnimLoaded(const std::string& Name, bool isLoaded)
{
	CAnimationSequence* Seq = FindAnimSequence(Name);

	if (!Seq)
		return;

	return Seq->SetAnimLoaded(isLoaded);
}

bool CAnimationManager::CheckAnimLoaded(const std::string& Name)
{
	CAnimationSequence* Seq = FindAnimSequence(Name);

	if (!Seq)
		return false;

	return Seq->CheckAnimLoaded();
}

bool CAnimationManager::AddAnimationSpriteFrame(const std::string& Name, const Vector2& Start, const Vector2& End, const Vector2& Offset)
{
	CAnimationSequence* Seq = FindAnimSequence(Name);

	if (!Seq)
		return false;

	Seq->AddSpriteFrame(Start, End, Offset);

	return true;
}

bool CAnimationManager::AddAnimationSpriteFrame(const std::string& Name, float PosX, float PosY, float SizeX, float SizeY, float OffsetX, float OffsetY)
{
	CAnimationSequence* Seq = FindAnimSequence(Name);

	if (!Seq)
		return false;

	Seq->AddSpriteFrame(PosX, PosY, SizeX, SizeY, OffsetX, OffsetY);

	return true;
}

bool CAnimationManager::AddAnimationFrameFrame(const std::string& Name)
{
	CAnimationSequence* Seq = FindAnimSequence(Name);

	if (!Seq)
		return false;

	Seq->AddFrameFrame();

	return true;
}

void CAnimationManager::ClearAnimSeq()
{
	m_mapSequence.clear();
}

