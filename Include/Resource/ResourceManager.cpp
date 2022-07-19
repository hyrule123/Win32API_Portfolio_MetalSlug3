#include "ResourceManager.h"

#include "Texture/TextureManager.h"
#include "Animation/AnimationManager.h"
#include "Sound/SoundManager.h"
#include "Font/FontManager.h"


DEFINITION_SINGLETON(CResourceManager)

CResourceManager::CResourceManager():
	m_TextureManager(nullptr)
{

}

CResourceManager::~CResourceManager()
{

	//애니메이션매니저 먼저 제거
	SAFE_DELETE(m_FontManager);
	SAFE_DELETE(m_SoundManager);
	SAFE_DELETE(m_AnimationManager);
	SAFE_DELETE(m_TextureManager);
}

bool CResourceManager::Init()
{
	//각 리소스별 매니저 초기화
	m_TextureManager = new CTextureManager;

	if (!m_TextureManager->Init())
		return false;

	m_AnimationManager = new CAnimationManager;

	if (!m_AnimationManager->Init())
		return false;


	m_SoundManager = new CSoundManager;
	if (!m_SoundManager->Init())
		return false;

	m_FontManager = new CFontManager;
	if (!m_FontManager->Init())
		return false;


	return true;
}

void CResourceManager::Update()
{
	m_SoundManager->Update();
}





//////////////////////////////// FONT /////////////////////////////////
bool CResourceManager::LoadFontInfo(const std::string& Name, const std::tstring& FontName, int Width, int Height)
{
	return m_FontManager->LoadFontInfo(Name, FontName, Width, Height);
}

bool CResourceManager::LoadFontResource(
	const std::tstring& FontFileName, 
	const std::string& PathName)
{
	return m_FontManager->LoadFontResource(FontFileName, PathName);
}

void CResourceManager::SetFont(const std::string& Name, HDC hDC)
{
	return m_FontManager->SetFont(Name, hDC);
}

void CResourceManager::ResetFont(const std::string& Name, HDC hDC)
{
	return m_FontManager->ResetFont(Name, hDC);
}

CFont* CResourceManager::FindFont(const std::string& Name)
{
	return m_FontManager->FindFont(Name);
}

void CResourceManager::ReleaseFont(const std::string& Name)
{
	return m_FontManager->ReleaseFont(Name);
}
//////////////////////////////////////////////////////////////////////////







//////////////////////////////// Animation /////////////////////////////////////
CAnimationSequence* CResourceManager::FindAnimationSequence(const std::string& Name)
{
	return m_AnimationManager->FindAnimSequence(Name);
}

void CResourceManager::ReleaseAnimation(const std::string& Name)
{
	m_AnimationManager->ReleaseAnimation(Name);
}

void CResourceManager::ClearAnimSeq()
{
	m_AnimationManager->ClearAnimSeq();
}

void CResourceManager::SetAnimLoaded(const std::string& Name, bool isLoaded)
{
	m_AnimationManager->SetAnimLoaded(Name, isLoaded);
}

bool CResourceManager::CheckAnimLoaded(const std::string& Name)
{
	return m_AnimationManager->CheckAnimLoaded(Name);
}

bool CResourceManager::AddAnimationSpriteFrame(const std::string& Name,
	const Vector2& Start, const Vector2& End, 
	const Vector2& Offset)
{
	return m_AnimationManager->AddAnimationSpriteFrame(Name, Start, End, Offset);
}

bool CResourceManager::AddAnimationSpriteFrame(const std::string& Name, 
	float PosX, float PosY, 
	float SizeX, float SizeY, 
	float OffsetX, float OffsetY
)
{
	return m_AnimationManager->AddAnimationSpriteFrame(Name, PosX, PosY, SizeX, SizeY, OffsetX, OffsetY);
}

bool CResourceManager::AddAnimationFrameFrame(const std::string& Name)
{
	return m_AnimationManager->AddAnimationFrameFrame(Name);
}

bool CResourceManager::CreateAnimationSequence(const std::string& Name, CTexture* Texture)
{
	return m_AnimationManager->CreateAnimationSequence(Name, Texture);
}

bool CResourceManager::CreateAnimationSequence(const std::string& Name, const std::string& TexName)
{

	//혹시나 애니메이션을 만드는 데 필요한 텍스처가 <없을> 경우 return
	CTexture* Tex = FindTexture(TexName);
	if (!Tex)
		return false;

	return m_AnimationManager->CreateAnimationSequence(Name, Tex);

}

bool CResourceManager::CreateAnimationSequence(const std::string& Name, const std::string& TexName, const std::tstring& FileName, const std::string& PathName)
{
	CTexture* Tex = FindTexture(TexName);

	if (Tex)
		return m_AnimationManager->CreateAnimationSequence(Name, Tex);
	

	//텍스처 로드에 실패했으면 return
	if (!m_TextureManager->LoadTexture(TexName, FileName, PathName))
		return false;

	//성공했으면 주소 받아다가
	Tex = FindTexture(TexName);

	//등록
	return m_AnimationManager->CreateAnimationSequence(Name, Tex);

}

bool CResourceManager::CreateAnimationSequenceFullPath(const std::string& Name, const std::string& TexName, const std::tstring& FullPath)
{
	CTexture* Tex = FindTexture(TexName);

	if (Tex)
		return m_AnimationManager->CreateAnimationSequence(Name, Tex);


	//텍스처 로드에 실패했으면 return
	if (!m_TextureManager->LoadTextureFullPath(TexName, FullPath))
		return false;

	//성공했으면 주소 받아다가
	Tex = FindTexture(TexName);

	//등록
	return m_AnimationManager->CreateAnimationSequence(Name, Tex);
}



bool CResourceManager::CreateAnimationSequence(const std::string& Name, const std::string& TexName, const std::vector<std::tstring>& vecFileName, const std::string& PathName)
{
	CTexture* Tex = FindTexture(TexName);

	if (Tex)
		return m_AnimationManager->CreateAnimationSequence(Name, Tex);


	//텍스처 로드에 실패했으면 return
	if (!m_TextureManager->LoadTexture(TexName, vecFileName))
		return false;

	//성공했으면 주소 받아다가
	Tex = FindTexture(TexName);


	//등록
	return m_AnimationManager->CreateAnimationSequence(Name, Tex);
}

bool CResourceManager::CreateAnimationSequenceFullPath(const std::string& Name, const std::string& TexName, const std::vector<std::tstring>& vecFullPath)
{
	CTexture* Tex = FindTexture(TexName);

	if (Tex)
		return m_AnimationManager->CreateAnimationSequence(Name, Tex);


	//텍스처 로드에 실패했으면 return
	if (!m_TextureManager->LoadTextureFullPath(TexName, vecFullPath))
		return false;

	//성공했으면 주소 받아다가
	Tex = FindTexture(TexName);

	//등록
	return m_AnimationManager->CreateAnimationSequence(Name, Tex);
}


//////////////////////////////////////////////////////////////////////






//////////////////////////////// Texture ///////////////////////////

CTexture* CResourceManager::FindTexture(const std::string& Name)
{
	return m_TextureManager->FindTexture(Name);
}

void CResourceManager::ReleaseTexture(const std::string& Name)
{
	m_TextureManager->ReleaseTexture(Name);
}


bool CResourceManager::SetColorKey(const std::string& Name, 
	unsigned char r, unsigned char g, unsigned char b, int index)
{
	return m_TextureManager->SetColorKey(Name, r, g, b, index);
}


bool CResourceManager::LoadTexture(const std::string& Name, const std::tstring& FileName, const std::string& PathName, EReactionChannel MaxChannel)
{
	return m_TextureManager->LoadTexture(Name, FileName, PathName, MaxChannel);
}

bool CResourceManager::LoadTextureFullPath(const std::string& Name, const std::tstring& FullPath, EReactionChannel MaxChannel)
{
	return m_TextureManager->LoadTextureFullPath(Name, FullPath, MaxChannel);
}



bool CResourceManager::LoadTextureByTextureNumbers(const std::string& Name, const std::tstring& FolderPath, int NumberOfTexture, const std::string& PathName)
{
	return m_TextureManager->LoadTextureByTextureNumbers(Name, FolderPath, NumberOfTexture, PathName);
}
bool CResourceManager::LoadTexture(const std::string& Name, const std::vector<std::tstring>& vecFileName, const std::string& PathName)
{
	return m_TextureManager->LoadTexture(Name, vecFileName, PathName);
}

bool CResourceManager::LoadTextureFullPath(const std::string& Name, const std::vector<std::tstring>& vecFullPath)
{
	return m_TextureManager->LoadTextureFullPath(Name, vecFullPath);
}

CTexture* CResourceManager::LoadTexture(FILE* File)
{
	return m_TextureManager->LoadTexture(File);
}


///////////////////////////////////////////////////////////////////////////////

bool CResourceManager::CreateSoundChannelGroup(const std::string& Name)
{
	return m_SoundManager->CreateSoundChannelGroup(Name);
}

FMOD::ChannelGroup* CResourceManager::FindChannelGroup(const std::string& Name)
{
	return m_SoundManager->FindChannelGroup(Name);
}

CSound* CResourceManager::FindSound(const std::string& Name)
{
	return m_SoundManager->FindSound(Name);
}

bool CResourceManager::LoadSound(const std::string& GroupName, const std::string& Name, bool Loop, const char* FileName, const std::string& PathName)
{
	return m_SoundManager->LoadSound(GroupName, Name, Loop, FileName, PathName);
}

bool CResourceManager::SetMasterVolume(int Volume)
{
	return m_SoundManager->SetMasterVolume(Volume);
}

bool CResourceManager::SetVolume(const std::string& GroupName, int Volume)
{
	return m_SoundManager->SetVolume(GroupName, Volume);
}

bool CResourceManager::SoundPlay(const std::string& Name)
{
	return m_SoundManager->SoundPlay(Name);
}

bool CResourceManager::SoundStop(const std::string& Name)
{
	return m_SoundManager->SoundStop(Name);
}

bool CResourceManager::SoundPause(const std::string& Name)
{
	return m_SoundManager->SoundPause(Name);
}

bool CResourceManager::SoundResume(const std::string& Name)
{
	return m_SoundManager->SoundResume(Name);
}

bool CResourceManager::GetPlaying(const std::string& Name)
{
	return m_SoundManager->GetPlaying(Name);
}

bool CResourceManager::SoundFadeIn(const std::string& Name, float Duration)
{
	return m_SoundManager->SoundFadeIn(Name, Duration);
}

bool CResourceManager::SoundFadeOut(const std::string& Name, float Duration)
{
	return m_SoundManager->SoundFadeOut(Name, Duration);
}

bool CResourceManager::SoundPlayOneChan(const std::string& Name)
{
	return m_SoundManager->SoundPlayOneChan(Name);
}

bool CResourceManager::SoundSetNoDelay(const std::string& Name)
{
	return m_SoundManager->SoundSetNodelay(Name);
}

void CResourceManager::ReleaseSound(const std::string& Name)
{

	m_SoundManager->ReleaseSound(Name);

}



