#pragma once

#include "../../Ref.h"
#include "../../GameInfo.h"

class CAnimationSequence :
	public CRef
{
	friend class CAnimationManager;

	//편집기에서 자유롭게 관리할 수 있도록
	friend class CSceneEdit;

private:
	CSharedPtr<class CTexture> m_Texture;
	std::vector<Sprite_FrameData> m_vecFrame;

	CAnimationSequence();
	~CAnimationSequence();


public:
	class CTexture* GetTexture()   const;
	ETextureType GetTextureType()   const;
	const Sprite_FrameData& GetFrameData(int index)   const;
	std::vector<Sprite_FrameData> GetFrameDataAll();
	bool m_AnimLoaded;
	size_t GetFrameCount() const;
	

private:
	void SetAnimLoaded(bool isLoaded = true);
	bool CheckAnimLoaded() const;
	//Sprite 형식의 텍스처 타입을 사용할 떄 수동으로 잡아줄 메소드
	void AddSpriteFrame(const Vector2& start, const Vector2& end, const Vector2& Offset);
	void AddSpriteFrame(float PosX, float PosY, float SizeX, float SizeY, float OffsetX = 0.f, float OffsetY = 0.f);


	//Frame 형식의 텍스처 타입을 사용할 떄 애니메이션 프레임을 이미지 사이즈에 맞게 자동 생성해주는 메소드.
	void AddFrameFrame();
	



};

