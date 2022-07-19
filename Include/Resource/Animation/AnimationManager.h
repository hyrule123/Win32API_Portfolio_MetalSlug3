#pragma once

#include "../../GameInfo.h"


class CAnimationManager
{
	friend class CResourceManager;

private:
	std::unordered_map<std::string, CSharedPtr<class CAnimationSequence>> m_mapSequence;

	CAnimationManager();
	~CAnimationManager();
	bool Init();

private:
	class CAnimationSequence* FindAnimSequence(const std::string& Name);
	void ReleaseAnimation(const std::string& Name);

	bool CreateAnimationSequence(const std::string& Name, class CTexture* Tex);

	void SetAnimLoaded(const std::string& Name, bool isLoaded = true);
	bool CheckAnimLoaded(const std::string& Name);
	bool AddAnimationSpriteFrame(const std::string& Name,
		const Vector2& Start, const Vector2& End, 
		const Vector2& Offset = Vector2(0.f, 0.f));
	bool AddAnimationSpriteFrame(const std::string& Name,
		float PosX, float PosY, 
		float SizeX, float SizeY, 
		float OffsetX = 0.f, float OffsetY = 0.f);
	bool AddAnimationFrameFrame(const std::string& Name);

	
	//에디트 모드용 로드된 AnimationSequence를 모두 제거하는 메소드
	void ClearAnimSeq();

};

