#pragma once

#include "../Singleton.h"
#include "../GameInfo.h"


class CResourceManager
{
public:
	bool Init();
	void Update();

private:
	class CTextureManager* m_TextureManager;
	class CAnimationManager* m_AnimationManager;
	class CSoundManager* m_SoundManager;
	class CFontManager* m_FontManager;


public:///////////////////////////// FONT ///////////////////////////
	bool LoadFontInfo(const std::string& Name, const std::tstring& FontName,
		int Width, int Height);
	bool LoadFontResource(
		const std::tstring& FontFileName,
		const std::string& PathName = FONT_PATH);
	void SetFont(const std::string& Name, HDC hDC);
	void ResetFont(const std::string& Name, HDC hDC);
	class CFont* FindFont(const std::string& Name);
	void ReleaseFont(const std::string& Name);
/////////////////////////////////////////////////////////////////////////





public://////////////////////////// SOUND ///////////////////////////
	//생성
	bool CreateSoundChannelGroup(const std::string& Name);

	//탐색
	FMOD::ChannelGroup* FindChannelGroup(const std::string& Name);
	class CSound* FindSound(const std::string& Name);

	//로딩
	bool LoadSound(
		const std::string& GroupName,
		const std::string& Name,
		bool Loop,
		const char* FileName,
		const std::string& PathName = SOUND_PATH);

	//기타 설정
	bool SetMasterVolume(int Volume);
	bool SetVolume(const std::string& GroupName, int Volume);
	bool SoundPlay(const std::string& Name);
	bool SoundStop(const std::string& Name);
	bool SoundPause(const std::string& Name);
	bool SoundResume(const std::string& Name);
	bool GetPlaying(const std::string& Name);
	bool SoundFadeIn(const std::string& Name, float Duration);
	bool SoundFadeOut(const std::string& Name, float Duration);
	bool SoundPlayOneChan(const std::string& Name);
	bool SoundSetNoDelay(const std::string& Name);


	//제거(SceneResource 용)
	void ReleaseSound(const std::string& Name);

	///////////////////////////////////////////////////////////////////////
	
	
	
	////////////////////////Animation/////////////////////////////////
public:
	class CAnimationSequence* FindAnimationSequence(const std::string& Name);
	void ReleaseAnimation(const std::string& Name);
	void ClearAnimSeq();

	void SetAnimLoaded(const std::string& Name, bool isLoaded = true);
	bool CheckAnimLoaded(const std::string& Name);
	bool AddAnimationSpriteFrame(
		const std::string& Name, 
		const Vector2& Start, const Vector2& End, 
		const Vector2& Offset = Vector2(0.f, 0.f)
	);
	bool AddAnimationSpriteFrame(const std::string& Name,
		float PosX, float PosY, float SizeX, float SizeY,
		float OffsetX = 0.f, float OffsetY = 0.f);
	bool AddAnimationFrameFrame(const std::string& Name);


	bool CreateAnimationSequence(const std::string& Name, class CTexture* Texture);
	bool CreateAnimationSequence(const std::string& Name, const std::string& TexName);
	bool CreateAnimationSequence(const std::string& Name, const std::string& TexName,
		const std::tstring& FileName, const std::string& PathName = TEXTURE_PATH);
	bool CreateAnimationSequenceFullPath(const std::string& Name, const std::string& TexName,
		const std::tstring& FullPath);


	bool CreateAnimationSequence(const std::string& Name, 
		const std::string& TexName,
		const std::vector<std::tstring>& vecFileName, 
		const std::string& PathName = TEXTURE_PATH);
	bool CreateAnimationSequenceFullPath(const std::string& Name, 
		const std::string& TexName,
		const std::vector<std::tstring>& vecFullPath);




	


	////////////////////////Texture///////////////////////////////////////
public:

	class CTexture* FindTexture(const std::string& Name);
	void ReleaseTexture(const std::string& Name);

	bool SetColorKey(const std::string& Name,
		unsigned char r = 255,
		unsigned char g = 0,
		unsigned char b = 255,
		int index = -1);

	
	bool LoadTexture(const std::string& Name, const std::tstring& FileName, const std::string& PathName = TEXTURE_PATH, EReactionChannel MaxChannel = EReactionChannel::Normal);
	bool LoadTextureFullPath(const std::string& Name, const std::tstring& FullPath, EReactionChannel MaxChannel = EReactionChannel::Normal);


	bool LoadTextureByTextureNumbers(const std::string& Name,
		const std::tstring& FolderPath, int NumberOfTexture, const std::string& PathName = TEXTURE_PATH);

	bool LoadTexture(const std::string& Name, const std::vector<std::tstring>& vecFileName, const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const std::vector<std::tstring>& vecFullPath);


	//세이브, 로드 관련
	class CTexture* LoadTexture(FILE* File);

////////////////////////////////////////////////////////////////////////////




	DECLARE_SINGLETON(CResourceManager)
};

